 //----- Include files --------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>          // Exit()
#include <string.h>         // Needed for memcpy() and strcpy()
#ifdef _WIN32
#include <windows.h>      // Needed for all Winsock stuff
#endif
#ifdef __linux__
#include <sys/types.h>    // Needed for sockets stuff
#include <netinet/in.h>   // Needed for sockets stuff
#include <sys/socket.h>   // Needed for sockets stuff
#include <arpa/inet.h>    // Needed for sockets stuff
#include <fcntl.h>        // Needed for sockets stuff
#include <netdb.h>        // Needed for sockets stuff
#include <signal.h>       // SigINT killing
#endif

 #include "cv.h"
 #include "highgui.h"
 #include <stdio.h>

//Needed for Pthreads:
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//----- Globals ---------------------------------------------------------------
//File name parsing
char *name = NULL;
double dArg = 0.0;
int iArg = 0;
int iFlag = 0;

//Jpeg State For Cross Thread Communication
int jpegRecv=0;

//----- Defines --------------------------------------------------------------
#define  PORT_NUM   1050    // Arbitrary port number for the server
#define  MESSAGE_MTU        1326
#define  PACKET_MTU         1390
#define  SIGNATURE_MTU      48
#define  XOR_MTU            4
#define  CURRENTPACKET_MTU  4
#define  TOTALPACKET_MTU    4
#define  SSCANF_SPACES      4
#define  NON_MESSAGE_BYTES  64 // S.T. NMB= Signature+CurrMTU+TotMTU+SSCANF


#ifdef __linux__
//----- Signal Intterupt --------------------------------------------------------------
void signal_callback_handler(int signum){
   printf("\nCaught signal %d \nQuitting... GOODBYE!\n\n",signum);
   // Cleanup and close up stuff here

   // Terminate program
   exit(signum);
}
#endif

//===== Functions ==========================================================
///////////////////////////////////////////////
// Print usage statement
void Usage(void){
  /* Print message with program arguments */
  fprintf(stderr, "usage: parseargs [-file <filename>] [-double <arg>] [-int <arg>] [-flag]\n");
}
///////////////////////////////////////////////

///////////////////////////////////////////////
// Parse command line arguments
// Input is argc and argv from main
// Fill in global variables
// Return 1 for success, 0 for failure
//int ParseArguments(int argc, char **argv){
//  /* Skip over program name */
//  argc--; argv++;
//
//  /* Loop through command line words */
//  while (argc > 0) {
//    if (!strcmp(*argv, "-f")) { argv++; argc--; name = *argv; }
//    //else if (!strcmp(*argv, "-int")) { argv++; argc--; iArg = atoi(*argv); }
//    //else if (!strcmp(*argv, "-double")) { argv++; argc--; dArg = atof(*argv); }
//    //else if (!strcmp(*argv, "-flag")) { iFlag = 1; }
//    else {
//      fprintf(stderr, "Unrecognized recognized command line argument: %s\n", *argv);
//      Usage();
//      return 0;
//    }
//    argv++; argc--;
//  }
//
//  /* Return success */
//  return 1;
//}
///////////////////////////////////////////////

