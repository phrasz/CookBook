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

#include "udpft.h"
#include "md5.h"

int packetType(char *p){
   return((int)*p);
}

/* FGETINFO */
struct fgetinfo *get_info(char file_path[FILENAME_MAX]){
   struct fgetinfo *p;
   p=malloc(sizeof(struct fgetinfo));
   p->type=1;
   strncpy(p->file_path,file_path,FILENAME_MAX);
   // set md5 to 0
   memset(p->md5,0,MD5_SIZE);
   strncpy(p->md5,calculate_md5(p,sizeof(*p)),MD5_SIZE);
   return(p);
}

void print_fgetinfo(struct fgetinfo p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   md5[MD5_SIZE]='\0';
   printf("Packet:\n - type: %c\n",p.type+'0');
   printf(" - path: %s\n",p.file_path);
   printf(" - md5: %s\n",md5);
}

int check_fgetinfo(struct fgetinfo p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,sizeof(p.md5));
   memset(p.md5,0,sizeof(p.md5));
   strncpy(p.md5,calculate_md5(&p,sizeof(p)),MD5_SIZE);
   if (strncmp(p.md5,md5,MD5_SIZE)==0){
      return(0);
   }
   return(1);
}


/* FGETFRAG */
struct fgetfrag *get_frag(fid_t file_id,int offset){
   struct fgetfrag *p;
   p=malloc(sizeof(struct fgetfrag));
   p->type=2;
   p->file_id=file_id;
   p->offset=offset;
   // set md5 to 0
   memset(p->md5,0,sizeof(p->md5));
   strncpy(p->md5,calculate_md5(p,sizeof(*p)),MD5_SIZE);
   return(p);
}

void print_fgetfrag(struct fgetfrag p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   md5[MD5_SIZE]='\0';
   printf("Packet:\n - type: %c\n",p.type+'0');
   printf(" - file id: %i\n",p.file_id);
   printf(" - offset: %i\n",(int)p.offset);
   printf(" - md5: %s\n",md5);
}

int check_fgetfrag(struct fgetfrag p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,sizeof(p.md5));
   memset(p.md5,0,sizeof(p.md5));
   strncpy(p.md5,calculate_md5(&p,sizeof(p)),MD5_SIZE);
   if (strncmp(p.md5,md5,MD5_SIZE)==0){
      return(0);
   }
   return(1);
}

/* FEND */

struct fend *get_fend(fid_t file_id){
   struct fend *p;
   p=malloc(sizeof(struct fend));
   p->type=3;
   p->file_id=file_id;
   // set md5 to 0
   memset(p->md5,0,sizeof(p->md5));
   strncpy(p->md5,calculate_md5(p,sizeof(*p)),MD5_SIZE);
   return(p);
}

void print_fend(struct fend p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   md5[MD5_SIZE]='\0';
   printf("Packet:\n - type: %c\n",p.type+'0');
   printf(" - file id: %i\n",p.file_id);
   printf(" - md5: %s\n",md5);
}

int check_fend(struct fend p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   memset(p.md5,0,sizeof(p.md5));
   strncpy(p.md5,calculate_md5(&p,sizeof(p)),MD5_SIZE);
   if (strncmp(p.md5,md5,MD5_SIZE)==0){
      return(0);
   }
   return(1);
}

/* FINFO */

struct finfo *get_sinfo(int file_exist,int file_id,int file_size){
   struct finfo *p;
   p=malloc(sizeof(struct finfo));
   p->type=4;
   p->file_exist=file_exist;
   p->file_id=file_id;
   p->file_size=file_size;
   // set md5 to 0
   memset(p->md5,0,sizeof(p->md5));
   strncpy(p->md5,calculate_md5(p,sizeof(*p)),MD5_SIZE);
   return(p);
}
void print_finfo(struct finfo p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   md5[MD5_SIZE]='\0';
   printf("Packet:\n - type: %c\n",p.type+'0');
   printf(" - file exist: %i\n",p.file_exist);
   printf(" - file id: %i\n",p.file_id);
   printf(" - file size: %i\n",p.file_size);
   printf(" - md5: %s\n",md5);
}

int check_finfo(struct finfo p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   memset(p.md5,0,sizeof(p.md5));
   strncpy(p.md5,calculate_md5(&p,sizeof(p)),MD5_SIZE);
   if (strncmp(p.md5,md5,MD5_SIZE)==0){
      return(0);
   }
   return(1);
}

/* FFRAG */

struct ffrag *get_ffrag(fid_t file_id,int offset,fra_t fragment,int size){
   struct ffrag *p;
   p=malloc(sizeof(struct ffrag));
   p->type=5;
   p->file_id=file_id;
   p->offset=offset;
   memcpy(p->fragment,fragment,sizeof(fra_t));
   p->size=size;
   // set md5 to 0
   memset(p->md5,0,sizeof(p->md5));
   strncpy(p->md5,calculate_md5(p,sizeof(*p)),MD5_SIZE);
   return(p);
}

void print_ffrag(struct ffrag p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   md5[MD5_SIZE]='\0';
   char fragment[FRAGMENT_SIZE+1];
   printf("Packet:\n - type: %c\n",p.type+'0');
   printf(" - file id: %i\n",p.file_id);
   printf(" - offset: %i\n",(int)p.offset);
   strncpy(fragment,p.fragment,p.size);
   fragment[p.size]='\0';
   printf(" - fragment: Better not to print that\n");
   printf(" - size: %i\n",p.size);
   printf(" - md5: %s\n",md5);
}

int check_ffrag(struct ffrag p){
   char md5[MD5_SIZE+1];
   strncpy(md5,p.md5,MD5_SIZE);
   memset(p.md5,0,sizeof(p.md5));
   strncpy(p.md5,calculate_md5(&p,sizeof(p)),MD5_SIZE);
   if (strncmp(p.md5,md5,MD5_SIZE)==0){
      return(0);
   }
   return(1);
}


/**************************************************************************/
/*       server_receive                                                   */
/*       return 0 -> recvfrom error                                       */
/*       else return packetType(buffer)                                   */
/**************************************************************************/
int server_receive(int s,char *buffer,int buflen,struct sockaddr *remote,int *rlen){
   *rlen = sizeof(struct sockaddr);
   if((recvfrom(s,buffer,buflen,0,(struct sockaddr *)remote,(socklen_t *)rlen))<0){
//       printf("Liada\n");
      return 0;
   }
   return (packetType(buffer));
   
}



