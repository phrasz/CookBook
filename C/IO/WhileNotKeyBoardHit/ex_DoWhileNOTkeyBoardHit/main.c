#include <stdio.h>
#include <conio.h>

#define ESCAPE 27

int main(void)
{

	int loop = 0;
	int c;

	while(!loop)
	{
		printf("\tIM IN THE LOOPY\n");

		if ( kbhit())
		{
			c = getch();

			if ( c == ESCAPE )
			{
				printf("YOU HIT ESCAPE...NOW QUITTING");
				loop = 1;
			}
		}
	}
}
