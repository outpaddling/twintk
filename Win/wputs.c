#include "twintk.h"

void 
tw_puts (win_t *win, char *string)

{
    while ( *string != '\0' )
	tw_putc(win,*string++);
}


void 
tw_nputs (win_t *win, char *string, int maxlen)

{
    while ( (*string != '\0') && (maxlen-- > 0) )
	tw_putc(win,*string++);
}