//===== Networking Thread program ==========================================================
void udpImageReceiver(){
    //printf("\n[MSG] udpImageServer Running within a new Thread!");
    //Make Thread Cancellable
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

printf("\n[DEBUGGING] Test 1: Startup");
#ifdef _WIN32
  WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
  WSADATA wsaData;                              // Stuff for WSA functions
#endif
  int                  server_s;        // Server socket descriptor
  struct sockaddr_in   server_addr;     // Server Internet address
  struct sockaddr_in   client_addr;     // Client Internet address
  struct in_addr       client_ip_addr;  // Client IP address
  int                  addr_len;        // Internet address length
  char                 out_buf[200000];   // Output buffer for data
  char                 in_buf[200000];    // Input buffer for data
  int                  retcode;         // Return code

  char                    packet[300][MESSAGE_MTU+1]; //388 KB Picture
  int                  XorValue=0;
  int                  currentPacket=0;
  int                  totalPacket=0;
  int                  largestPacket=0;
  int                  countOfPacket=0;
  char                 message_buf[MESSAGE_MTU];
  char                 packet_buf[PACKET_MTU];
  char                 result[12];
  char                 signature_buf[SIGNATURE_MTU];


  char                 tempBuf[PACKET_MTU];
  int                  bytesReceived;
  int                    last_packet_length=0;
        /// Debugging
        int byteCounter=0;

#ifdef _WIN32
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

#ifdef __linux__
   // Register signal and signal handler
   signal(SIGINT, signal_callback_handler);
#endif
printf("\n[DEBUGGING] Test 2: Initialization and Startup Done!");

  // >>> Step #1 <<<
  // Create a socket
  //   - AF_INET is Address Family Internet and SOCK_DGRAM is datagram
  server_s = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_s < 0)
  {
    printf("*** ERROR - socket() failed \n");
    exit(-1);
  }
printf("\n[DEBUGGING] Test 3: Socket Created!");

  // >>> Step #2 <<<
  // Fill-in my socket's address information
  server_addr.sin_family = AF_INET;                 // Address family to use
  server_addr.sin_port = htons(PORT_NUM);           // Port number to use
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address
  retcode = bind(server_s, (struct sockaddr *)&server_addr,
    sizeof(server_addr));
  if (retcode < 0)
  {
    printf("*** ERROR - bind() failed \n");
    exit(-1);
  }
int bufferCounter;
bufferCounter=0;
printf("\n[DEBUGGING] Test 4: Socket Address Info Completed!");

