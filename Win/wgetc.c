#include <ctype.h>
#include "twintk.h"

int     tw_getc(win)
win_t  *win;

{
    int     ch;
    
    /* Make sure terminal cursor is in the right place */
    TW_SYNC_CURSOR(win);
    TW_SYNC_COLORS(win);
    
    /* Read character */
    ch = tgetc(win->terminal);
    if ( TTY_LFLAG_ON(win->terminal,ECHO) && isascii(ch) && isprint(ch) ) 
	iputc(&win->image,ch);
    return(ch);
}

