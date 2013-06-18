#include <stdio.h>
#define MAXBUFLEN 25000

main(){
        char source[MAXBUFLEN + 1];
        FILE *fp = fopen("myfile.txt", "r");
        if (fp != NULL) {
                size_t newLen = fread(source, sizeof(char), MAXBUFLEN, fp);
                if (newLen == 0) {
                        fputs("Error reading file", stderr);
                }
                else {
                        source[++newLen] = '\0'; /* Just to be safe. */
                }
                fclose(fp);
        }

        printf("\nThis is the Buffer:\n%s",source);
}