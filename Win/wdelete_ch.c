#include <string.h>
#include "twintk.h"

void 
tw_del_ch (win_t *win)

{
    /* Update screen image */
    idelete_ch(&win->image);

    /* Update physical screen */
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    
    if ( TW_FULL_WIDTH_VIEW(win) )
    {
	TW_SYNC_CURSOR(win);
	tdelete_ch(win->terminal);
    }
    else
	tw_redraw_line(win,TW_CUR_LINE(win),TW_CUR_COL(win),TW_COLS(win)-1);
}

