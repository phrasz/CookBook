#include <stdio.h>

void main(){
	int i;
	
	for (i=0; i<10; i++){
		switch(i){
			case 1:
				printf("\n One is the loneliest number");
				break;
			case 2:
				printf("\n Second in the Best");
				break;
			case 6:
				printf("\n 6 ... because");
				break;
			case 9:
				printf("\n woot 3x3 = 9");
				break;
			default:
				printf("\n In the loop Default");
		}
	
	}
}