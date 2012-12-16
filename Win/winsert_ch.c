#include <string.h>
#include "twintk.h"

void    tw_insertc(win,ch)
win_t  *win;
int     ch;

{
    /* Update screen image */
    iinsert_ch(&win->image,ch);

    /* Update physical screen */
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    
    if ( TW_FULL_WIDTH_VIEW(win) )
	tinsert_ch(win->terminal,ch);
    else
	tw_redraw_line(win,TW_CUR_LINE(win),TW_CUR_COL(win)-1,TW_COLS(win)-1);
}

