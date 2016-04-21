#include <libepc.h>

int main(int argc, char *argv[])
{
	//+++++ Section 1. Initialization +++++

	ClearScreen(0x07);
	SetCursorPosition(1, 1);
	DWORD32 stopTime = 0;
	QWORD64 initialCount = 0;
	QWORD64 finalCount = 0;
	QWORD64 x = 0;
	DWORD32 xM = 0;

	//+++++ Section 2. +++++

	stopTime = Now_Plus(1);
	initialCount = CPU_Clock_Cycles();

	while( Now_Plus(0) < stopTime ){}

	finalCount = CPU_Clock_Cycles();
	x = finalCount - initialCount;
	xM = (DWORD32)(x / 1000000);
	
	PutString("x = ");
	PutUnsigned(xM, 10, 8);
	PutString(" x 10^6 \n");

	//+++++ Section 3. +++++

	stopTime = Milliseconds() + 1000;
	initialCount = CPU_Clock_Cycles();

	while( Milliseconds() < stopTime ){}

	finalCount = CPU_Clock_Cycles();
	x = finalCount - initialCount;
	xM= (DWORD32)(x / 1000000);
	
	SetCursorPosition(2, 1);
	PutString("x = ");
	PutUnsigned(xM, 10, 8);
	PutString(" x 10^6 \n");

	//+++++ Section 4. +++++

	stopTime = Milliseconds();
	SetCursorPosition(3, 1);
	PutUnsigned(stopTime, 10, 8);
	PutString("\n");
	stopTime = Milliseconds() + 2345;
	while( Milliseconds() < stopTime ){}
	SetCursorPosition(4, 1);
	stopTime = Milliseconds();
	PutUnsigned(stopTime, 10, 8);
	PutString("\n");	

	return 0;
}
