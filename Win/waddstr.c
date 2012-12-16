#include <string.h>
#include <ctype.h>
#include "twintk.h"

void    tw_add_string(win, string, maxlen)
win_t   *win;
char   *string;
int maxlen;

{
    int     c;

    for (c = 0; (c < maxlen) && (*string != '\0'); ++c)
	tw_putc(win, *string++);
}

