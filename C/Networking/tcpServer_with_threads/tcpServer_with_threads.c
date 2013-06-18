//====================================================== file = tcpServer_with_threads.c =====
//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>         // Needed for exit()
#include <string.h>         // Needed for memcpy() and strcpy()
#include <fcntl.h>          // Needed for file i/o stuff
#ifdef _WIN32
  #include <process.h>      // Needed for _beginthread() and _endthread()
  #include <stddef.h>       // Needed for _threadid
  #include <windows.h>      // Needed for all Winsock stuff
  #include <sys\stat.h>     // Needed for file i/o constants
  #include <io.h>           // Needed for file i/o stuff
#endif
#ifdef __linux__
  #include <pthread.h>      // Needed for pthread_create() and pthread_exit()
  #include <sys/stat.h>     // Needed for file i/o constants
  #include <sys/types.h>    // Needed for sockets stuff
  #include <netinet/in.h>   // Needed for sockets stuff
  #include <sys/socket.h>   // Needed for sockets stuff
  #include <arpa/inet.h>    // Needed for sockets stuff
  #include <fcntl.h>        // Needed for sockets stuff
  #include <netdb.h>        // Needed for sockets stuff
#endif

//----- Defines -------------------------------------------------------------
#define  PORT_NUM            33213     // Port number for Web server
#define  BUF_SIZE            1300     // Buffer size (big enough for a GET)

//----- Function prototypes -------------------------------------------------
#ifdef _WIN32
  void handle_get(void *in_arg);      // Windows thread function to handle GET
#endif
#ifdef __linux__
  void *handle_get(void *in_arg);     // POSIX thread function to handle GET
#endif

//===== Main program ========================================================
int main(void){
	#ifdef _WIN32
		WORD wVersionRequested = MAKEWORD(1,1);    // Stuff for WSA functions
		WSADATA wsaData;                           // Stuff for WSA functions
	#endif
	int                  server_s;             // Server socket descriptor
	struct sockaddr_in   server_addr;          // Server Internet address
	int                  client_s;             // Client socket descriptor
	struct sockaddr_in   client_addr;          // Client Internet address
	struct in_addr       client_ip_addr;       // Client IP address
	#ifdef _WIN32
		int                  addr_len;             // Internet address length
	#endif
	#ifdef __linux__
		socklen_t            addr_len;             // Internet address length
		pthread_t            thread_id;            // Thread ID
	#endif
	int                  retcode;              // Return code

	#ifdef _WIN32
		// This stuff initializes winsock
		WSAStartup(wVersionRequested, &wsaData);
	#endif

	// Create a socket, fill-in address information, and then bind it
	server_s = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	retcode = bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (retcode < 0){
		printf("*** ERROR - bind() failed \n");
		exit(-1);
	}

	// Set-up the listen
	listen(server_s, 100);

	// Main loop to accept connections and then spin-off thread to handle the GET
	printf(">>> weblite is running on port %d <<< \n", PORT_NUM);
	while(1){
		addr_len = sizeof(client_addr);
		client_s = accept(server_s, (struct sockaddr *)&client_addr, &addr_len);
		if (client_s == -1){
			printf("\n[ERROR] Unable to create a socket!");
			exit(1);
		}

		#ifdef _WIN32
			if (_beginthread(handle_get, 4096, (void *)client_s) < 0){
		#endif
		#ifdef __linux__
			if (pthread_create(&thread_id, NULL, handle_get, (void *)client_s) != 0){
		#endif
		printf("ERROR - Unable to create a thread to handle the GET \n");
		exit(1);
		}
	}

  return(0);
}

//===========================================================================
//=  This is is the thread function to handle the GET                       =
//===========================================================================
#ifdef _WIN32
void handle_get(void *in_arg) {
#endif
#ifdef __linux__
void *handle_get(void *in_arg) {
#endif
	int   client_s;             // Client socket descriptor
	char  in_buf[BUF_SIZE];     // Input buffer for GET request
	char  out_buf[BUF_SIZE];    // Output buffer for HTML response
	int   buf_len;              // Buffer length for file reads
	int   retcode;              // Return code
	char  command[BUF_SIZE];    // Command buffer
	int randomInt;
	int	numbertoadd;
	int tempbuf;
	// Set client_s to in_arg
	client_s = (int)in_arg;

	// Receive the data from the client
	retcode = recv(client_s, in_buf, BUF_SIZE, 0);

	if (retcode <= 0){ // If bad then bail-out
		printf("ERROR - Receive failed --- probably due to dropped connection \n");
		#ifdef _WIN32
			closesocket(client_s);
			_endthread();
		#endif
		#ifdef __linux__
			close(client_s);
			pthread_exit(NULL);
		#endif
	}

	// Parse out the command from the (presumed) GET request and filename
	//sscanf(in_buf, "%s", command);
	memcpy(&tempbuf, in_buf, 4);
	randomInt=rand();
	numbertoadd=ntohl(tempbuf)+randomInt;
	
	tempbuf=htonl(numbertoadd);
	printf("\n[DEBUGGING] Sending new value to the network: %i (from random: %i)",numbertoadd,randomInt);
	memcpy(out_buf,&tempbuf,4);
	//memcpy(out_buf+4,'0',1);
	send(client_s, out_buf, 4, 0);
	
	
	// Close the client socket, and end the thread
	#ifdef _WIN32
		closesocket(client_s);
		_endthread();
	#endif
	#ifdef __linux__
		close(client_s);
		pthread_exit(NULL);
	#endif
}
