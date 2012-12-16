#include "twintk.h"

void    tw_redraw(win, topline, leftcol, bottomline, rightcol)
win_t  *win;
int     topline, leftcol, bottomline, rightcol;

{
    int     line, start_line, start_col, auto_scroll = TW_AUTO_SCROLL(win);

    TW_FIND_CURSOR(win, start_line, start_col);
    TW_UNSET_AUTO_SCROLL(win);

    /* Redraw all but bottom line on terminal */
    for (line = topline; (line <= bottomline) &&
	 (line + win->screen_line < TLINES(win->terminal) - 1); ++line)
	tw_redraw_line_leave_cursor(win, line, leftcol, rightcol);

    /* Redraw all but last char on bottom of terminal to avoid scroll */
    if (line == bottomline)
    {
	if (win->screen_col + TW_COLS(win) == TCOLS(win->terminal))
	    tw_redraw_line_leave_cursor(win, line, leftcol, rightcol - 1);
	else
	    tw_redraw_line_leave_cursor(win, line, leftcol, rightcol);
    }
    tw_move_to(win, start_line, start_col);
    if (auto_scroll)
	TW_SET_AUTO_SCROLL(win);
}
