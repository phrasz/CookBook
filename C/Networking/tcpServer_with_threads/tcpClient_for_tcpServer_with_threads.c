//==================================================== file = tcpClient_for_tcpServer_with_threads.c =====
//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
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
#endif

//----- Defines ---------------------------------------------------------------
#define  PORT_NUM         33213   // Port number used at the server
#define  IP_ADDR    "127.0.0.1"  // IP address of server (*** HARDWIRED ***)

//===== Main program ==========================================================
void main(void)
{
	#ifdef _WIN32
	WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
	WSADATA wsaData;                              // Stuff for WSA functions
	#endif
	int                  client_s;        // Client socket descriptor
	struct sockaddr_in   server_addr;     // Server Internet address
	char                 out_buf[4096];   // Output buffer for data
	char                 in_buf[4096];    // Input buffer for data
	int                  retcode;         // Return code
	int		numbertosend=1337;
	int		tempval;
	int		numberrecieved;
	//char	nullval='\0';
#ifdef _WIN32
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  // >>> Step #1 <<<
  // Create a client socket
  //   - AF_INET is Address Family Internet and SOCK_STREAM is streams
  client_s = socket(AF_INET, SOCK_STREAM, 0);
  if (client_s < 0)
  {
    printf("*** ERROR - socket() failed \n");
    exit(-1);
  }

  // >>> Step #2 <<<
  // Fill-in the server's address information and do a connect with the
  // listening server using the client socket - the connect() will block.
  server_addr.sin_family = AF_INET;                 // Address family to use
  server_addr.sin_port = htons(PORT_NUM);           // Port num to use
  server_addr.sin_addr.s_addr = inet_addr(IP_ADDR); // IP address to use
  
  //CONNECT TO SERVER!
	printf("\n[DEBUGGING] Conecting to server...");
	retcode = connect(client_s, (struct sockaddr *)&server_addr,
	sizeof(server_addr));
	if (retcode < 0){
		printf("*** ERROR - connect() failed \n");
		exit(-1);
	}
	else{
		printf("\n[DEBUGGING] CONNECTED!");
	}

  // Send to the server using the client socket
  //strcpy(out_buf, "This is a reply message from CLIENT to SERVER");
	printf("\n[DEBUGGING] Prepping message: %i ...", numbertosend);
	tempval=htonl(numbertosend);
	memcpy(out_buf,&tempval,4);
	printf("\n[DEBUGGING] Prepping null terminator...");
	//memcpy(out_buf+4,'\0',1);
	retcode = send(client_s, out_buf, 4, 0);
	if (retcode < 0){
		printf("*** ERROR - send() failed \n");
		exit(-1);
	}
	else{
		printf("\n[DEBUGGING] Message sent!");
	}

	// >>> Step #3 <<<
	// Receive from the server using the client socket
	printf("\n[DEBUGGING] Waiting for message...");
	retcode = recv(client_s, in_buf, sizeof(in_buf), 0);
	if (retcode < 0){
		printf("*** ERROR - recv() failed \n");
		exit(-1);
	}
	else{
		printf("\n[DEBUGGING] Message Receieved!");
	}
	memcpy(&tempval, in_buf, 4);
	numberrecieved=ntohl(tempval);
	printf("\n[DEBUGGING] Number Received: %i",numberrecieved);
	
  // >>> Step #5 <<<
  // Close the client socket
#ifdef _WIN32
  retcode = closesocket(client_s);
  if (retcode < 0)
  {
    printf("*** ERROR - closesocket() failed \n");
    exit(-1);
  }
#endif
#ifdef __linux__
  retcode = close(client_s);
  if (retcode < 0)
  {
    printf("*** ERROR - close() failed \n");
    exit(-1);
  }
#endif

#ifdef _WIN32
  // Clean-up winsock
  WSACleanup();
#endif
}
