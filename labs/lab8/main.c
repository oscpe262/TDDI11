#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <X11/Xlib.h>
#include <sys/time.h>

	/* Automatic Bike Lights
		Complie with:
			gcc main.c -lX11 -lm 	*/


int main(int argc, char **argv)
{
	//++++++ Initialization ++++++//
	int horizontalPosition, verticalPosition;
	int horizontalMotion, verticalMotion;
	double motionMetric = 0; 
	static double motionThreshold = 10;
	struct timeval  timeReadOut;	
	long double timerValueSeconds = 0;
	static long double blinkHalfCycleSeconds = 0.2;
	static long double waitBeforeDeactivatingAutoBlink = 2.0;
	long double nextBlinkAction = 0;
	long double nextMotionDeactivation = 0;
	static int leftButtonID = 1;
	static int middleButtonID = 2;
	//static int rightButtonID = 3; //for debug
	enum Light_States { NO_BATTERY, BATTERY_INSTALLED, BATTERY_REMOVED, MANUAL_OFF, MAN_OFF_BLINK, MANUAL_BLINK_OFF, MANUAL_BLINK_ON, MANUAL_BLINK_AUTO, AUTO_OFF, AUTO_BLINK_OFF, AUTO_BLINK_ON, AUTO_MANUAL_OFF } Light_State;
	Light_State = NO_BATTERY;
	bool left_button_pushed = false;
	bool middle_button_pushed = false;
	bool motion_detected = false;
	bool blink_on = false;
	bool light_on = false;
	bool light_previously_on = false; // for blink simulation
	bool exit_loop = false;
	// prepare environment for listening to Mouse
	Display *display;
	XEvent xevent;
	Window window;
	if( (display = XOpenDisplay(NULL)) == NULL )
		return -1;
	window = DefaultRootWindow(display);
	XAllowEvents(display, AsyncBoth, CurrentTime);
	XGrabPointer(display, window, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);

	//++++++ Main Body ++++++//	
	printf("Click Left Mouse Button to power-up the SmartLight (putting the batteries in). \n");
	while (true)
	{ 
		//++++++ Section 1. Read the Current Time ++++++// 
		gettimeofday(&timeReadOut, NULL);
		timerValueSeconds = (long double)timeReadOut.tv_sec + (long double)timeReadOut.tv_usec/1000000.0;
		//printf("Current timer value is %Lf seconds\n", timerValueSeconds); //for debug		

		//++++++ Section 2. Check Mouse Buttons ++++++//
		left_button_pushed = false;
		middle_button_pushed = false;
		if(XCheckMaskEvent(display, ButtonReleaseMask, &xevent))
		{
			// Check to turn on/off the system (batteries removed/installed)
			if(xevent.xbutton.button == leftButtonID)
				left_button_pushed = true;
			if(xevent.xbutton.button == middleButtonID)
				middle_button_pushed = true;
		}

		//++++++ Section 3. Check Mouse Movement ++++++//
		// update motion metric and motion_detected
		if(XCheckMaskEvent(display, PointerMotionMask, &xevent))
		{
			// calculate motion
			horizontalMotion = xevent.xmotion.x_root - horizontalPosition;
			verticalMotion   = xevent.xmotion.y_root - verticalPosition;
			// update position
			horizontalPosition = xevent.xmotion.x_root;
			verticalPosition   = xevent.xmotion.y_root;
			//printf("X = %d , Y = %d \n", horizontalPosition, verticalPosition); //for debug
			//printf("dX = %d , dY = %d \n", horizontalMotion, verticalMotion); //for debug
			motionMetric = 0.5 * motionMetric + 0.5 * sqrt(pow((double)horizontalMotion,2.0) + pow((double)verticalMotion,2.0));
			//printf("motionMetric = %f\n", motionMetric); //for debug
			if (motionMetric > motionThreshold)
			{
				motion_detected = true;
				nextMotionDeactivation = timerValueSeconds + waitBeforeDeactivatingAutoBlink;
				//printf("nextMotionDeactivation = %Lf\n", nextMotionDeactivation); //for debug
			}
		}

		//++++++ Section 4. Time-Based Actions ++++++//		
		// motion detection time-out
		if (timerValueSeconds > nextMotionDeactivation)
			motion_detected = false;
		// Blink ON/OFF timing
		if (timerValueSeconds > nextBlinkAction)
		{
			nextBlinkAction = timerValueSeconds + blinkHalfCycleSeconds;
			blink_on = !blink_on;
		}	  
	
		//++++++ Section 5. Update States ++++++//		
		switch(Light_State)
		{
			case NO_BATTERY:
				if(left_button_pushed)
					Light_State = BATTERY_INSTALLED;
				break;
			case BATTERY_INSTALLED:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else
					Light_State = MANUAL_OFF;
				break;
			case MANUAL_OFF:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = MAN_OFF_BLINK;
				break;
			case MAN_OFF_BLINK:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else 
					Light_State = MANUAL_BLINK_ON;
				break;
			case MANUAL_BLINK_ON:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = MANUAL_BLINK_AUTO;
				else if(!blink_on)
					Light_State = MANUAL_BLINK_OFF;
				break;
			case MANUAL_BLINK_OFF:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = MANUAL_BLINK_AUTO;
				else if(blink_on)
					Light_State = MANUAL_BLINK_ON;
				break;
			case MANUAL_BLINK_AUTO:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else 
					Light_State = AUTO_BLINK_ON;
				break;			
			case AUTO_OFF:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = AUTO_MANUAL_OFF;
				else if(motion_detected)
				{
					Light_State = AUTO_BLINK_ON;
					//printf("\n Motion detected: moving from AUTO_OFF to AUTO_BLINK_ON. \n"); //for debug
				}
				break;
			case AUTO_BLINK_ON:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = AUTO_MANUAL_OFF;
				else if(! motion_detected)
				{
					Light_State = AUTO_OFF;
					//printf("\n Motion not detected: moving from AUTO_BLINK_ON to AUTO_OFF. \n"); //for debug
				}
				else if(!blink_on)
					Light_State = AUTO_BLINK_OFF;
				break;
			case AUTO_BLINK_OFF:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else if(middle_button_pushed)
					Light_State = AUTO_MANUAL_OFF;
				else if(! motion_detected)
				{
					Light_State = AUTO_OFF;
					//printf("\n Motion not detected: moving from AUTO_BLINK_OFF to AUTO_OFF. \n"); //for debug
				}
				else if(blink_on)
					Light_State = AUTO_BLINK_ON;
				break;
			case AUTO_MANUAL_OFF:
				if(left_button_pushed)
					Light_State = BATTERY_REMOVED;
				else
					Light_State = MANUAL_OFF;
				break;
			case BATTERY_REMOVED:				
				break;
			default:
				Light_State = NO_BATTERY;
		}

		//++++++ Section 6. Generate Outputs ++++++//
		light_on = false;		
		switch(Light_State)
		{
			case NO_BATTERY:
				break;
			case BATTERY_INSTALLED:
				printf("Turning ON the SmartLight. To exit: Left Mouse Button (removing the batteries). \n");
				printf("SmartLight will start in MANUAL_OFF mode as default. To swith to other modes use Middle Mouse Button.\n");
				break;
			case MANUAL_OFF:
				break;
			case MAN_OFF_BLINK:
				printf("\n Changing mode from MANUAL OFF to MANUAL BLINK. \n");
				break;
			case MANUAL_BLINK_ON:
				light_on = true;
				break;
			case MANUAL_BLINK_OFF:
				break;
			case MANUAL_BLINK_AUTO:
				printf("\n Changing mode from MANUAL BLINK to AUTO. \n");
				break;
			case AUTO_OFF:
				break;
			case AUTO_BLINK_ON:
				light_on = true;
				break;
			case AUTO_BLINK_OFF:
				break;
			case AUTO_MANUAL_OFF:
				printf("\n Changing mode from AUTO to MANUAL OFF. \n");
				break;
			case BATTERY_REMOVED:				
				printf("\n Left Mouse Button is Clicked: Turning OFF the SmartLight (removing the batteries). \n");
				exit_loop = true;
				break;
			default:
				printf("\n Something went wrong! (Maybe, transient fault on state registeers?!) Switching to NO_BATTERY. \n");
		}

		//++++++ Section 7. Blink Simulation ++++++//
		// print X when the light starts receiving power
		// print _ when the light stops receiving power
		if(light_previously_on != light_on)
		{	
			if(light_on)
				printf("X");
			else
				printf("_");
			light_previously_on = light_on;
		}
		
		//++++++ Exit If Batteries Are Removed ++++++//		 	
		fflush(stdout);
		if(exit_loop)
			break;
	}
	return 0;
}
