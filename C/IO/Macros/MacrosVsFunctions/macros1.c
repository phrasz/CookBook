#include <stdio.h>
#ifdef _WIN32
  #include <sys\timeb.h>    // Needed for ftime() and timeb structure
#endif
#ifdef __linux__
  #include <sys/timeb.h>    // Needed for ftime() and timeb structure
#endif

#define Multiply_2Nums(x,y) printf("\n\tMultiplying %i by %i: %i", x,y,x*y);printf("\n");
#define Print_Variable(x) printf("\n\n\nThis is the Variable Fadespeed: %i", x);
void main(){
	//Elapsed Time:
	struct timeb         start, stop;        // Start and stop times structures
	double               elapsed;            // Elapsed time in seconds
	
	int fadespeed = 77;
	
	// Start timing
	ftime(&start);
  
	printf("\nAttempting to multiply X * Y:");
	Multiply_2Nums(2,3);
	Multiply_2Nums(4,3);
	Multiply_2Nums(7,3);
	Multiply_2Nums(2,23);
	
	Print_Variable(fadespeed);
		
	// Stop timing and compute elapsed time
	ftime(&stop);
	elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) - ((double) start.time + ((double) start.millitm * 0.001));
	printf("\nTime Elapsed: %f", elapsed);
}