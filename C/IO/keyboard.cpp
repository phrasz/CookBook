#include <iostream>  // C++
#include <conio.h>
using namespace std; // C++

int main() /* program main begins execution */
{
	int c= 0;
	while( c != 27)
	{
		c = getche();
		if(c == 0 || c == 224)
			c = -getche();
		cout << c << endl;
		
	}

   return 0;  /* indicates successful program execution */
}