#include <stdio.h>

#define PE_Multiply_2Nums(x,y) printf("\n\tMultiplying %i by %i: %i", x,y,x*y);printf("\n");
#define PE_Print_Variable(x) printf("\n\n\nThis is the Variable Fadespeed: %i", x);
void main(){
	int fadespeed = 77;
	printf("\nAttempting to multiply X * Y:");
	PE_Multiply_2Nums(2,3);
	PE_Multiply_2Nums(4,3);
	PE_Multiply_2Nums(7,3);
	PE_Multiply_2Nums(2,23);
	
	PE_Print_Variable(fadespeed);
}