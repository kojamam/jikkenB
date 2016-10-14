#include <stdio.h>
main(){
  char c;
  int  ic;
  unsigned char uc;
  while ((ic = getchar()) != EOF) {
      c = ic;
      uc = ic;
      printf("int:%3d, char:%3d, unsigned_char:%3d\n", ic, c, uc);
  }
  c = -108;
  printf("c=%d\n", c);
}

