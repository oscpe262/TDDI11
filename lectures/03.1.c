#include <stdio.h>

struct card {
  char *face;
  char *suit;
};

typedef union Number {
  int i;
  float f;
} NumberUnion;

int main() {
  struct card threeHearts;
  threeHearts.face = "Three";
  threeHearts.suit = "Hearts";
  printf("Face is %s \n", threeHearts.face);

  struct card aceOfSpades = {"Ace", "Spades"};
  printf("Face is %s \n", aceOfSpades.face);

  struct card *myptr = &threeHearts;
  printf("Face is %s \n", myptr->face);

  NumberUnion n;
  n.i = 3;
  printf("Number is %d\n", n.i); //3
  printf("Number is %d\n", *((int*)(&n))); //3
  n.f = 6.3;
  printf("Number is %f\n", n.f); //6.3
  printf("Number is %d\n", *((int*)(&n))); // [!]
}
#if 0
bits |= (1<<7);  //   sets bit 7
bits &= ~(1<<7); // clears bit 7
bits ^= (1<<7);  //  flips bit 7
#endif
