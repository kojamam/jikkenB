#include <stdio.h>
#include <stdlib.h>
main(){
  int  c;
  unsigned char euc_ch[3];
  while ((c = getchar()) != EOF) {
      if(c >= 128) {
          euc_ch[0] = c;
          if((c = getchar()) != EOF){
              euc_ch[1] = c;
              euc_ch[2] = '\0'; /* C言語における文字列の終端を表す文字 */
          } else {
              fprintf(stderr, "input text isn't encoded in EUC code\n");
              exit(2);
          }
      } else if(c == '\n') {
          continue;
      } else {
          euc_ch[0] = c;
          euc_ch[1] = '\0';
      }
      printf("%s\n", euc_ch);
  }
}

