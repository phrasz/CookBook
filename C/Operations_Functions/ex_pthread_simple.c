/*****************************************************************************
* FILE: hello_arg3.c
* DESCRIPTION:
*   This "hello world" Pthreads program demonstrates an unsafe (incorrect) 
*   way to pass thread arguments at thread creation.  In this case, the 
*   argument variable is changed by the main thread as it creates new threads.
*
* LAST REVISED: 09/04/02 Blaise Barney
******************************************************************************/

//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>         // Needed for exit()
#include <string.h>         // Needed for memcpy() and strcpy()
#include <fcntl.h>          // Needed for file i/o stuff
#ifdef _WIN32 || _WIN64
  #include <process.h>      // Needed for _beginthread() and _endthread()
  #include <stddef.h>       // Needed for _threadid
  #include <windows.h>      // Needed for all Winsock stuff
  #include <sys\stat.h>     // Needed for file i/o constants
  #include <io.h>           // Needed for file i/o stuff
#endif
#ifdef __linux__
  #include <pthread.h>      // Needed for pthread_create() and pthread_exit()
  #include <sys/stat.h>     // Needed for file i/o constants
  #include <sys/types.h>    // Needed for sockets stuff
  #include <netinet/in.h>   // Needed for sockets stuff
  #include <sys/socket.h>   // Needed for sockets stuff
  #include <arpa/inet.h>    // Needed for sockets stuff
  #include <fcntl.h>        // Needed for sockets stuff
  #include <netdb.h>        // Needed for sockets stuff
#endif


/// OLD!!!!
#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS	8
/// OLD!!!!


char *messages[NUM_THREADS];




// //----- Function prototypes -------------------------------------------------
// #ifdef _WIN32 || _WIN64
// void handle_get(void *in_arg);      // Windows thread function to handle GET
// #endif
// #ifdef __linux__
// void *handle_get(void *in_arg);     // POSIX thread function to handle GET
// #endif


#ifdef _WIN32 || _WIN64
void PrintHello(void *threadid)
#endif
#ifdef __linux__
void *PrintHello(void *threadid)
#endif
{
   int *id_ptr, taskid;

   sleep(3);
   id_ptr = (int *) threadid;
   taskid = *id_ptr;
   printf("Thread %d: %s \n", taskid, messages[taskid]);
   
	#ifdef _WIN32 || _WIN64
	_endthread();
	#endif
	#ifdef __linux__
	pthread_exit(NULL);
	#endif
}

int main(int argc, char *argv[])
{
	#ifdef __linux__
	pthread_t            threads[NUM_THREADS];            // Array of Thread ID
	#endif
	int rc, t;

	messages[0] = "English: Hello World!";
	messages[1] = "French: Bonjour, le monde!";
	messages[2] = "Spanish: Hola al mundo";
	messages[3] = "Klingon: Nuq neH!";
	messages[4] = "German: Guten Tag, Welt!"; 
	messages[5] = "Russian: Zdravstvytye, mir!";
	messages[6] = "Japan: Sekai e konnichiwa!";
	messages[7] = "Latin: Orbis, te saluto!";

	for(t=0;t<NUM_THREADS;t++){
		printf("Creating thread %d\n", t);

		#ifdef _WIN32 || _WIN64
		if (_beginthread(handle_get, 4096, (void *)client_s) < 0)
		#endif
		#ifdef __linux__
		if (pthread_create(&thread_id, NULL, handle_get, (void *)client_s) != 0)
		#endif
		{
			printf("ERROR - Unable to create a thread to handle the GET \n");
			exit(1);
		}
		/// OLD!!!!
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *) &t);
		if (rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			pexit(-1);
		}
		/// OLD!!!!
   }

   pthread_exit(NULL);
}