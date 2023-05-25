#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "twintk.h"

void 
tw_clear_win (win_t *win)

{
    int     line;

    /* Clear buffer */
    iclear_image(&win->image);
    
    /* Clear screen segment */
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    
    if ( TW_FULL_SCREEN_VIEW(win) )   /* View window is full screen */
	tclear_screen(win->terminal);
    else if ( TW_FULL_WIDTH_VIEW(win) )    /* View window is full width of screen */
    {
	for (line = 0; line < win->view_lines; ++line)
	{
	    tw_move_to(win, line, 0);
	    tw_clear_to_eol(win);
	}
    }
    else    /* View window is not full width */
    {
	tw_redraw(win,0,0,TW_LINES(win)-1,TW_COLS(win)-1);
    }

    /* Home cursor */
    tw_move_to(win,0,0);
}

