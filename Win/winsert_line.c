#include "twintk.h"
	    
void    tw_insert_line(win)
win_t   *win;

{
    int     line,col;
    
    iscroll_reverse(&win->image,TW_CUR_LINE(win),TW_LINES(win)-1);

    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    if ( TW_FULL_WIDTH_VIEW(win) )
    {
	TFIND_CURSOR(win->terminal,line,col);
	if ( TW_FULL_SCREEN_VIEW(win) )
	{
	    tinsert_line(win->terminal);
	}
	else
	{
	    int    save_top,save_bottom;
	    
	    save_top = TSCROLL_TOP(win->terminal);
	    save_bottom = TSCROLL_BOTTOM(win->terminal);
	    tchange_scroll_region(win->terminal,TW_SCREEN_CUR_LINE(win),
		TW_SCREEN_LINE(win) + TW_LINES(win) - 1);
	    tscroll_reverse(win->terminal);
	    tchange_scroll_region(win->terminal,save_top,save_bottom);
	}
	tmove_to(win->terminal,line,col);
    }
    else
	tw_redraw(win,TW_CUR_LINE(win),0,TW_LINES(win)-1,TW_COLS(win)-1);
}

