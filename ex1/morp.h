#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* 単語の最大文字数 */
#define MAX_WORDLEN 50

/* 辞書の最大エントリー数 */
#define MAX_ENTRY_SIZE 150000

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
    TYPE_UNKNOWN
} CharType;

char *charTypeName[] = {
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
    "その他"
};

/* 品詞 */
typedef enum POS{
    POS_DOSHI,
    POS_KEIYOSHI,
    POS_KEIYODOSHI,
    POS_MEISHI,
    POS_RENTAISHI,
    POS_FUKUSHI,
    POS_SETSUZOKUSHI,
    POS_KANDOSHI,
    POS_JOSHI,
    POS_JODOSHI
}POS;

char *posName[] = {
    "動詞",
    "形容詞",
    "形容動詞",
    "名詞",
    "連体詞",
    "副詞",
    "接続詞",
    "感動詞",
    "助詞",
    "助動詞"
};

/* utf8の文字 */
typedef struct Utf8Char{
    char c[5];
    int numByte;
    CharType type;
}Utf8Char;

/* 単語 */
typedef struct WordPtr{
    char* midashi; //単語
    char* yomi; //よみがな
    char* kihon; //基本形
    char* pos; //品詞
    float* uni; //単語コスト
}WordPtr;



/* explde.cに定義されている関数 */
Result explode(Utf8Char*);
int utf8ToCodepoint(Utf8Char *utf8Char);
CharType detectCharType(int);
Result loadUtfChar(Utf8Char*);

/* dic.cに定義されている関数群 */
Result initializeDicModule(char*);
Result finalizeDicModule();
int loadDic(char *filename);
FILE *fopenRead(char *filename);
int fgetline(char *buf, size_t size, FILE *stream);
void printUsage();
int lookup(char *word);
void printWord(int ent);
