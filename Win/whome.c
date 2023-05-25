#include "twintk.h"

void 
tw_home_win (win_t *win)

{
    if ( (win->screen_line == 0) && (win->screen_col == 0) )
    {
	thome(win->terminal);
	IHOME(&win->image);
    }
    else
	tw_move_to(win,0,0);
}

