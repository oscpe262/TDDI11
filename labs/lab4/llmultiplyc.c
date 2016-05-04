#include <libepc.h>

void llmultiply(unsigned long long int a,
		unsigned long long int b,
		unsigned char *result)
{
  unsigned long int aHi = (a >> 32);
  unsigned long int aLo = (a & 0x00000000FFFFFFFF); //32 lowest bits
  unsigned long int bHi = (b >> 32);
  unsigned long int bLo = (b & 0x00000000FFFFFFFF); //32 lowest bits

  unsigned long long int aHi_bHi = (unsigned long long int)aHi * bHi;
  unsigned long long int aHi_bLo = (unsigned long long int)aHi * bLo;
  unsigned long long int aLo_bHi = (unsigned long long int)aLo * bHi;
  unsigned long long int aLo_bLo = (unsigned long long int)aLo * bLo;

  *(unsigned long long int *)result = 0;
  *(unsigned long long int *)(result + 8) = 0;

  *(unsigned long long int *)result += aLo_bLo;
  *(unsigned long long int *)(result + 4) += aLo_bHi;
  *(unsigned long long int *)(result + 8) += aHi_bHi;

  //Caution to not miss any 64-bit carries

  *(unsigned long long int *)(result + 4) += (aHi_bLo & 0xFFFFFFFF); //aHi_bLo - low
  *(unsigned long long int *)(result + 8) += (aHi_bLo >> 32); //aHi_bLo - high
  	      
}
