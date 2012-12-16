#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "twintk.h"

/*
 *  Write character ch to window win.
 *  This function writes character `ch' into the window 'win', and
 *  also to the corresponding image.  The character is immediately written
 *  to the output stream, but may or may not immediately appear on screen
 *  due to stream buffering.  Calling fflush(terminal->fpout) will cause
 *  immediate display of all characters in the stream buffer, as will
 *  calling any TWIN function that performs an fflush(), such as tw_getc().
 */
 
void    tw_putc(win_t   *win, int ch)

{
    static int    start_line,start_col,modes;
    static int         acs_missing;

    acs_missing = (ch & ACS_MISSING) && (TW_CUR_MODES(win) & ACS_MODE);
    if ( acs_missing )
    {
	modes = TW_CUR_MODES(win);
	TW_SET_MODES(win,modes&~ACS_MODE);
	ch &= ~ACS_MISSING;     /* Strip off ACS_MISSING flag */
    }

    /* Check for insert mode */
    if ( TW_INSERT_MODE_ON(win) )
    {
	tw_insertc(win,ch);
	return;
    }

    /* Set terminal color if necessary */
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    TW_SYNC_CURSOR(win);

    /* Get starting cursor position */
    TW_FIND_CURSOR(win,start_line,start_col);
    
    /* Update image */
    iputc(&win->image,ch);

    /* Print character on screen */
    if ( TW_CURSOR_IN_VIEW(win) && (isprint(ch) || (ch > 127)) )
	tputc(win->terminal,ch);
    
    /* Scroll if necessary */
    /* Better yet...
    switch(ch)
    {
	case    '\n':
	case    '\r':
	default:
    }
    */
    
    if ( (ch == '\n') || (start_col == TW_COLS(win)-1) )
    {
	if ( (start_line == TW_SCROLL_BOTTOM(win)) && TW_AUTO_SCROLL(win) )
	{
	    /* iputc() already scrolled image */
	    /* Watch for bottom line of terminal - terminal will auto
	       scroll when at bottom right */
	    if ( (TW_SCREEN_LINE(win) + start_line != TLINES(win->terminal)-1)
		|| (ch=='\n') )
		ptw_scroll_forward(win);
	    tw_move_to(win,TW_CUR_LINE(win),0);
	}
    }
    if ( acs_missing )
	TW_SET_MODES(win,modes);
}

