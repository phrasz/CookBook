#include <stdio.h>
int main(){
	int* array;
	array = malloc(sizeof(int) * 50000);
	if (array == NULL){
		fprintf(stderr,"Could not allocate that much memory");
      		return 1;
	}
	 
	/* do something with array */
	free(array);

}
