#include <stdio.h>
#define nrows 10
#define ncolumns 10
 
int main(){
	int **array;
	int i;
	array = malloc( nrows * sizeof(int *));
	printf("\n[DEBUGGING] Allocation of pointer array with: %i elements.",nrows);
	if(array == NULL){
		printf("\n[ERROR] OUT OF MEMORY!!!!");
		return 0;
	}

	printf("\n[DEBUGGING] Allocation of array elements having each the size of: %i.",ncolumns);
	for(i=0;i<nrows;i++){
		array[i] = malloc(ncolumns * sizeof(int));
		if(array[i] == NULL){
			printf("\n[ERROR] OUT OF MEMORY!!!!");
			return 0;
		}
	}

	// Clean up
	printf("\n[DEBUGGING] Freeing Allocation of Array");
	for(i=0;i<nrows;i++){
		free(array[i]);
	}
	free(array);
	printf("\n");
}
