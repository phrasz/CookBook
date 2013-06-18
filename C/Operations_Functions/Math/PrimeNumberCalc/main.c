/* By Bryce Schroeder           *
 * gcc -lpthread -lm main.c    */
#include <stdio.h>   /* For printf, scanf, puts      */
#include <pthread.h> /* For various thread functions */
#include <math.h>    /* For sqrt                     */
#include <stdlib.h>  /* For calloc                   */
#include <assert.h> 
/* This program implements this exercise: 
 4.9 Write a multithreaded Java, Pthreads, or Win32 program that outputs prime
 numbers. This program should work as follows: The user will run the program
 and will enter a number on the command line. The program will then create a
 separate thread that outputs all the prime numbers less than or equal to the
 number entered by the user.
*/
typedef struct {
    /* Store the thread's current status. */
    pthread_cond_t start_working;
    /* This mutex protects the condition variable. */
    pthread_mutex_t cond_mutex;
    /* To what number is the thread searching? */
    long max_search;
} thread_parameters_t;

typedef struct {
    long f[2];
} factor_t;

/* This function prints prime factorizations. */
void print_factorization(long n, factor_t *sieve) {
    int i;
    if (sieve[n].f[0]) {
        for (i = 0; i < 2; ++i)
            if (sieve[n].f[i])
                print_factorization(sieve[n].f[i], sieve);
    } else
                printf(" %ld ", n);
}

/* This function runs the prime computer */
void *primes_computer_runner(void *param) {
    factor_t *sieve;
    long i, prime, limit;
    thread_parameters_t *thread_parameters = (thread_parameters_t*)param;
    pthread_mutex_lock(&thread_parameters->cond_mutex);
    pthread_cond_wait(&thread_parameters->start_working, 
                      &thread_parameters->cond_mutex);
    /* Now our thread sleeps until the condition is true.       *
     * When it reaches the next line, it has been signaled from *
     * the main thread.                                         */
    printf("Thread woken up to find primes less than %ld.\n", 
	   thread_parameters->max_search);
    /* We use the Sieve of Eratosthenes here. */
    sieve = (factor_t*)calloc(thread_parameters->max_search, sizeof(factor_t)); 
    assert(sieve != NULL);
    /* Because calloc helpfully initalizes our memory to 0, we will    *
     * use 0 to represent "is prime" in accordance with the algorithm. */
    limit = (long)sqrt(thread_parameters->max_search) + 1;
    /* the 'prime' here is really a prime-presumptive, of course. */
    for (prime = 2; prime <= limit; ++prime)
	for (i=2; i*prime < thread_parameters->max_search; ++i) {
	    sieve[i*prime].f[0] = i;
	    sieve[i*prime].f[1]= prime; 
	                     /* It is nonprime because it has factors *
	                      * i and prime...                        */
	}

    /* Display the results */
    for(i=2; i < thread_parameters->max_search; ++i)
       if (!sieve[i].f[0]) 
           printf("* %ld is prime.\n",i);
#ifdef SHOW_NONPRIME
       else {
           printf("  %ld is nonprime, factorization: (",i);
	   print_factorization(i, sieve);
	   puts(")");
       }
#endif

    free(sieve);

    return NULL;
}

int main (int argc, char *argv[]) {
    /* We pass a pointer to this struct to the computational thread. */
    thread_parameters_t thread_parameters;
    /* Set up the initial conditions of our thread */
    pthread_cond_init(&thread_parameters.start_working, NULL);
    pthread_mutex_init(&thread_parameters.cond_mutex, NULL);

    /* This thread will do the computing of prime numbers. */
    pthread_t computational_thread;

    /* Create and start our new thread. */
    pthread_create(&computational_thread, NULL, primes_computer_runner, 
                   (void*)&thread_parameters);

    puts("Bryce Schroeder's Prime Number Calculator Program with Threads\n"
         "Enter an integer and the program will print all primes less than\n"
	 "the integer you enter. (^D or ^C to quit.)\n"
#ifdef SHOW_NONPRIME
         "This program has been compiled to show prime factorizations of \n"
	 "the nonprimes it finds, with -DSHOW_NONPRIME in the Makefile.\n"
#endif
        );

    /* Get the integer from the user */
    if (!scanf("%ld", &thread_parameters.max_search)) return 0;
    /* Wake up the thread to do the computation. */
    pthread_cond_broadcast(&thread_parameters.start_working);
    /* Wait for it to finish (makes this rather pointless, but I forgot
     * about this assignment until the eleventh hour.) */
    pthread_join(computational_thread,NULL);

    return 0;
}

