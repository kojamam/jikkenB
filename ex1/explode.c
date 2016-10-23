#include "explode.h"

int main() {
    int c;

    while((c = getchar()) != EOF){
        char utf8Char[5];
        CharType charType = TYPE_OTHERS;

        if(c >= 0x0 && c <= 0x7F){
            loadUtfChar(c, utf8Char, &charType, 1);
        }else if(c >= 0xC2 && c <= 0xDF){
            loadUtfChar(c, utf8Char, &charType, 2);
        }else if(c >= 0xE0 && c <= 0xEF){
            loadUtfChar(c, utf8Char, &charType, 3);
        }else if(c >= 0xF0 && c <= 0xF7){
            loadUtfChar(c, utf8Char, &charType, 4);
        }else{
            fprintf(stderr, "Input text isn't ncoded in UTF-8 code.\n");
            return 1;
        }

        if(charType == TYPE_CONTROL){
            continue;
        }

        printf("%s %s\n", utf8Char, charTypeName[charType]);
    }

    printf("\n");
    return 0;
}

/* コードポイントの作成
 * @param char *utf8Char
 * @param int numByte
 * @return int unicode code point
 */
int utf8ToCodepoint(char *utf8Char, int numByte) {
    int utf8BitShift =  6;
    int utf8ByteMuskHead[] = {0x7F, 0x1F, 0x0F, 0x07};
    int utf8ByteMusk = 0x3F;
    int codePoint;

    codePoint = (utf8Char[0] & utf8ByteMuskHead[numByte-1]) << utf8BitShift*(numByte-1);

    for(int i=1; i<numByte; ++i){
        codePoint |= (utf8Char[i] & utf8ByteMusk) << utf8BitShift*(numByte-i-1);
    }

    return codePoint;
}

/* 文字種を判定
 * @param int codePoint
 * @return CharType 文字種
 */
CharType detectCharType(int codePoint) {

    CharType charType = TYPE_OTHERS;

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
        charType = TYPE_OTHERS;
    }

    return charType;
}

/* 文字種を判定
 * @param int c 0番目のバイト
 * @param char *utf8Char UTF-8バイト列を入れる配列
 * @param int numByte 文字のバイト数
 * @return CharType 文字種
 */
int loadUtfChar(int c, char *utf8Char, CharType *charType, int numByte){
    int i = 0;
    int checkMusk = 0xC0;
    int checkBit = 0x80;

    utf8Char[i++] = c;

    while(i < numByte){
        c  = getchar();
        if ((c & checkMusk) == checkBit) {
            utf8Char[i++] = c;
        }else{
            fprintf(stderr, "Input text isn't ncoded in UTF-8 code.\n");
        }
    }

    utf8Char[i] = '\0';

    utf8charToInt(utf8Char, numByte);

    *charType = detectCharType(utf8ToCodepoint(utf8Char, numByte));

    return 0;
}

/* UTF-8文字をintに格納
* @param char *utf8Char
* @param int numByte
* @return int utf8 in int
 */
int utf8charToInt(char *utf8Char, int numByte) {
    int utf8Int = 0;

    utf8Int = utf8Char[0] & 0xFF;
    // printf("%8x ", utf8Int);


    for(int i=1; i<numByte; ++i){
        utf8Int <<= 8;
        utf8Int |= utf8Char[i] & 0xFF;
    }
    printf("%8x ", utf8Int);
    return utf8Int;
}
