#include <stdio.h>
#include <stdlib.h>

#define HelloWorld "\nHello World!"


#define InstallInt(VariableName)		\
		int VariableName = 199;		\
		int Width_##VariableName = 200;	\
		int VariableName##_Width = 9001

#define	EchoInt(VariableName)									\
		printf("\nThis is "#VariableName"'s value: %i",VariableName);			\
		printf("\nThis is Width_"#VariableName"'s value: %i",Width_##VariableName);	\
		printf("\nThis is "#VariableName"_Width's value: %i",VariableName##_Width)

int main(){
	//Clear screen
	#ifdef __linux__
		system("clear");
	#elif _WIN32
		system("cls");
	#endif

	//First Macro
	printf(HelloWorld);

	//Example execution
	int Temp_x = 0;
	printf("\nThis is Temp_x: %i",Temp_x);

	//Second Macro
	InstallInt(Temp);
	EchoInt(Temp);
	printf("\n");
	return 0;
}
