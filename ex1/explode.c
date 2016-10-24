/*
 * explode.c
 * UTF-8文字列を一文字ごとに切り分ける
 */
#include "morp.h"

/* 仮 テスト用 */
int main(int argc, char const *argv[]) {
    Utf8Char utf8Char;

    while(1){
        explode(&utf8Char);
        if(utf8Char.c[0] != EOF && utf8Char.type != TYPE_CONTROL){
            printf("%s %s\n", utf8Char.c, charTypeName[utf8Char.type]);
        }else{
            break;
        }
    }

    printf("\n");
    return 0;
}

/* 標準入力からUTF-8を1文字切り出す。
 * @param char *utf8Char
 * @param int numByte
 * @return int unicode code point
 */
Result explode(Utf8Char *utf8Char) {

    utf8Char->c[0] = getchar();
    utf8Char->numByte = 0;
    utf8Char->type = TYPE_UNKNOWN;

    if(utf8Char->c[0] == EOF){
        return OK;
    }
    int c = utf8Char->c[0] & 0xFF;

    if(utf8Char->c[0] >= 0x0 && utf8Char->c[0] <= 0x7F){
        utf8Char->numByte = 1;
    }else if(c >= 0xC2 && c <= 0xDF){
        utf8Char->numByte = 2;
    }else if(c >= 0xE0 && c <= 0xEF){
        utf8Char->numByte = 3;
    }else if(c >= 0xF0 && c <= 0xF7){
        utf8Char->numByte = 4;
    }else{
        fprintf(stdout, "Input text isn't ncoded in UTF-8 code.1\n");
        return NG;
    }

    loadUtfChar(utf8Char);

    if(utf8Char->type == TYPE_CONTROL){
        return NG;
    }

    return OK;
}

/* コードポイントの作成
 * @param char *utf8Char
 * @param int numByte
 * @return int unicode code point
 */
int utf8ToCodepoint(Utf8Char *utf8Char) {
    int utf8BitShift =  6;
    int utf8ByteMuskHead[] = {0x7F, 0x1F, 0x0F, 0x07};
    int utf8ByteMusk = 0x3F;
    int codePoint;

    codePoint = (utf8Char->c[0] & utf8ByteMuskHead[utf8Char->numByte-1]) << utf8BitShift*(utf8Char->numByte-1);

    for(int i=1; i<utf8Char->numByte; ++i){
        codePoint |= (utf8Char->c[i] & utf8ByteMusk) << utf8BitShift*(utf8Char->numByte-i-1);
    }

    return codePoint;
}

/* 文字種を判定
 * @param int codePoint
 * @return CharType 文字種
 */
CharType detectCharType(int codePoint) {

    CharType charType = TYPE_UNKNOWN;

    if(codePoint >= 0x3040 && codePoint <= 0x309F){
        charType = TYPE_HIRAGANA;
    }else if(codePoint >= 0x30A0 && codePoint <= 0x30FA){
        charType =TYPE_KATAKANA;
    }else if(codePoint >= 0x3000 && codePoint <= 0x303F){
        charType =TYPE_KUTOHTEN;
    }else if(codePoint >= 0x0020 && codePoint <= 0x007E){
        charType = TYPE_ASCII;
    }else if(codePoint == 0x30FC){
        charType = TYPE_CHOON;
    }else if(codePoint >= 0xFF10 && codePoint <= 0xFF19){
        charType = TYPE_NUM;
    }else if((codePoint >= 0xFF21 && codePoint <= 0xFF3A) || (codePoint >= 0xFF41 && codePoint <= 0xFF5A)){
        charType = TYPE_MULTI_ALPHABET;
    }else if((codePoint >= 0x3400 && codePoint <= 0x4DBF) || (codePoint >= 0x4E00 && codePoint <= 0x9FB7)){
        charType = TYPE_KANJI;
    }else if((codePoint >= 0x0001 && codePoint <= 0x001F) || codePoint == 0x7F){
        charType = TYPE_CONTROL;
    }else if(codePoint == 0x20 || codePoint == 0x3000){
        charType = TYPE_SPACE;
    }else{
        charType = TYPE_UNKNOWN;
    }

    return charType;
}

/* 文字種を判定
 * @param int c 0番目のバイト
 * @param char *utf8Char UTF-8バイト列を入れる配列
 * @param int numByte 文字のバイト数
 * @return CharType 文字種
 */
Result loadUtfChar(Utf8Char* utf8Char){
    int i;
    int c;
    int checkMusk = 0xC0;
    int checkBit = 0x80;


    i = 1;
    while(i < utf8Char->numByte){
        c  = getchar();
        if ((c & checkMusk) == checkBit) {
            utf8Char->c[i++] = c;
        }else{
            fprintf(stdout, "Input text isn't ncoded in UTF-8 code.2\n");
        }
    }

    utf8Char->c[i] = '\0';

    utf8Char->type = detectCharType(utf8ToCodepoint(utf8Char));

    return OK;
}
