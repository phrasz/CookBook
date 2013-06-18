//======================================================== file = blast.c =====
//=  A simple program to "blast" UDP packets                                  =
//=   - Be careful of IP_ADDR (default should be 127.0.0.1)                   =
//=============================================================================
//=  Notes:                                                                   =
//=    1) This program conditionally compiles for Winsock and BSD sockets.    =
//=       Set the initial #define to WIN or BSD as appropriate.               =
//=    2) There is *no* error checking in this program.  Error checking was   =
//=       omitted to simplify the program.                                    =
//=    3) Must manually set IP_ADDR, MESS_SIZE, and N.                        =
//=    4) Computed data rate does not consider packet headers                 =
//=---------------------------------------------------------------------------=
//=  Build: bcc32 blast.c or cl blast.c wsock32.lib for Winsock               =
//=         gcc blast.c -lsocket -lnsl for BSD                                =
//=---------------------------------------------------------------------------=
//=  History:  KJC (06/02/06) - Genesis (from client1.c)                      =
//=            KJC (02/21/07) - Added some timing                             =
//=============================================================================

//----- Include files ---------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#ifdef _WIN32
  #include <windows.h>      // Needed for all Winsock stuff
  #include <sys\timeb.h>    // Needed for ftime() and timeb structure
#endif
#ifdef __linux__
  #include <sys/types.h>    // Needed for system defined identifiers.
  #include <netinet/in.h>   // Needed for internet address structure.
  #include <sys/socket.h>   // Needed for socket(), bind(), etc...
  #include <arpa/inet.h>    // Needed for inet_ntoa()
  #include <sys/timeb.h>    // Needed for ftime() and timeb structure
  #include <fcntl.h>        // Needed for sockets stuff
  #include <netdb.h>        // Needed for sockets stuff
#endif

//----- Defines ---------------------------------------------------------------
#define  PORT_NUM            1050      // Port number used
#define  IP_ADDR       "127.0.0.1"     // IP address to blast to
#define  MESS_SIZE           1024      // Message size
#define  N                 100000      // Number of times to send message

//===== Main program ==========================================================
void main(void)
{
#ifdef _WIN32
  WORD wVersionRequested = MAKEWORD(1,1);       // Stuff for WSA functions
  WSADATA wsaData;                              // Stuff for WSA functions
#endif

  unsigned int         my_s;               // My (client1) socket descriptor
  struct sockaddr_in   dest_addr;          // Server Internet address
  unsigned int         addr_len;           // Internet address length
  char                 out_buf[MESS_SIZE]; // Buffer for output data
  struct timeb         start, stop;        // Start and stop times structures
  double               elapsed;            // Elapsed time in seconds
  double               data_rate;          // Computed data rate in Mb/s
  unsigned int         i;                  // Loop counter

#ifdef _WIN32
  // This stuff initializes winsock
  WSAStartup(wVersionRequested, &wsaData);
#endif

  // Create a socket and fill in destination address information
  my_s = socket(AF_INET, SOCK_DGRAM, 0);
  dest_addr.sin_family      = AF_INET;            // Address family to use
  dest_addr.sin_port        = htons(PORT_NUM);    // Port num to use
  dest_addr.sin_addr.s_addr = inet_addr(IP_ADDR); // IP address to use

  // Load the output buffer
  for (i=0; i<MESS_SIZE; i++)
    out_buf[i] = 0x55;

  // Output start banner
  printf("Blasting %d messages of size %d bytes to %s... \n",
    N, MESS_SIZE, IP_ADDR);

  // Start timing
  ftime(&start);

  // Blast out the test message in the output buffer N times
  for (i=0; i<N; i++)
  {
    sendto(my_s, out_buf, MESS_SIZE, 0,
      (struct sockaddr *)&dest_addr, sizeof(dest_addr));
  }

  // Stop timing and compute elapsed time
  ftime(&stop);
  elapsed=((double) stop.time + ((double) stop.millitm * 0.001)) -
          ((double) start.time + ((double) start.millitm * 0.001));

  // Compute data rate
  data_rate = ((8.0 * N * MESS_SIZE) / elapsed) / 1000000.0;

  // Output daat rate
  printf("Done.  Measured data rate = %f Mb/s \n", data_rate);

  // Close all open sockets and clean-up
#ifdef _WIN32
  closesocket(my_s);
  WSACleanup();
#endif
#ifdef __linux__
  close(my_s);
#endif
}
