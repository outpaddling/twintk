#include "twintk.h"

void    tw_scroll_forward(win)
win_t   *win;

{
    iscroll_forward(&win->image,TW_SCROLL_TOP(win),TW_SCROLL_BOTTOM(win));
    ptw_scroll_forward(win);
}


void    ptw_scroll_forward(win)
win_t   *win;

{
    int top,bottom,reset;
    
    if ( TW_FULL_WIDTH_VIEW(win) )
    {
	TW_SYNC_COLORS(win);
	tw_sync_modes(win);
	top = TSCROLL_TOP(win->terminal);
	bottom = TSCROLL_BOTTOM(win->terminal);
	reset = ( (top != win->screen_line) ||
	    (bottom != win->screen_line+TW_LINES(win)-1) );
	if ( reset )
	    tchange_scroll_region(win->terminal,win->screen_line,win->screen_line+TW_LINES(win)-1);
	tscroll_forward(win->terminal);
	if ( reset )
	    tchange_scroll_region(win->terminal,top,bottom);
    }
    else
	TW_RESTORE_WIN(win);
}

