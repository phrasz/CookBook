#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define ESCAPE 27

int main(void){
int loop = 0;
int number = 0;
int c;
	while(!loop){
		//printf("\tIM IN THE LOOPY\n");
		printf("[MESSAGE] Number is: %i\n",number);
		number++;		
		if (kbhit()){
			c = getch();
				if ( c == ESCAPE ){
					printf("YOU HIT ESCAPE...NOW QUITTING");
					loop = 1;
				}
		}
		system("cls");
	}
}
