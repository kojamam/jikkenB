/*
 * dic.c
 * 辞書を読み込み、メモリに格納
 * 単語の検索
 */
#include "morp.h"

WordPtr wordPtr[MAX_ENTRY_SIZE];

char dicStr[10000000];
char *endDicStr = dicStr;
int  numEnt = 0;

Result initializeDicModule(char* dicFilename){
    loadDic(dicFilename);
    return OK;
}
Result finalizeDicModule(){
    return OK;
}

FILE *fopenRead(char *filename){
    FILE *fileR = fopen(filename, "r");
    if(fileR == NULL){
        fprintf(stderr, "can't open the file: %s\n", filename);
        exit(2);
    }
    return fileR;
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

void printUsage(){
    fprintf(stderr, "Usage: readdic dicfile < text > lookup_result\n");
    exit(2);
}

int loadDic(char *filename){
    char buf[10000], midasi[200], yomi[200], kihon[200], pos[200];
    float uni;
    int  num = 0;
    FILE *dicFile = fopenRead(filename);
    while(fgetline(buf, sizeof(buf), dicFile) != -1){
        sscanf(buf, "%s%s%s%s%f", midasi, yomi, kihon, pos, &uni);
        strcpy(endDicStr, midasi);
        wordPtr[num].midashi = endDicStr;
        endDicStr += strlen(midasi)+1;
        strcpy(endDicStr, yomi);
        wordPtr[num].yomi = endDicStr;
        endDicStr += strlen(yomi)+1;
        strcpy(endDicStr, kihon);
        wordPtr[num].kihon = endDicStr;
        endDicStr += strlen(kihon)+1;
        strcpy(endDicStr, pos);
        wordPtr[num].pos = endDicStr;
        endDicStr += strlen(pos)+1;
        memcpy(endDicStr, &uni, sizeof(float));
        wordPtr[num].uni = (float*)endDicStr;
        endDicStr += sizeof(float);
        num++;
    }
    numEnt = num;
    return num;
}


int lookup(char *word){
    int i;

/*  linear search */
    for(i = 0; i < numEnt; i++){
        if(!strcmp(word, wordPtr[i].midashi)) return i;
    }

    return -1;
}
void printWord(int ent){
    if(ent == -1){
        printf("Unknown word!!\n");
    } else {
        printf("%s\t%s\t%s\t%s\t%f\n", wordPtr[ent].midashi, wordPtr[ent].yomi, wordPtr[ent].kihon, wordPtr[ent].pos, *wordPtr[ent].uni);
    }
}

int main(int ac, char **av){
    initializeDicModule(av[1]);

    char buf[100000];

    if(ac != 2) printUsage();
    while(fgetline(buf, sizeof(buf), stdin) != -1){
        printWord(lookup(buf));
    }
    return 0;
}
