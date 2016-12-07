/*
 *  Transformation program: 'jedic.txt' -> 'jedic.dat'
 *     jedic.txt : text file of edict (j-e dictionary)
 *     jedic.dat : data file for 'jedic' command
 *
 *  Filename: mkjedic.c
 *  Location: /home/lab/Patrec/myama/jikken/nlp00/src/mkjedic.c
 *  Date: Apr. 2000.
 *  Mikio Yamamoto
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int jword[100000];
int eword[100000];
char strtable[10000000];

int end = 0;

int delimiter_p(char ch){
    return (ch=='\n' || ch==' ' || ch=='\t' || ch=='\0');
}

int jstore(char *buf, int p){
    while(!delimiter_p(buf[p])){
        strtable[end++] = buf[p++];
    }
    strtable[end++] = '\0';
    return p;
}

int estore(char *buf, int p){
    while(buf[p] != '\0'){
        strtable[end++] = buf[p++];
    }
    strtable[end++] = '\0';
    return p;
}

/*
int skip(char *buf, int p){
    while(delimiter_p(buf[p])) p++;
    while(!delimiter_p(buf[p])) p++;
    while(delimiter_p(buf[p])) p++;
    return p;
}
*/
int skip(char *buf, int p){
    while(buf[p] != '/') p++;
    return p;
}
        
main(int ac, char **av){
    int p, num=0;
    int fd;
    int len;
    char buf[100000];
    if(ac != 2) {
         fprintf(stderr, "Usage: mkjedic jedic.dat < jedic.txt\n");
         exit(2);
    }
    fgets(buf, sizeof(buf), stdin);
    while(fgets(buf, sizeof(buf), stdin) != NULL){
        len = strlen(buf);
        if(buf[len-1] == '\n') buf[len-1] = '\0';
        jword[num] = end;
        p = jstore(buf, 0);
        eword[num++] = end;
        p = skip(buf, p);
        estore(buf, p);
    }
    fd = creat(av[1], 0755);
    write(fd, &num, sizeof(int));
    write(fd, jword, num*sizeof(int));
    write(fd, eword, num*sizeof(int));
    write(fd, strtable, end);
}

