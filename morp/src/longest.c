#include "morp.h"


int main(int argc, char *argv[]) {

    int numEnt = 0;
    Utf8Char *utf8Chars;
    char word[MAX_WORDLEN*4];
    int i = 0;
    int numChars = 0;
    int start = 0;
    int len = MAX_WORDLEN;
    int ent;

    numEnt = initializeDicModule(argv[1]);

    utf8Chars = (Utf8Char*)malloc(sizeof(Utf8Char)*MAX_INPUT_LEN);

    while(1){
       explode(&utf8Chars[i]);
    //    printf("%s\n", utf8Chars[i].c);
       if(utf8Chars[i].c[0] == EOF){
           break;
       }
       if(utf8Chars[i].type == TYPE_CONTROL){
           i--;
       }
       i++;
   }

   numChars = i;
   if(len > numChars){
       len = numChars;
   }

   while(len > 0){
       for (; len > 0; --len) {
           combineChars(utf8Chars, word, start, len);
           if((ent = lookup(word, 0, numEnt-1)) != -1){
               start += len;
               if(numChars-start > MAX_WORDLEN){
                   len = MAX_WORDLEN;
               }else{
                   len = numChars-start;
               }
               printWord(ent);
               break;
           }
       }
       if(ent == -1){
        //    combineChars(utf8Chars, word, start, 2);
           printf("%s\t定義なし\n", word);
           start += 1;
           if(numChars-start > MAX_WORDLEN){
               len = MAX_WORDLEN;
           }else{
               len = numChars-start;
           }
       }
   }

   free(utf8Chars);

    return 0;
}
