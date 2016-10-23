#include <stdio.h>

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
    TYPE_OTHERS
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
    "その他"
};

/* explde.cに定義されている関数 */
int utf8ToCodepoint(char*, int);
CharType detectCharType(int);
int loadUtfChar(int, char*, CharType*, int);
int utf8charToInt(char*, int);
