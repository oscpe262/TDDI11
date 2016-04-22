#include <libepc.h>

void printrow(char *thing, DWORD32 value, int *row, int digits)
{
  SetCursorPosition(++(*row), 1);
  PutString(thing);
  PutUnsigned(value, 10, digits);
  PutString("\n");
}

int main(int argc, char *argv[])
{

  //+++ Initialization +++

  ClearScreen(0x07);
  SetCursorPosition(1, 1);
  DWORD32 startIntnp = 0;
  DWORD32 stopIntnp = 0;
  DWORD32 startTime = 0;
  DWORD32 stopTime = 0;
  DWORD32 startTime2 = 0;
  DWORD32 stopTime2 = 0;
  QWORD64 initialCount = 0;
  QWORD64 finalCount = 0;
  int row = 0;
  
  int many_iterations = 100 * 1000 * 1000;
  int clock_gran = 1000 * 1000;
  int ccc_display_digits = 8;
  int i = 0;
  int dummy = 0;
  int test_calc = 0;

  printrow("Clock Cycle Counts given in units of 10^", 6, &row, 1);
  ++row;

  //+++ Let's do stuff +++
  
  startTime = Milliseconds();
  initialCount = CPU_Clock_Cycles();

  printrow("Start MS: ", startTime, &row, 8);
  printrow("Start CPU Clock Cycles: ", initialCount/clock_gran, &row, ccc_display_digits);

  ++row;

  //+++ Loop 1 +++

  SetCursorPosition(++row,1);
  PutString("*** Running LOOP 1 of dummy instructions ***");
  ++row;

  startTime = Milliseconds();
  initialCount = CPU_Clock_Cycles();

  for (i = 0 ; i < many_iterations ; ++i) {
    dummy = dummy + 1; // dummy
  }
  
  stopTime = Milliseconds();
  finalCount = CPU_Clock_Cycles();

  printrow("Milliseconds taken: ", stopTime - startTime, &row, 8);
  printrow("Relative CCCs: ", (finalCount - initialCount)/clock_gran, &row, ccc_display_digits);
  
  ++row;

  //+++ Loop 2 +++

  SetCursorPosition(++row,1);
  PutString("*** Running LOOP 2 of dummy + tested instructions ***");
  ++row;

  startTime2 = Milliseconds();
  initialCount = CPU_Clock_Cycles();  
  
  for (i = 0 ; i < many_iterations ; ++i) {
    dummy = dummy + 1; // dummy
    test_calc += 1; // instruction to test
  }
  
  stopTime2 = Milliseconds();
  finalCount = CPU_Clock_Cycles();
  
  printrow("Milliseconds taken: ", stopTime2 - startTime2, &row, 8);
  printrow("Relative CCCs: ", (finalCount - initialCount)/clock_gran, &row, ccc_display_digits);
  ++row;
  printrow("test_calc += 1; 's in a millisecond: ",
	   many_iterations/( (stopTime2 - startTime2) - (stopTime - startTime) ),
	   &row,
	   1);

  return 0;
}
