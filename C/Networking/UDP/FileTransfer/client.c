/*
   This file is part of "UDP file transfer".
   
   "UDP file transfer" is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   "UDP file transfer" is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with "UDP file transfer".  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "udp.h"
#include "udpft.h"
#define MAXNAMELEN 256

void parse();
int transfer();

int main (int argc,char **argv){
   struct sockaddr_in server;
   struct fgetinfo    *p1;
   struct finfo       p4;
   char               buffer[10000];
   FILE *foutfd;
   
   char host[1000],fin[FILENAME_MAX],fout[FILENAME_MAX];
   int sock,port,connected,retval;
 
   parse(argc,argv,host,&port,fin,fout);
   
   /* PRINT PARSED ARGUMENTS */
   printf("Port %i<--\n",port);
   printf("Server %s<--\n",host);

   start_client(&sock,&server,port,host);
   printf("Starting communication\n\n");
   p1=get_info(fin);

   // Try to open out file
   if((foutfd=(FILE *)fopen(fout,"wb"))==NULL){
      printf("Out file could't be created or truncated for writing\n");
      exit(1);
   }
   
   connected=0;
   while (connected<3){
      connected++;
      send_buf(sock,&server,p1,sizeof(*p1));
      print_fgetinfo(*p1);
      
      if(receive(sock,buffer,sizeof(buffer))==0){
         memcpy(&p4,buffer,sizeof(p4));
         if(!check_finfo(p4)){
            connected=4;
            print_finfo(p4);
            if (p4.file_exist==1){
               // Start transfer
               retval=transfer(sock,&server,p4.file_id,foutfd,p4.file_size);
               fclose(foutfd);
               // Show transfer result
               exit(retval);
            }
            else{
               printf("\nERROR: Requested file does not exist on server\n\n");
               exit(1);
            }
         }
      }
   }
   if (connected==3){
      printf("\nERROR: Server not responding\n\n");
      exit(1);
   }
   exit (0);
}


/* PARSE COMMAND LINE ARGUMENTS */
void parse(int argc,char **argv,char *host,int *port,char *fin,char *fout){
   char usage[40]="port host remote_file local_file\n\0";
   if (argc<5){
      fprintf(stderr,"Usage: %s %s",*argv,usage);
      exit (1);
   }
   *port=atoi(argv[1]);
   strcpy(host,argv[2]);
   strcpy(fin,argv[3]);
   strcpy(fout,argv[4]);
}


/* TRANSFER */
int transfer(int sock,struct sockaddr_in *server,int file_id,FILE *foutfd,int size){
   struct fgetfrag *req;
   struct ffrag     ans;
   int last_asked=0;
   int offset=0;


   printf("\nvale: %i\n",sizeof(*req));
   printf("vale: %i\n",sizeof(req->type));
   printf("vale: %i\n",sizeof(req->file_id));
   printf("vale: %i\n",sizeof(req->offset));
   printf("vale: %i\n",sizeof(req->md5));


   while (offset<size){
      req=get_frag(file_id,offset);
      last_asked=offset;
      send_buf(sock,server,req,sizeof(*req));
      print_fgetfrag(*req);
      if(receive(sock,(void *)&ans,sizeof(ans))==0){
         if(check_ffrag(ans)){
            printf("Corrupt packet\n");
            exit(1);
         }
         else{
            if (ans.offset==last_asked){
               // Answer correct
               fseek(foutfd,SEEK_SET,ans.offset);
               if(fwrite(ans.fragment,sizeof(char),ans.size,foutfd)!=ans.size){
                  // Fail writing to outfile
                  printf("Error writing to file");
                  exit(1);
               }
               offset=ans.offset+ans.size;
            }
         }
      }
   }
   return 0;
}



