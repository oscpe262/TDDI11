#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <X11/Xlib.h>


// compile and run
// gcc main.c -lm -lX11
// ./a.out
// To exit:
// ctrl+c 


int main(int argc, char **argv)
{
	//+++++ section 1. Initialization +++++

	bool blinkOn = false;
	int horizontalPosition, verticalPosition;
	int horizontalMotion, verticalMotion;
	double combinedMotion = 0; 
	double motionMetric = 0; 	
	static double motionThreshold = 90;
	static double alpha = 0.80;
	double beta = 1 - alpha;
	struct timeval  timeReadOut;	
	long double timerValueSeconds = 0;
	static long double blinkHalfCycleSeconds = 0.5 ;
	long double nextActionTime = 0;

	// initialize environment for listening to Mouse
	Display *display;
	XEvent xevent;
	Window window;
	if( (display = XOpenDisplay(NULL)) == NULL )
		return -1;
	window = DefaultRootWindow(display);
	XAllowEvents(display, AsyncBoth, CurrentTime);
	XGrabPointer(display, window, 1, PointerMotionMask | ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
	
	// initialize for time-based actions
	gettimeofday(&timeReadOut, NULL);
	timerValueSeconds = (long double)timeReadOut.tv_sec + (long double)timeReadOut.tv_usec/1000000.0;
	printf("Current timer value is %Lf seconds\n", timerValueSeconds);
	nextActionTime = timerValueSeconds + blinkHalfCycleSeconds;

	system("clear");

	while(true)
	{
		//+++++ section 2. Update motion metric ++++++++++++++++++
	  
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
			combinedMotion = sqrt(pow((double)horizontalMotion,2.0) + pow((double)verticalMotion,2.0));
			//printf("dXY = %f \n", combinedMotion); //for debug
			motionMetric = alpha * combinedMotion + beta * motionMetric ;
			//printf("motionMetric = %f\n", motionMetric); //for debug
			if (motionMetric > motionThreshold)
				printf("\nLarge motion detected. Motion Metric = %f\n", motionMetric);
				}

		//+++++ section 3. Time-based actions +++++++

		gettimeofday(&timeReadOut, NULL);
		timerValueSeconds = (long double)timeReadOut.tv_sec + (long double)timeReadOut.tv_usec/1000000.0;
		//printf("Current timer value is %Lf seconds\n", timerValueSeconds); //for debug		
		if (timerValueSeconds > nextActionTime)
		{	
			nextActionTime = timerValueSeconds + blinkHalfCycleSeconds;
			fflush(stdout);
			//printf("\r");
			if(blinkOn)
				printf("X");
			else
				printf(".");
			blinkOn = !blinkOn;
			fflush(stdout);	
		}	  
	}
	return 0;
}
