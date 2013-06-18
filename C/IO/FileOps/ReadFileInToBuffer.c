/* This reads the input file into a buffer.  The scanner will then read from */
/* this buffer, and the buffer will be preserved for eventual output of the  */
/* unmodified parts.                                                         */

/* Note that the parser requires the buffer to have two empty bytes after    */
/* the data, which it will fill with its YY_END_OF_BUFFER_CHAR.  This is why */
/* buffers are allocated as (nbytes+2).                                      */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define BLKSIZE 10240

//char *ReadFile (char *name, int *bytes){
//  return (buffer);
//}

void main(){

//	ReadFile("sixtith.jpg", 1000);
  FILE *in;
  struct stat fs;
  size_t nbytes, nread;
  char *buffer, *name;
  int *bytes;
  *bytes = -1;
  buffer = NULL;

  stat (name, &fs);         /* Get size of file */
  nbytes = fs.st_size;

  in = fopen (name, "r");

  if (in == NULL)
  {
    fprintf (stderr, "ModelAssembler: Can't open input file '%s'.\n", name);
  }
  else
  {
    buffer = (char *) malloc (nbytes + 2);

    nread = fread (buffer, 1, nbytes, in);
    fclose (in);

    if (nbytes > 0)
      buffer [*bytes] = '\0';      /* make sure it's NULL-terminated */

    *bytes = (int) nbytes;         /* and return length in argument, converting */
  }                                /* in case size_t != int */



}