//File ops:
FILE * pFile;
int packetcounter=0;
while(1){
///  printf("\n==================================================================================");

  // >>> Step #3 <<<
  // Wait to receive a message from client
///  printf("Waiting for recvfrom() to complete... \n");
  addr_len = sizeof(client_addr);
  retcode = recvfrom(server_s, in_buf, sizeof(in_buf), 0, (struct sockaddr *)&client_addr, &addr_len);

  // if (retcode < 0){
    // printf("\n[ERROR] recvfrom() failed \n");
    // exit(-1);
  // }

  // >>> Step #4 <<<
  // Send to the client using the server socket
/////////////////////////////////////////////////////////////////
///  int                         packet[300][MESSAGE_MTU]; //388 KB Picture
///  int                       XorValue=0;
///  int                       currentPacket=0;
///  int                       totalPacket=0;
///  int                       largestPacket=0;
///  int                       countOfPacket=0;
///  char                      message_buf[MESSAGE_MTU];
///  char                      signature_buf[SIGNATURE_MTU];
/////////////////////////////////////////////////////////////////

/// First way to scan in:
  /// OLD: sscanf(in_buf,"%4X %4X %4X %48X %s",&XorValue,&currentPacket,&totalPacket,&signature_buf,&message_buf);
  sscanf(in_buf,"%4X %4X %4X %48X",&XorValue,&currentPacket,&totalPacket,&signature_buf);

/// Variables for processing:
  bytesReceived=strlen(in_buf);
  int i;
  int forloopLen;
  bufferCounter+=bytesReceived;
// Attempt to pull values w/o sscanf, but if it aint broke -- dont FIX IT!
//  //Read 4 XOR BYTEs -- Save as INT
//      tempBuf[0]=in_buf[0];
//      tempBuf[1]=in_buf[1];
//      tempBuf[2]=in_buf[2];
//      tempBuf[3]=in_buf[3];
//      tempBuf[4]='\0';
//      sscanf(tempBuf,"%i",&XorValue);
//  //Read 4 BYTES -- Current Packet #
//      tempBuf[0]=in_buf[5];
//      tempBuf[1]=in_buf[6];
//      tempBuf[2]=in_buf[7];
//      tempBuf[3]=in_buf[8];
//      tempBuf[4]='\0';
//      sscanf(tempBuf,"%i",&currentPacket);
//
//  //Read 4 BYTES -- Total Packet #
//      tempBuf[0]=in_buf[10];
//      tempBuf[1]=in_buf[11];
//      tempBuf[2]=in_buf[12];
//      tempBuf[3]=in_buf[13];
//      tempBuf[4]='\0';
//      sscanf(tempBuf,"%i",&totalPacket);


  //Read 48 BYTES (or Ignore) for signature
        //#define  SIGNATURE_MTU      48
        forloopLen=SIGNATURE_MTU;
        //printf("\n[DEBUGGING] forloopLen: %i",forloopLen);
        //printf("\n[DEBUGGING] tempBuf Before length: %i", strlen(tempBuf));
        for(i=0;i<forloopLen;i++){
                tempBuf[i]=in_buf[i+NON_MESSAGE_BYTES-SIGNATURE_MTU];
        }

        tempBuf[SIGNATURE_MTU]='\0'; //This allows the signature buffer to be parsed out of the message

        //printf("\n[DEBUGGING] tempBuf After length: %i", strlen(tempBuf));
        //printf("\n[DEBUGGING] tempBuf: %s", tempBuf);
        strncpy (signature_buf,tempBuf,strlen(tempBuf));
  //Read Message BYTES -- save as char buffer
        forloopLen=bytesReceived;
        //printf("\n[DEBUGGING] forloopLen: %i",forloopLen);
        //printf("\n[DEBUGGING] tempBuf Before length: %i", strlen(tempBuf));

        //tempBuf[PACKET_MTU]

        //Buffer Cleaning
        /// Does this need to be done after EACH packet?
//        if(currentPacket == totalPacket){ //case of 1/1 packets or nth/nth
			 //           last_packet_length=bytesReceived-NON_MESSAGE_BYTES;
			 //           printf("\n\n[DEBUGGING] LAST PACKET!!!!\n[DEBUGGING] Size: %i\n\n", last_packet_length);
			 //
			 //          //CLEAN THE MESSAGE BUFFERS!
//			 printf("\n[DEBUGGING] Last Packet Shenanigans...");
//			 printf("\n[DEBUGGING] Retcode from recvfrom(): %d",retcode);
//			for(i=0;i<MESSAGE_MTU;i++){
//				message_buf[i]=""; //'\0'
//			}
			 //           for(i=0;i<PACKET_MTU;i++){
			 //               tempBuf[i]=""; //'\0'
			 //           }
//        }

		///BAN THE PACKET! (It's Malformed), but for now... roll with it:
		if(XorValue <= 0 || XorValue>1326){
			XorValue=1326;
		}
		memcpy(packet[currentPacket-1],in_buf+61,XorValue);
        //strcpy (tempBuf,NULL);
 //       for(i=0;i<forloopLen;i++){//subtract '\0' terminator
 //               tempBuf[i]=in_buf[i+NON_MESSAGE_BYTES];
 //       }
        if(currentPacket == totalPacket){ //exception handling: Was noticing odd "0h0A" character on the final packet fragment
        //Null Terminate the Buffer
        //tempBuf[forloopLen]='\0';
        //}

       // printf("\n[DEBUGGING] tempBuf After length: %i", strlen(tempBuf));
       // printf("\n[DEBUGGING] tempBuf: %s", tempBuf);
 //      if(currentPacket == totalPacket){
 //           strncpy (packet[currentPacket-1],tempBuf,forloopLen);
 //      }
 //      else{
 //           strncpy (packet[currentPacket-1],tempBuf,MESSAGE_MTU);
			last_packet_length=XorValue;
		}
//		strncpy (packet[currentPacket-1],message_buf,XorValue);
//		strncpy (packet[currentPacket-1],message_buf,MESSAGE_MTU);

///		printf("\n=========================================");
///  printf("\n[DEBUGGING] Packet Number: %i", packetcounter);
///  printf("\n[DEBUGGING] Bytes received: %i", bytesReceived);
///  printf("\n[DEBUGGING] This is the XorValue: %i", XorValue);
///  printf("\n[DEBUGGING] This is the Current Packet: %i", currentPacket);
///  printf("\n[DEBUGGING] This is the Total Packets: %i", totalPacket);
///  printf("\n[DEBUGGING] This is the Signature Buffer: %s", signature_buf);
///  printf("\n[DEBUGGING] Sizeof packet[][] %i\n", sizeof(packet));
///  printf("\n[DEBUGGING] Sizeof packet[currentPacket][] %i\n", sizeof(packet[currentPacket-1]));
///  printf("\n[DEBUGGING] Buffer Counter: %i\n", bufferCounter);
///  printf("\n[DEBUGGING] This is the Message Buffer(length: %i): \n%s#EOP\n\n", forloopLen, packet[currentPacket-1]);

  snprintf(out_buf,sizeof(out_buf), "");
    packetcounter++;

        if(packetcounter >= totalPacket){ //We have seen at LEAST totalPacket worth of packets... so process data
            packetcounter=0;
///            printf("\n\n[DEBUGGING] WRITING TO FILE!\n\n");
///            printf("\n[DEBUGGING] Last Packet Length: %i\n\n", last_packet_length);
        pthread_mutex_lock(&mutex);
            pFile=fopen("Image.jpg","wb+");
            for(i=0;i<totalPacket;i++){
                //if (i==1){pFile=fopen("test.txt","w");}
                //else{pFile=fopen("test.txt","a");}

                if(i==(totalPacket-1)){
                    fwrite(packet[i], 1, last_packet_length, pFile);
                }
                else{
                    fwrite(packet[i], 1, MESSAGE_MTU, pFile);
                   /// printf("%s\n\n", packet[i]);

                }

            }
            fclose(pFile);
        pthread_mutex_unlock(&mutex);

            jpegRecv=1;
        }
          pthread_testcancel();

/////////////////////////////
/////////////////////////////
//XorValue[0]=in_buf[0];
//bufferCounter++;
//XorValue[1]=in_buf[2];
//bufferCounter++;
//XorValue[3]=in_buf[3];
//bufferCounter++;
/////////////////////////////
/////////////////////////////

// retcode = sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,
    // (struct sockaddr *)&client_addr, sizeof(client_addr));
  // if (retcode < 0)
  // {
    // printf("*** ERROR - sendto() failed \n");
    // exit(-1);
  // }
}
  // >>> Step #5 <<<
  // Close all open sockets
