#include <stdio.h>
#include <stdlib.h> //Malloc and Free

//Constructors
void Set_Array_Default_Num(int **array, int nrows, int ncolumns);

void main(){
	int **array;
	int nrows=5;
	int ncolumns=5;
	int i, j;
	
	//Create 2D array of Int's
	array = malloc(nrows * sizeof(int *));
	if(array == NULL){	// Create Array of Int Pointers
		printf("\n[ERROR] out of memory");
		exit;
	}
	for(i = 0; i < nrows; i++){	//Create Sub Array of Integers having ncolumn elements
		array[i] = malloc(ncolumns * sizeof(int)); //Could replace with Char to make array of char; or Structs, and have Array of Structs...
	
		if(array[i] == NULL){
			printf("\n[ERROR] out of memory\n");
			exit;
		}
	}
	
	//Print Array:
	printf("\n[DEBUGGING] Created Array with \"Uninitialized\" Values:");
	for(i = 0; i < nrows; i++){
		for(j = 0; j < ncolumns; j++){
			printf("\n[DEBUGGING] Array[%i][%i]: %i",i,j,array[i][j]);
		}
	}
	
	//Zero it:
	Set_Array_Default_Num(array,nrows,ncolumns);
	
	//Print Array:
	printf("\n\n[DEBUGGING] Updated Array with \"Default Number\" Values:");
	for(i = 0; i < nrows; i++){
		for(j = 0; j < ncolumns; j++){
			printf("\n[DEBUGGING] Array[%i][%i]: %i",i,j,array[i][j]);
		}
	}	
	
	//Clean up:
	for(i = 0; i < nrows; i++){
		free(array[i]);
	}
	free(array);
}
		
void Set_Array_Default_Num(int **array, int nrows, int ncolumns){
	int i, j;
	for(i = 0; i < nrows; i++)
		{
		for(j = 0; j < ncolumns; j++)
			array[i][j] = -1;
		}
}