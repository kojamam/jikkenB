#include <stdio.h>
#include <string.h>

/* Array of skip-bytes-per-initial character.
* http://dev.ariel-networks.com/Members/inoue/utf8-handling
*/
const char * const utf8_skip;
#define utf8_next_char(p) (char *)((p) + utf8_skip[*(unsigned char *)(p)])

static const char utf8_skip_data[256] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};
const char * const utf8_skip = utf8_skip_data;

#define BUFSIZE 10
int main(void)
{
    const char *str = "本日は晴天";

    const char *str_p = str;
    const char *prev  = NULL;

    int  count;
    char buffer[BUFSIZE];

    while (*str_p) {
        prev  = str_p;
        str_p = utf8_next_char(str_p);
        count = str_p - prev;

        strncpy(buffer, prev, count);
        buffer[count] = '\0';
        printf("%s\n", buffer);
    }

    return 0;
}
