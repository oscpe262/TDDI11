#include <stdio.h>

#define PI 3.141592 //No '='!
#define pow(x) ((x)*(x))

int mypow(int x) {
  return x*x;
}

void main() {
#if 0 //comment out like /* */, but allows for nesting comments ...
#ifdef PI
  printf("Pi equals %f\n", PI);
  printf("Pi squared equals %f\n", poW(PI));
  printf("Pi squared is  %f\n", mypow(PI));
#endif
#endif
  printf("Current date is %s\n", __DATE__); 
  printf("Current line is %s\n", __LINE__);
  printf("Current file is %s\n", __FILE__);
 
}
