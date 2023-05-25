#include <stdio.h>
#include <string.h>
#include "twintk.h"

void 
tw_clear_to_eol (win_t *win)

{
    /* Clear buffer to eol */
    iclear_eol(&win->image);
    
    /* Clear to end of screen line */
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    TW_SYNC_CURSOR(win);
    
    if ( ( TW_FULL_WIDTH_VIEW(win)) && ( TCAN_CLEAR_TO_EOL(win->terminal) ) )
    {
	/* Clear terminal to eol */
	tclear_eol(win->terminal);
    }
    else
    {
	tw_redraw_line(win,TW_CUR_LINE(win),TW_CUR_COL(win),TW_COLS(win)-1);
    }
}

