#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include <errno.h>

#define WWW_PORT	80

//--------------------------------------------------------------------------
// Print an error message to stderr and terminate the program immediately
void
printerror(char *pcErrStr)
{
    fprintf(stderr, "ERROR: %s\n\n", pcErrStr);
    exit(1);
}

//--------------------------------------------------------------------------
// Input parameters are the name of the server to query and a pointer to a
// sockaddr_in structure to fill.  The function performs a lookup of the
// specified Internet name and stores the address along with the web port
// number (WWW_PORT) in the provided sockaddr_in structure
void
FillAddress(char *pcURL, struct sockaddr_in *psAddress)
{
    struct hostent      *psHostDetails;

    if ((psHostDetails = gethostbyname(pcURL)) == NULL) printerror("Can't determine address of website");

    memcpy(&(psAddress->sin_addr), psHostDetails->h_addr_list[0], 4);
    psAddress->sin_family = AF_INET;
    psAddress->sin_port = htons(WWW_PORT);

    printf("Server details:\n");
    printf("  Server:\t%s\n", pcURL);
    printf("  IP Address:\t%s\n", inet_ntoa(psAddress->sin_addr)); //Convert to human readable string
}

//--------------------------------------------------------------------------
int
main(int iArgC, char **ppcArgV)
{
    struct sockaddr_in  sAddress;
    char                *pcWWWServer, *pcPage, *pcPos;

    if (iArgC != 2) printerror("Command Line requires one parameter = website");

    // If the provided URL begins with "http://" then set pcWWWServer to point
    // to the next character otherwise set it to point to the entire provided URL
	//
	// strstr() - find a string in another string. return the first position
    if (pcWWWServer = strstr(ppcArgV[1], "http://")) 
        pcWWWServer+= 7;  //skip our pointer pcWWWServer forward 7 characters. Past the "http://"
    else 
        pcWWWServer = ppcArgV[1]; //It doesnt have http://, just grab the pointer to the string


	//Work out what the page part of the path is
    // If there is a "/" in the URL (after the http:// has been stripped) then 
    // replace it with a 0 so the string terminates as just the name.  Create 
    // a new string pointer (pcPage) to be a copy of the remaining string OR
    // just "/" if no page details have been provided
	//
	// strstr() - find a string in another string. return the first position
	// strdup() - duplicate a string, return a pointer to a new string that's a copy. New memory!
    if (pcPos = strstr(pcWWWServer, "/"))
    {
        pcPage = strdup(pcPos);
        pcPos[0] = 0; //We're splitting the original string by doing this. But entering a null
    } else
        pcPage = strdup("/");

    // Call FillAddress() to fill sAddress with the IP address details (and port
    // number 80) of the requested URL
	//
	//              INPUT      OUTPUT
    FillAddress(pcWWWServer, &sAddress);


    /* Download The Webpage */

    //Get some memory ready to store the entire page
    //What if our page was bigger than this?!
    //This is a bad design and we wouldn't use it in production code.
    char bufferAllOfPage[1000000];
    bzero(bufferAllOfPage, sizeof(bufferAllOfPage));

	//Get the variables we need to open a network socket to the webserver
    //and get data back
	int sockfd;        //This will the be the var we communicate with the server through
	char buffer[1024]; //We'll use this for recieving data
	int bytes_read;    //This is to know how much data we've got

    //Try to create a new network socket
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printerror("Socket error");

    //Try to connect that socket to the webserver
    //See we use sAddress that we got earlier
    if( connect(sockfd, (struct sockaddr*)&sAddress, sizeof(sAddress)) != 0)
        printerror("Couldn't connect");

    //Ok, we're connected
    
    //Tell the webserver what page we want to retrieve
    //Don't forget to tell them which site we're asking for (The host bit)
    // and which page we want
    sprintf(buffer, "GET %s HTTP/1.0\nHost: %s\n\n", pcPage, pcWWWServer);
    send(sockfd, buffer, strlen(buffer), 0);


    //Start recieving data from the server
    //Grab a chunk at a time and put it into the temporary buffer
    //we made before called....buffer
    do
    {
        //Blank out our buffer of retreived data
        //Then ask the network stack to give us some more
        bzero(buffer, sizeof(buffer));
        bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if( bytes_read > 0) //Did we get any?
        {
            //Copy the text we just got from the server into the
            //buffer we're storing the whole page in
            strncat(bufferAllOfPage, buffer, bytes_read);

            //strncat stuffs one string on to the end of another string. It's smart
            //enough to find the right place by itself. We still pass along bytes_read
            //so that it only copies as many characters as we want it to.
        }
    }
    while ( bytes_read > 0); //keep going through this loop until we have all the page

    //Clean up our network socket
	close(sockfd);

/// --------------
/// Print Page Contents:
printf("\n[DEBUGGING] The following is the webpage's source: \n%s\n\n",bufferAllOfPage);

/// ---------------

    /* Now lets try and parse out the links and images */

    //We're doing to do some very nieve searching, so first thigns first, make
    //the whole string lower case. (Uses the ctype.h library)
    int i = 0;
    while(bufferAllOfPage[i])
    {
        bufferAllOfPage[i] = tolower(bufferAllOfPage[i]);
        i++;
    }

    printf("\n[DEBUGGING]Looking for Links:\n");
    //Step through the string looking for things like this
    //    href=' ...... '
    //These are links to other pages and documents
    //Work out where it starts and ends, then print this URL out to the user    
    char *posStartLink;
    char *posEndLink;
    char *placeToStartLooking;

    placeToStartLooking = bufferAllOfPage;
    do
    {
        //Find the first text saying ' href="'
        posStartLink = strstr(placeToStartLooking, " href=\"");
        if(posStartLink)
        {
            //If we found the start of a link, then try to find the end of it
            posStartLink += 7; //+7 to get us past the href=" characters
            posEndLink  = strstr(posStartLink, "\""); 
        }

        if(posStartLink){
		printf("\n[DEBUGGING] Found Start Position!");
	}
        if(posEndLink){
		printf("\n[DEBUGGING] Found END Position!");
	}

        //Did we find anything (start AND end)?
        if(posStartLink && posEndLink)
        {
            //Ok we found a link
            //Extract the text of that link into a new string
            int linkLength;
            char linkBuf[1000];
            linkLength = posEndLink - posStartLink; 
            strncpy(linkBuf, posStartLink, linkLength);
            linkBuf[linkLength] = 0;

            //Print out the link we found
            printf("\n[DEBUGGING] Link: %s\n", linkBuf);

            //Move up the position of where to look
            placeToStartLooking = posEndLink;
        }
        else
            break; //We didnt find anything, so stop looking.
    }
    while(posEndLink <= (bufferAllOfPage + strlen(bufferAllOfPage)) );
    //Keep going until the end of the HTML we got


    printf("\n\n[DEBUGGING]Looking for Name:\n");
   // char *posStartLink;
   // char *posEndLink;
   // char *placeToStartLooking;

    placeToStartLooking = bufferAllOfPage;
    do
    {
        //Find the first text saying ' href="'
        posStartLink = strstr(placeToStartLooking, "Name");
        if(posStartLink)
        {
            //If we found the start of a link, then try to find the end of it
            posStartLink += 1; //+7 to get us past the href=" characters
            posEndLink  = strstr(posStartLink, "\n"); 
        }
        if(posStartLink){
		printf("\n[DEBUGGING] Found Start Position!");
	}
        if(posEndLink){
		printf("\n[DEBUGGING] Found END Position!");
	}


        //Did we find anything (start AND end)?
        if(posStartLink && posEndLink){
            //Ok we found a link
            //Extract the text of that link into a new string
            int linkLength;
            char linkBuf[1000];
            linkLength = posEndLink - posStartLink; 
            strncpy(linkBuf, posStartLink, linkLength);
            linkBuf[linkLength] = 0;

            //Print out the link we found
            printf("%s\n", linkBuf);

            //Move up the position of where to look
            placeToStartLooking = posEndLink;
        }
        else
            break; //We didnt find anything, so stop looking.
    }
    while(posEndLink <= (bufferAllOfPage + strlen(bufferAllOfPage)) );
    //Keep going until the end of the HTML we got



    return 0;
}
