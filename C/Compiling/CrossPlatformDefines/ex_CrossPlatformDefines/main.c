#ifdef TARGET_OS_MAC
  // Mac Includes Here
  #error Can't be compiled on Mac yet
#endif

#ifdef __linux__
  // Linux Includes Here
  //#error Can't be compiled on Linux yet
#endif

#ifdef _WIN32 || _WIN64
  // Windows Includes Here
  //#error Can't be compiled on Windows yet
#endif

//#include <iostream>
#include <stdio.h>
//using namespace std;

int probe() {
    #ifdef TARGET_OS_MAC
      return 0;
    #elif defined __linux__
      return 1;
    #elif defined _WIN32 || defined _WIN64
      return 2;
    #else
    #error "unknown platform"
    #endif
}


int main(){
    printf("\nI say %i\n\n",probe());
    return 1;
}

