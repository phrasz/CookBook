//http://en.wikipedia.org/wiki/C_date_and_time_functions

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
 
 #define PE1_ENGINE_GET_LOCAL_TIME()											\
		current_time = time(NULL);												\
		strftime (PE1_ENGINE_LocalTime, 50,"%I:%M:%S",localtime(&current_time));	\
		printf("\n%s", PE1_ENGINE_LocalTime);
 
int main(void){
    time_t current_time;
    char PE1_ENGINE_LocalTime[20];

    /* Obtain current time as seconds elapsed since the Epoch. */
    
 
    //if (current_time == ((time_t)-1)){
    //    (void) fprintf(stderr, "Failure to compute the current time.");
    //    return EXIT_FAILURE;
    //}
 
    /* Convert to local time format. */
    //c_time_string = ctime(&current_time);
 
    //if (c_time_string == NULL)
    //{
    //    (void) fprintf(stderr, "Failure to convert the current time.");
    //    return EXIT_FAILURE;
    //}

	
	PE1_ENGINE_GET_LOCAL_TIME();
	
    return EXIT_SUCCESS;
}