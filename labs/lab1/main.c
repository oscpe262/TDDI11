#include <libepc.h>

int main(int argc, char *argv[])
{
   ClearScreen(0x07);
   SetCursorPosition(0, 0);
   
   PutString("12/4 -16 Oscar Petersson, Matteus Laurent\r\noscpe262, matla782\r\n");
   
   return 0;
}
