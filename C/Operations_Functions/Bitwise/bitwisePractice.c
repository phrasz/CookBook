/* Bitwise operation practice */
#include <stdio.h>

int main(void) {
  
  unsigned int x, y, ans;
  
  // Read a value and set it to variable x.
  // Set y to be the value of x by setting the higher 16 bits to 0
  // and preserving the lower 16 bits.
  printf("Enter an integer value of x: ");
  scanf("%d", &x);
  y = x & 0x0000FFFF;
  printf("Setting the higher 16 bits to 0 and preserving the lower 16 bits:\n");
  printf("x=%08X, y=%08X\n\n", x, y);
  
  // Read a value and set it to variable x.
  // Set y to be the value of x by setting the odd bits to 1 and preserving the even bits.
  printf("Enter an integer value of x: ");
  scanf("%d", &x);
  y = x | 0x55555555;
  printf("Setting the higher 16 bits to 0 and preserving the lower 16 bits:\n");
  printf("x=%08X, y=%08X\n\n", x, y);
  
  // Read a value and set it to variable x.
  // Set variable y to x/512+149*16777216. (1677721610=100000016=2^24.)
  printf("Enter an integer value of x: ");
  scanf("%d", &x);
  y = x >> 9 | 149 << 24;
  printf("Set variable y to x/512+149*16777216:\n");
  printf("x=%08X, y=%08X\n\n", x, y);
  
  // Read two values and set them to variables x and y. 
  // Set variable ans to x*128+y%128 using bitwise operations.
  printf("Enter two integer value of x and y: ");
  scanf("%d %d", &x, &y);
  ans = (x << 7) | (y & 127);
  printf("Set variable y to x*128+y%%128:\n");
  printf("x=%08X, y=%08X, ans=%08X\n\n", x, y, ans);
  
  system("pause");
  return 0; 
}
