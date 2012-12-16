#include "twintk.h"

void    tw_clear_to_eow(win)
win_t   *win;

{
    int    l,start_line,start_col;
    
    TW_FIND_CURSOR(win,start_line,start_col);
    
    /* If window bottom is screen bottom */
    if ( TW_BOTTOM_OF_SCREEN(win) && TW_FULL_WIDTH_VIEW(win) )
    {
	iclear_eos(&win->image);
	tclear_eos(win->terminal);
    }
    else
    {
	/* Clear from cursor to eol */
	tw_clear_to_eol(win);
	
	/* Clear remaining lines */
	for (l=TW_CUR_LINE(win)+1; l < TW_VIEW_LINES(win); ++l)
	{
	    tw_move_to(win,l,0);
	    tw_clear_to_eol(win);
	}
    }
    tw_move_to(win,start_line,start_col);
}

