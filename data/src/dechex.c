#include <stdio.h>
main(){
    int c;
    while ((c = getchar()) != EOF) {
        printf("%5d %5x\n", c, c);
    }
}

