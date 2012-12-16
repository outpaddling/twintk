#include "twintk.h"

void    tw_outer_border(win)
win_t   *win;

{
    int     c,c2,l,ul=201,up=205,ur=187,ri=186,ll=200,lo=205,lr=188,le=186;
    int    original_mode;
    image_t *image = &(win->image);
    
    TW_SYNC_COLORS(win);
    tw_sync_modes(win);
    original_mode = TCUR_MODES(win->terminal);
    
    /* Set border characters */
    if ( !TW_NO_ACS(win) )
    {
	ul = ACS_ULCORNER(win->terminal);
	up = ACS_HLINE(win->terminal);
	ur = ACS_URCORNER(win->terminal);
	ri = ACS_VLINE(win->terminal);
	lr = ACS_LRCORNER(win->terminal);
	lo = ACS_HLINE(win->terminal);
	ll = ACS_LLCORNER(win->terminal);
	le = ACS_VLINE(win->terminal);
	if ( THAS_ACS(win->terminal) )
	    tset_modes(win->terminal,ACS_MODE|original_mode);
    }
    else
    {
	ul = UPPER_LEFT(&image->border);
	up = UPPER(&image->border);
	ur = UPPER_RIGHT(&image->border);
	ri = RIGHT(&image->border);
	lr = LOWER_RIGHT(&image->border);
	lo = LOWER(&image->border);
	ll = LOWER_LEFT(&image->border);
	le = LEFT(&image->border);
    }
    
    /* Top border */
    if ( win->screen_line > 0 )
    {
	if ( win->screen_col > 0 )
	{
	    tmove_to(win->terminal,win->screen_line-1,win->screen_col-1);
	    tputc(win->terminal,ul);
	}
	else
	    tmove_to(win->terminal,win->screen_line-1,0);
	for (c=0; c<win->view_cols; ++c)
	    tputc(win->terminal,up);
	if ( win->screen_col+TW_COLS(win) < TCOLS(win->terminal) )
	    tputc(win->terminal,ur);
    }
	
    /* Left border */
    c = win->screen_col - 1;
    c2 = win->screen_col + TW_COLS(win);
    if ( c >= 0 )
    {
	for (l=0; l<TW_LINES(win); ++l)
	{
	    tmove_to(win->terminal,win->screen_line+l,c);
	    tputc(win->terminal,le);
	    if ( c2 < TCOLS(win->terminal) )
	    {
		tmove_to(win->terminal,win->screen_line+l,c2);
		tputc(win->terminal,ri);
	    }
	}
    }

    /* Right border */
    if ( c < TCOLS(win->terminal) )
    {
	for (l=0; l<TW_LINES(win); ++l)
	{
	}
    }
    
    /* Bottom border */
    if ( win->screen_line+TW_LINES(win) < TLINES(win->terminal)-1 )
    {
	if ( win->screen_col > 0 )
	{
	    tmove_to(win->terminal,win->screen_line+TW_LINES(win),
		win->screen_col-1);
	    tputc(win->terminal,ll);
	}
	else
	    tmove_to(win->terminal,win->screen_line+TW_LINES(win),0);
	for (c=0; c<win->view_cols; ++c)
	    tputc(win->terminal,lo);
	if ( win->screen_col+TW_COLS(win) < TCOLS(win->terminal) )
	    tputc(win->terminal,lr);
    }
    
    tset_modes(win->terminal,original_mode);
}
