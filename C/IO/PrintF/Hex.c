/* strlen example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char szInput[256];
  printf ("Enter a sentence: ");
  gets (szInput);
  printf ("The sentence entered is %u characters long.\n",(unsigned)strlen(szInput));
  printf ("The sentence in Hex: ");
  int i=0;

  for(i=0;i<(unsigned)strlen(szInput);i++){
	printf("%x ",szInput[i]);
  }
  return 0;
}
