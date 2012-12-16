#include <ctype.h>
#include "twintk.h"

int     tw_get_seq(win,sequence,max_len)
win_t  *win;
char    *sequence;
size_t  max_len;

{
    int     ch;
    
    /* Make sure terminal cursor is in the right place */
    TW_SYNC_CURSOR(win);
    TW_SYNC_COLORS(win);
    
    /* Read character */
    ch = tgetseq(win->terminal,sequence,max_len);
    if ( TTY_LFLAG_ON(win->terminal,ECHO) && isascii(ch) && isprint(ch) ) 
	iputc(&win->image,ch);
    return(ch);
}

