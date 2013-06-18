#include<sys/stat.h>
#include <stdio.h>

void main(){
	printf("\n===================================\n\tSuper Awesome File Checker...");

//int file_exists (char * fileName)
//{
   struct stat buf;
   int i = stat ( "file.txt", &buf );
     /* File found */
     if ( i == 0 ){
       //return 1;
     	printf("\nFile exists!");
     }
     else{
     	printf("\nFile does NOT exist!");
     }
     //return 0;
//}
	printf("\n===================================\n");
}
