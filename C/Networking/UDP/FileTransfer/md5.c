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

#include "md5.h"


char *calculate_md5(void *buf, ssize_t buflen){
   unsigned char md_value[EVP_MAX_MD_SIZE];
   unsigned int md_len;
   char *retval;
   int i;
   EVP_MD_CTX mdctx;
   EVP_DigestInit(&mdctx, EVP_md5());
   EVP_DigestUpdate(&mdctx, buf, (size_t) buflen);
   EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
   EVP_MD_CTX_cleanup(&mdctx);
   retval=malloc(sizeof(char)*2*MD5_SIZE);
   for(i = 0; i < MD5_SIZE; i++){
      sprintf (&retval[2*i],"%02x", md_value[i]);
   }
   return(retval);
}