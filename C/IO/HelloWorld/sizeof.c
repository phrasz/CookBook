#include <stdio.h>
#include <stdlib.h>

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


int main()
{
    char                 signature_buf[SIGNATURE_MTU];
    char                 packet_buf[PACKET_MTU];
    char                 message_buf[MESSAGE_MTU];

    printf("Hello world!\n");
    //return 0;
    int value;
    printf("\n[DEBUGGING] Size of Int: %d",sizeof(value));
    printf("\n[DEBUGGING] Size of Signature: %d",sizeof(signature_buf));
    printf("\n[DEBUGGING] Size of Message: %d",sizeof(message_buf));
    printf("\n[DEBUGGING] Size of Packet: %d",sizeof(packet_buf));
    return 0;
}
