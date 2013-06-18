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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "md5.h"
#define FRAGMENT_SIZE 35000

typedef char typ_t;
typedef char md5_t[MD5_SIZE];
typedef int  fid_t;
typedef int  fsi_t;
typedef int  fex_t;
typedef int  siz_t;
typedef char fra_t[FRAGMENT_SIZE];
 
/* CLIENT */
/* #1 */
struct fgetinfo{
   typ_t type;
   char  file_path[FILENAME_MAX];
   md5_t md5;
};
/* #2 */
struct fgetfrag{
   typ_t type;
   fid_t file_id;
   int offset;
   md5_t md5;
};
/* #3 */
struct fend{
   typ_t type;
   fid_t file_id;
   md5_t md5;
};

/* SERVER */
/* #4 */
struct finfo{
   typ_t type;
   fex_t file_exist;
   fid_t file_id;
   fsi_t file_size;
   md5_t md5;
};
/* #5 */
struct ffrag{
   typ_t type;
   fid_t file_id;
   int offset;
   fra_t fragment;
   siz_t size;
   md5_t md5;
};

/* FILES */
struct file_s{
   FILE  *fd;
   fsi_t size;
   char  file_path[FILENAME_MAX];
   fid_t file_id;
};

int packetType(char *p);

/* FGETINFO */
struct fgetinfo *get_info(char file_path[FILENAME_MAX]);
void print_fgetinfo(struct fgetinfo p);
int check_fgetinfo(struct fgetinfo p);

/* FGETFRAG */
struct fgetfrag *get_frag(fid_t file_id,int offset);
void print_fgetfrag(struct fgetfrag p);
int check_fgetfrag(struct fgetfrag p);

/* FEND */

struct fend *get_fend(fid_t file_id);
void print_fend(struct fend p);
int check_fend(struct fend p);

/* FINFO */

struct finfo *get_sinfo(int file_exist,int file_id,int file_size);
void print_finfo(struct finfo p);
int check_finfo(struct finfo p);


/* FFRAG */

struct ffrag *get_ffrag(fid_t file_id,int offset,fra_t fragment,int size);
void print_ffrag(struct ffrag p);
int check_ffrag(struct ffrag p);


int server_receive(int s,char *buffer,int buflen,struct sockaddr *remote,int *rlen);
