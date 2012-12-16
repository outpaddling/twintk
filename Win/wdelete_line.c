#include "twintk.h"

void    tw_del_line(win)
win_t   *win;

{
    iscroll_forward(&win->image,TW_CUR_LINE(win),TW_LINES(win)-1);

    if ( TW_FULL_WIDTH_VIEW(win) )
    {
	TW_SYNC_COLORS(win);
	tw_sync_modes(win);
	if ( TW_FULL_SCREEN_VIEW(win) )
	{
	    tdelete_line(win->terminal);
	}
	else
	{
	    int    save_top,save_bottom,line,col;
	    
	    save_top = TSCROLL_TOP(win->terminal);
	    save_bottom = TSCROLL_BOTTOM(win->terminal);
	    tchange_scroll_region(win->terminal,TW_SCREEN_CUR_LINE(win),
		win->screen_line + TW_LINES(win) - 1);
	    tscroll_forward(win->terminal);
	    tchange_scroll_region(win->terminal,save_top,save_bottom);
	    /* Make sure bottom line is cleared */
	    TW_FIND_CURSOR(win,line,col);
	    tw_move_to(win,TW_LINES(win)-1,0);
	    tw_clear_to_eol(win);
	    tw_move_to(win,line,col);
	}
    }
    else
	tw_redraw(win,TW_CUR_LINE(win),0,TW_LINES(win)-1,TW_COLS(win)-1);
}

