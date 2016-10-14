#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_ENTRY_SIZE 150000

char *Tmidasi[MAX_ENTRY_SIZE];
char *Tyomi[MAX_ENTRY_SIZE];
char *Tkihon[MAX_ENTRY_SIZE];
char *Tpos[MAX_ENTRY_SIZE];
float Tuni[MAX_ENTRY_SIZE];
char Tstr[10000000];
char *endTstr = Tstr;
int  Tnum = 0;

FILE *fopen_read(char *filename){
    FILE *istream = fopen(filename, "r");
    if(istream == NULL){
        fprintf(stderr, "can't open the file: %s\n", filename);
        exit(2);
    }
    return istream;
}

int fgetline(char *buf, size_t size, FILE *stream){
    int len;
    if(fgets(buf, size, stream) == NULL) return -1;
    len = strlen(buf);
    if(len == 0) return len;
    if(buf[len-1] == '\n'){
        buf[len-1] = '\0';
        return len-1;
    } else {
        return len;
    }
}

void print_usage(){
    fprintf(stderr, "Usage: readdic dicfile < text > lookup_result\n");
    exit(2);
}

int read_dic(char *filename){
    char buf[10000], midasi[200], yomi[200], kihon[200], pos[200];
    int  num = 0;
    FILE *dicstream = fopen_read(filename);
    while(fgetline(buf, sizeof(buf), dicstream) != -1){
        sscanf(buf, "%s%s%s%s%f", midasi, yomi, kihon, pos, &Tuni[num]);
        strcpy(endTstr, midasi);
        Tmidasi[num] = endTstr;
        endTstr += strlen(midasi)+1;
        strcpy(endTstr, yomi);
        Tyomi[num] = endTstr;
        endTstr += strlen(yomi)+1;
        strcpy(endTstr, kihon);
        Tkihon[num] = endTstr;
        endTstr += strlen(kihon)+1;
        strcpy(endTstr, pos);
        Tpos[num] = endTstr;
        endTstr += strlen(pos)+1;
        num++;
    }
    Tnum = num;
    return num;
}


int lookup(char *word){
    int i;

/*  linear search */
    for(i = 0; i < Tnum; i++){
        if(!strcmp(word, Tmidasi[i])) return i;
    }

    return -1;
}

void print_word(int ent){
    char unkword[10];
    if(ent == -1){
        printf("Unknown word!!\n");
    } else {
        printf("%s\t%s\t%s\t%s\n", Tmidasi[ent], Tyomi[ent], Tkihon[ent],Tpos[ent]);
    }
}

main(int ac, char **av){
    char buf[100000];
    int i;
    int ent;
    if(ac != 2) print_usage();
    read_dic(av[1]);
    while(fgetline(buf, sizeof(buf), stdin) != -1){
        print_word(lookup(buf));
    }   
}