#ifdef _WIN32
  retcode = closesocket(server_s);
  if (retcode < 0)
  {
    printf("*** ERROR - closesocket() failed \n");
    exit(-1);
  }
#endif

#ifdef __linux__
  retcode = close(server_s);
  if (retcode < 0)
  {
    printf("*** ERROR - close() failed \n");
    exit(-1);
  }
#endif

#ifdef _WIN32
  // This stuff cleans-up winsock
  WSACleanup();
#endif
}

//===== Main program ==========================================================
 int main(){
    printf("\n==========================================");
    printf("\n| Streaming Baby Video Monitor 0.1 Alpha |");
    printf("\n==========================================");
    printf("\n[MSG] Main Camera Thread Running!");
    pthread_t    thread_camera;

    jpegRecv=0;
    IplImage* frame;
    cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    frame = cvLoadImage( "Image.jpg", CV_LOAD_IMAGE_COLOR );
    if(pthread_create(&thread_camera, NULL, udpImageReceiver, NULL)){
        exit(-1);
    }
    while(1){
        pthread_mutex_lock(&mutex);
            if(jpegRecv==1){
                frame = cvLoadImage( "Image.jpg", CV_LOAD_IMAGE_COLOR );
            }
        pthread_mutex_unlock(&mutex);

        cvShowImage( "mywindow", frame );

        if(jpegRecv==1){
            cvReleaseImage( &frame ); // Memory leak fix
            jpegRecv=0;
        }
        if((cvWaitKey(10) & 255) == 27) break; //If ESC key pressed
   }
   cvDestroyWindow( "mywindow" );
   return 0;

    pthread_cancel(thread_camera);

   return 0;
 }
