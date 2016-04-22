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
  int clock_gran = 1000 * 1000; //Keep in the thousands.
  int ccc_display_digits = 15;
  int i = 0;
  int dummy = 0;
  int test_calc = 0;

  for (i = clock_gran ; i % 1000 != 0 ; i / 1000) {
    ccc_display_digits -= 3;
    if (ccc_display_digits < 5)
      break;
  }

  //+++ Let's do stuff +++
  
  printrow("Clock Cycle Counts given in units of 10^", 15-ccc_display_digits, &row, 1);

  //startIntnp = Now_Plus(0);
  startTime = Milliseconds();
  initialCount = CPU_Clock_Cycles();

  printrow("Start MS: ", startTime, &row, 8);
  printrow("CPU Clock Cycles in millions: ", initialCount/clock_gran, &row, ccc_display_digits);

  for (i = 0 ; i < many_iterations ; ++i) {
    dummy = dummy + 1; // dummy
  }
  
  stopIntnp = Now_Plus(0);
  stopTime = Milliseconds();
  initialCount = CPU_Clock_Cycles();
  
  printrow("Stop MS: ", stopTime, &row, 8);
  printrow("Stop Now Plus: ", stopIntnp, &row, 8);
  printrow("Relative: ", stopTime - startTime, &row, 8);

  ++row;

  initialCount = CPU_Clock_Cycles();
  
  SetCursorPosition(++row, 1);
  PutString("CPU Clock Cycles: ");
  PutUnsigned(initialCount/clock_gran, 10, 10);
  PutString("\n");

  ++row;

  //+++ Let's compare +++

  startTime2 = Milliseconds();
  initialCount = CPU_Clock_Cycles();
  
  printrow("Start 2: ", startTime2, &row, 8);
  
  for (i = 0 ; i < many_iterations ; ++i) {
    dummy = dummy + 1; // dummy
    test_calc += 1;
  }
  
  stopTime2 = Milliseconds();
  initialCount = CPU_Clock_Cycles();
  
  printrow("Stop 2: ", stopTime2, &row, 8);
  printrow("Relative: ", stopTime2 - startTime2, &row, 8);
  printrow("testcalc += 1; 's in a millisecond: ",
	   many_iterations/( (stopTime2 - startTime2) - (stopTime - startTime) ),
	   &row,
	   8);

  finalCount = CPU_Clock_Cycles();

  ++row;

  SetCursorPosition(++row, 1);
  PutString("CPU Clock Cycles: ");
  PutUnsigned(finalCount/clock_gran, 10, 10);
  PutString("\n");

  return 0;
}
