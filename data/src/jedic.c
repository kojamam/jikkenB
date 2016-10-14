#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_LINE_SIZE 100000

int num;
char *strtable;
int  *jword;
int  *eword;

void print_usage(){
    fprintf(stderr, "Usage: jedic [-n (default: -1)] < word/line.txt\n");
    fprintf(stderr, "  * -n: the n-th column is regarded as a word\n");
    exit(2);
}

off_t fd_length(int fd)
{
  struct stat stat_buf;
  if(fstat(fd, &stat_buf) != -1)
    return(stat_buf.st_size);
  return(-1);
}

void *mmapfile(char *filename, int *n){
    int fd;
    fd = open(filename, O_RDONLY);
    if(fd == -1) {
        fprintf(stderr, "mmapfile: can't open the file: %s\n", filename);
        exit(2);
    }
    *n = fd_length(fd);
    return(mmap(NULL, *n, PROT_READ, MAP_PRIVATE, fd, 0));
}

int delimiter_p(char ch){
    return (ch=='\n' || ch==' ' || ch=='\t' || ch=='\0');
}

int del_skip(char *buf, int p){
    while(delimiter_p(buf[p])) {
        if(buf[p] == '\0') return p;
        else p++;
    }
    return p;
}

int char_skip(char *buf, int p){
    while(!delimiter_p(buf[p])) p++;
    return p;
}

int skip(char *buf, int p){
    p = del_skip(buf, p);
    p = char_skip(buf, p);
    return del_skip(buf, p);
}

void getncol(char *word, char *buf, int col){
    int i;
    int p = 0;
    for(i = 0; i < col-1; i++) p = skip(buf, p);
    i = 0;
    while(!delimiter_p(buf[p])){
        word[i++] = buf[p++];
    }
    word[i] = '\0';
}
    
char *j2e(char *word){
    int i, c;
    int t=0, m=num/2, b=num-1;
/*
printf("S:%s t=%d m=%d b=%d: ", word, t, m, b);
*/

/*  linear search */
/*
    for(i = 0; i < num; i++){
        if(!strcmp(word, strtable+jword[i])) return strtable+eword[i];
    }
*/

/* binary search */
    while(t <= b) {
/*
printf("S:%s\n", strtable+jword[m]);
*/
        c = strcmp(word, strtable+jword[m]);
        if(c == 0) return strtable+eword[m];
        else if(c < 0){
            b = m - 1;
        } else {
            t = m + 1;
        }
        m = (t+b)/2;
    }
    return NULL;
}

main(int ac, char **av){
    int i, n;
    int col = 1;   
    char *engp;
    int len;
    char buf[MAX_LINE_SIZE];
    char eng[MAX_LINE_SIZE];
    char word[MAX_LINE_SIZE];
    void *dat;

    if(ac == 1) {}
    else if(ac == 2) {
        if(av[1][0] == '-') col = atoi(&(av[1][1]));
        else print_usage();
    }
    else print_usage();

/*
    dat = mmapfile("/home/lab/Denjo/myama/jikken/nlp00/dic/jedic.dat", &n);
    dat = mmapfile("/home1/prof/myama/jikken/nlp00/dic/jedic.dat", &n);
    dat = mmapfile("/home/prof/myama/jikken/nlp00/dic/jedic.dat", &n);
*/
    dat = mmapfile("/home/prof/myama/jikken/nlp00/dic/jedic.dat.old", &n);
    num = *((int *)dat);
    jword = ((int *)dat) + 1;
    eword = jword + num;
    strtable = (char *)(eword + num);
    while(fgets(buf, sizeof(buf), stdin) != NULL){
        len = strlen(buf);
        if(buf[len-1] == '\n') buf[len-1] = '\0';
        printf("%s", buf);
        getncol(word, buf, col);
/*
printf("\t(%s)", word);
*/
        if((engp = j2e(word)) != NULL){
            printf("\t%s", engp);
        }
        printf("\n");
    }   
}
