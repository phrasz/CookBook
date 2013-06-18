/* fwrite example : write buffer */
#include <stdio.h>

int main ()
{
  FILE * pFile;
  char buffer[] = { 'x' , 'y' , 'z' };
  pFile = fopen ( "myfile.bin" , "wb" );
  fwrite (buffer , 1 , sizeof(buffer) , pFile );
  fclose (pFile);
  return 0;
}
