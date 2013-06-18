/*
2 * sendto1.c
3 *
4 * cross-platform  compatible  example  of
5 * sendto() function. send UDP datagram to
6 * port 1234 at address 127.0.0.1
7 */
#ifdef WIN32

 /* required for winsock */
 #pragma comment(lib, "ws2_32.lib")

 #include <winsock2.h>

 #else

 /* UNIX header .les */
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>

 #endif

 #include <stdio.h>

 /* target IP address & port to connect to */
 #de.ne TARGET_ADDR "127.0.0.1"
 #de.ne TARGET_PORT 1234

 /* example data to send */
 struct data{
int x;
int y;
};
int main(void){
//Portable Network Programming . Chapter 7 299
//327_SSPC_07.qxd  2/25/05  1:47 PM  Page 29942
#ifdef WIN32
WSADATA wsa; /* used by WSAStartup() */
SOCKET  sd = 0;
#else
int     sd = 0;
#endif
struct sockaddr_in sin ;
struct data        data;
int                ret = 0;
/* must initialize winsock if on win32 platform */
#ifdef WIN32
memset(&wsa, 0x0, sizeof(WSADATA));
57 if(WSAStartup(MAKEWORD(2, 0), &wsa) != 0x0)
{
printf("WSAStartup() failed.\n");
return(1);
}
#endif
/* create UDP socket */
sd     = socket(AF_INET, SOCK_DGRAM, 0);
/* if Win32, check for INVALID_SOCKET constant */
#ifdef WIN32
if(sd == INVALID_SOCKET)
/* otherwise, check for -1 */
#else
if(sd  < 0)
#endif
{
printf("socket() failed.\n");
return(1);
}
printf("socket descriptor created.\n");
/* de.ne remote end-point */
memset(&sin, 0x0, sizeof(sin));
sin.sin_family=AF_INET;
sin.sin_port=htons(TARGET_PORT);
sin.sin_addr.s_addr = inet_addr(TARGET_ADDR);
ret     = connect(sd, (struct sockaddr *) &sin, sizeof(sin));
#ifdef WIN32
if(ret ==SOCKET_ERROR)
#else
if(ret  < 0)
#endif
{
printf("connect() failed.\n");
return(1);
}
/* send data using send function */
// Chapter 7 . Portable Network Programming
///_SSPC_07.qxd  2/25/05  1:47 PM  Page 30099 
data.x = 0;
data.y = 0;
/* cast pointer from type struct data * to const char * to avoid compiler warnings w/ Visual Studio */
ret     = send(sd, (const char *) &data, sizeof(data), 0);
#ifdef WIN32
if(ret == SOCKET_ERROR)
#else
if(ret  < 0)
#endif
{
printf("send() failed.\n");
return(1);
}
printf("data sent.\n");
return(0);
}
