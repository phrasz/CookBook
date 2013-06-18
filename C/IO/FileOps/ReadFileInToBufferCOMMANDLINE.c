/* Standard include files */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/stat.h>

/* Command line argument variables */

char *pcFilename = NULL;
double dArg = 0.0;
int iArg = 0;
int iFlag = 0;


/***************************************/
/* Print usage statement               */
/***************************************/

void Usage(void)
{
  /* Print message with program arguments */
  fprintf(stderr, "usage: parseargs [-file <filename>]\n");
}


/***************************************/
/* Parse command line arguments        */
/* Input is argc and argv from main    */
/* Fill in global variables            */
/* Return 1 for success, 0 for failure */
/***************************************/

int ParseArguments(int argc, char **argv) 
{
  /* Skip over program name */
  argc--; argv++;

  /* Loop through command line words */
  while (argc > 0) {
    if (!strcmp(*argv, "-file")) { argv++; argc--; pcFilename = *argv; }
    //else if (!strcmp(*argv, "-int")) { argv++; argc--; iArg = atoi(*argv); }
    //else if (!strcmp(*argv, "-double")) { argv++; argc--; dArg = atof(*argv); }
    //else if (!strcmp(*argv, "-flag")) { iFlag = 1; }
    else {
      fprintf(stderr, "Unrecognized recognized command line argument: %s\n", *argv);
      Usage();
      return 0;
    }
    argv++; argc--;
  }
  
  /* Return success */
  return 1;
}


/******************************************/
/* Main program                           */
/* Parse arguments into global variables  */
/* Print results                          */
/******************************************/

int main(int argc, char **argv)
{
  /* Parse arguments */
//  if (!ParseArguments(argc, argv)) return -1;

  /* Print results */
//  if (pcFilename) printf("%s ", pcFilename);
//  else printf("<stdin> ");
//  printf("%g %d %d\n", dArg, iArg, iFlag);


/// //////////////////////////////////////////////
/// FILE I/O VARS
//      ReadFile("sixtith.jpg", 1000);
  FILE *in;
  char *buffer;
  buffer = NULL;
/// //////////////////////////////////////////////


/// //////////////////////////////////////////////
/// Open File ---> Read into buffer

   /* to retrieve data from file */
   if ((in = fopen("myfile.txt", "r"))== NULL) {
       fprintf(stderr, "Can't open file for read : Program terminates!\n");
       exit(1);
   }

   /* read data into buf from file */
   if (fread(buffer, sizeof(buffer), 1, in) != 1)  {
       if (ferror(in)) {  /* if error, not EOF */  
           printf("\nNot able to read file : Program terminates!\n");
           exit(1);
       }
   }
   fclose(in);
/// END Open File ---> Read into buffer
/// //////////////////////////////////////////////

 printf("\n[DEBUGGING] File Read in: %s",pcFilename);
 printf("\n[DEBUGGING] File Contents(length: %i): %s",strlen(buffer), buffer);

  /* Return success */
  return 0;
}













