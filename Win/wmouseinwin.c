#include "twintk.h"

int 
tw_mouse_in_win (win_t *win, win_t *event_win, event_t *event)

{
    int     term_line = event->mouse_line + TW_SCREEN_LINE(event_win),
	    term_col = event->mouse_col + TW_SCREEN_COL(event_win),
	    in_win;
    
    in_win = (term_line >= TW_SCREEN_LINE(win)) &&
	    (term_line < TW_SCREEN_LINE(win)+TW_LINES(win)) &&
	    (term_col >= TW_SCREEN_COL(win)) &&
	    (term_col < TW_SCREEN_COL(win)+TW_COLS(win));
    return in_win;
}

