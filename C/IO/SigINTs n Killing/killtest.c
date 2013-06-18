/*
 * File: killtest.c
 * ----------------
 * Demonstrate the use of kill and signal.
 *
 * Oliver Fringer
 * COS315: Operating Systems
 * 9 May 03
 *
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define WAITFORRETURN getchar()

typedef enum {
  false, true 
} bool;

void sigproc();
void sighup();
void sigquit();
void sigkill();
void inquire(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  int pid;

  inquire(argc,argv);

  if((pid = fork())<0) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(!pid) {
    printf("Child: %d\n",getpid());

    signal(SIGHUP,sighup);
    signal(SIGINT,sigproc);
    signal(SIGQUIT,sigquit);
    signal(SIGKILL,sigkill);

    while(true);
  } else {
    printf("Parent: %d\n",getpid());

    if(argc==1) {
      WAITFORRETURN;

      printf("What happens when we send a SIGINT to the child (%d)? (hit return): ",pid);
      WAITFORRETURN;
      kill(pid,SIGINT);
      
      WAITFORRETURN;
      printf("Now what happens when we send a SIGHUP to the child? (hit return): ");
      WAITFORRETURN;
      kill(pid,SIGHUP);
      
      WAITFORRETURN;
      printf("Now what happens when we send a SIGQUIT to the child? (hit return): ");
      WAITFORRETURN;
      kill(pid,SIGQUIT);
      
      WAITFORRETURN;
      printf("Now if we raise the parent with SIGINT, we get (hit return): ");
      WAITFORRETURN;
      raise(SIGINT);
    } else {
      while(true);
    }
  }
}

/*
 * Function: sigproc
 * Usage: kill(SIGINT,sigproc);
 * ----------------------------
 * Function that executes upon receiving a SIGINT.
 *
 */ 
void sigproc() {
  signal(SIGINT,sigproc);

  printf("\nProcess %d: Ctrl-C is SIGINT.\n",getpid());
  printf("Press Ctrl-\\, or SIGQUIT, to exit.\n");
}

/*
 * Function: sighup
 * Usage: kill(SIGHUP,sighup);
 * ---------------------------
 * Function that executes upon receiving a SIGHUP.
 *
 */
void sighup() {
  signal(SIGINT,sigproc);

  printf("\nProcess %d: Received a SIGHUP and ignoring...\n",getpid());
}

/*
 * Function: sigquit
 * Usage: kill(SIGQUIT,sigquit);
 * ----------------------------
 * Function that executes upon receiving a SIGHUP.
 *
 */
void sigquit() {
  signal(SIGQUIT,sigquit);

  printf("\nProcess %d: Quitting with Ctrl-\\\n",getpid());
  exit(EXIT_SUCCESS);
}

/*
 * Function: sigkill
 * Usage: kill(SIGKILL,sigkill);
 * -----------------------------
 * Function that executes upon receiving a SIGKILL.
 * The point of this is to show that the KILL signal cannout be
 * caught.
 *
 */
void sigkill() {
  signal(SIGKILL,sigkill);

  printf("\nProcess %d: Killing with SIGKILL!",getpid());
  exit(EXIT_SUCCESS);
}

/*
 * Function: inquire
 * Usage: inquire(argc,argv);
 * --------------------------
 * Parse the command line arguments.
 *
 */
void inquire(int argc, char *argv[]) {
  switch(argc) {
  case 1: 
    break;
  case 2:
    if(strcmp(argv[1],"-s")) {
      printf("Usage: %s [-s]\n",argv[0]);
      exit(EXIT_FAILURE);
    }
    break;
  default:
    printf("Usage: %s [-s]\n",argv[0]);
    exit(EXIT_FAILURE);
  }
}
