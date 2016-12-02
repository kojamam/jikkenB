#ifndef __INCLUDE_MORP_H__
#define __INCLUDE_MORP_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* 単語の最大文字数 */
#define MAX_WORDLEN 16

/* 辞書の最大エントリー数 */
#define MAX_ENTRY_SIZE 150000

/* 最大読み込み文字数 */
#define MAX_INPUT_LEN 10000000

#define TRIE_NUM_NODE 500000 //実測で372197ノードまで使った

/* 関数の終了ステータス */
typedef enum Result{
    OK,
    NG
} Result;

/* 文字種 */
typedef enum CharType{
    TYPE_ASCII,
    TYPE_HIRAGANA,
    TYPE_KATAKANA,
    TYPE_CHOON,
    TYPE_NUM,
    TYPE_MULTI_ALPHABET,
    TYPE_KANJI,
    TYPE_KUTOHTEN,
    TYPE_SPACE,
    TYPE_CONTROL,
    TYPE_EMOJI,
    TYPE_UNKNOWN
} CharType;

static char *charTypeName[] = {
    "ASCII",
    "ひらがな",
    "カタカナ",
    "長音",
    "数字",
    "2byteアルファベット",
    "漢字",
    "句読点",
    "スペース",
    "制御文字",
    "絵文字",
    "その他"
};

/* utf8の文字 */
typedef struct Utf8Char{
    char c[5]; //utf8文字
    int numByte; //バイト数
    CharType type; //文字種
}Utf8Char;

/* 単語ポインタ */
typedef struct WordPtr{
    char* midashi; //単語
    char* yomi; //よみがな
    char* kihon; //基本形
    char* pos; //品詞
    float* uni; //単語コスト
}WordPtr;



/* chars.cに定義されている関数 */
Result explode(Utf8Char*);
int utf8ToCodepoint(Utf8Char *utf8Char);
CharType detectCharType(int);
Result loadUtfChar(Utf8Char*);
int combineChars(Utf8Char*, char*, int, int);

/* dic.cに定義されている関数群 */
int initializeDicModule(char*);
Result finalizeDicModule();
int loadDic(char *filename);
FILE *fopenRead(char *filename);
int fgetline(char *buf, size_t size, FILE *stream);
void printUsage();
int lookup(char *word, int min, int max);
int lookupLinear(char *);
void printWord(int ent);
double getCost(int ent);
Result initTrie();
int searchEmptyNode();
Result insertTrie(char *word, int ent);
int lookupTrie(char *word);

#endif
