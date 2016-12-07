/*
 * dic.c
 * 辞書を読み込み、メモリに格納
 * 単語の検索
 */
#include "morp.h"

WordPtr dicPtr[MAX_ENTRY_SIZE];
char dicStr[MAX_ENTRY_SIZE*50]; //辞書の格納メモリ
char *endDicStr = dicStr;
int  numEnt = 0;
int trie[TRIE_NUM_NODE][0xFF]; //トライの疎行列
int trieWord[TRIE_NUM_NODE]; //ノードの単語 => emptyのときは-2, 単語じゃないときは-1
int emptyNode = 0;

/* dicモジュールを初期化
 * @param char* docFilename 辞書のファイル名
 * @return Result
 */
int initializeDicModule(char* dicFilename){
    initTrie();
    return loadDic(dicFilename);
}

/* dicモジュールの最終処理
 * @return Result
 */
Result finalizeDicModule(){
    return OK;
}

/* ファイルをリードモードでオープン
 * @param char* filename ファイル名
 * @return FILE オープンしたファイル
 */
FILE *fopenRead(char *filename){
    FILE *fileR = fopen(filename, "r");
    if(fileR == NULL){
        fprintf(stderr, "can't open the file: %s\n", filename);
        exit(2);
    }
    return fileR;
}

/* ファイルから1行読み込む
 * @param char *buf 読み込んだ文字列を格納する
 * @param size_t size 読み込む最大文字数
 * @param FILE 読み込み元ファイル
 * @return int 読み込んだバイト数
 */
int fgetline(char *buf, size_t size, FILE *stream){
    int len;
    if(fgets(buf, size, stream) == NULL) return -1;
    if((len = strlen(buf)) == 0){
        return len;
    }
    if(buf[len-1] == '\n'){
        buf[len-1] = '\0';
        return len-1;
    } else {
        return len;
    }
}

/* 使用方法を表示
 */
void printUsage(){
    fprintf(stderr, "Usage: readdic dicfile < text > lookup_result\n");
    exit(2);
}

/* ファイルから辞書を読み込んでメモリに格納
 * @param char *filename 読み込む辞書ファイルの名前
 * @return 読み込んだ単語数
 */
int loadDic(char *filename){
    char buf[10000], midasi[200], yomi[200], kihon[200], pos[200];
    float uni;
    int  i = 0;
    FILE *dicFile = fopenRead(filename);
    while(fgetline(buf, sizeof(buf), dicFile) != -1){
        sscanf(buf, "%s%s%s%s%f", midasi, yomi, kihon, pos, &uni);
        strcpy(endDicStr, midasi);
        dicPtr[i].midashi = endDicStr;
        endDicStr += strlen(midasi)+1;
        strcpy(endDicStr, yomi);
        dicPtr[i].yomi = endDicStr;
        endDicStr += strlen(yomi)+1;
        strcpy(endDicStr, kihon);
        dicPtr[i].kihon = endDicStr;
        endDicStr += strlen(kihon)+1;
        strcpy(endDicStr, pos);
        dicPtr[i].pos = endDicStr;
        endDicStr += strlen(pos)+1;
        memcpy(endDicStr, &uni, sizeof(float));
        dicPtr[i].uni = (float*)endDicStr;
        endDicStr += sizeof(float);

        insertTrie(dicPtr[i].midashi, i);

        i++;
    }
    numEnt = i;

    return i;
}

/*
 * トライ木を初期化
 * @return 終了ステータス
 */
Result initTrie(){
    //初期化
    for (size_t i = 0; i < TRIE_NUM_NODE; i++) {
        for (size_t j = 0; j < 256; j++) {
            trie[i][j] = -1;
        }
        trieWord[i] = -2;
    }
    trieWord[0] = -1;

    return OK;
}

/*
 * トライ木に単語を挿入
 * @param char *word 挿入する単語
 * @param int ent エントリー番号
 * @return 終了ステータス
 */
Result insertTrie(char *word, int ent){
    int node = 0;
    for (size_t i = 0; i < strlen(word); i++) {
        if(trie[node][(unsigned char)word[i]] >= 0){
            node = trie[node][(unsigned char)word[i]];
        }else{
            trieWord[emptyNode] = -1;
            trie[node][(unsigned char)word[i]] = emptyNode;
            node = emptyNode++;
        }

    }

    trieWord[node] = ent;

    return OK;
}

/* 二分探索で単語を見つける(再帰)
 * @param char *word 探す文字列
 * @return int 見つけたエントリー。なければ-1
 */
int lookupTrie(char *word){
    int node = 0;
    for (size_t i = 0; i < strlen(word); i++) {
        if(trie[node][(unsigned char)word[i]] >= 0){
            node = trie[node][(unsigned char)word[i]];
        }else{
            return -1;
        }
    }
    return trieWord[node];
}

/* 二分探索で単語を見つける(再帰)
 * @param char *word 探す文字列
 * @param int min 探索する最小エントリー
 * @param int max 探索する最大エントリー
 * @return int 見つけたエントリー。なければ-1
 */
int lookup(char *word, int min, int max){
    if(min > max){
        return -1;
    }

    int ent = (max - min)/2 + min;
    int diff = strcmp(word, dicPtr[ent].midashi);

    // printf("%d %d %d\n", min, ent, max);
    if(diff > 0){
        return lookup(word, ent+1, max);
    }else if(diff < 0){
        return lookup(word, min, ent-1);
    }else{
        return ent;
    }
}

/* 線形探索で単語を見つける(再帰)
 * @param char *word 探す文字列
 * @return int 見つけたエントリー。なければ-1
 */
int lookupLinear(char *word){

    for(int ent=0; ent<numEnt; ++ent){
        if(strcmp(word, dicPtr[ent].midashi) == 0){
            return ent;
        }
    }

    return -1;
}


/* 単語の表示
 * @param int ent 表示する単語のエントリー
 */
void printWord(int ent){
    if(ent == -1){
        printf("定義なし\n");
    } else {
        printf("%s\t%s\t%s\t%s\n", dicPtr[ent].midashi, dicPtr[ent].yomi, dicPtr[ent].kihon, dicPtr[ent].pos);
    }
}

double getCost(int ent){
    return *dicPtr[ent].uni;
}

#ifdef DIC_DEBUG
int main(int ac, char **av){
    initializeDicModule(av[1]);

    char buf[100000];

    if(ac != 2) printUsage();
    while(fgetline(buf, sizeof(buf), stdin) != -1){
        printWord(lookup(buf, 0, numEnt-1));
    }
    return 0;
}
#endif
