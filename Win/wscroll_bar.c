#include "twintk.h"

void    tw_draw_scroll_bar(bar_win)
win_t   *bar_win;

{
    int     line, col;
    
    tw_set_foreground(bar_win,WHITE);
    tw_set_background(bar_win,BLACK);
    TW_SET_MODES(bar_win,ACS_MODE);
    
    if ( TW_LINES(bar_win) == 1 )     /* Horizontal */
    {
	tw_move_to(bar_win,0,0);
	tw_putc(bar_win,ACS_LARROW(bar_win->terminal));
	tw_draw_block(bar_win);
	for (col=2; col<TW_COLS(bar_win)-1; ++col)
	    tw_putc(bar_win,ACS_CKBOARD(bar_win->terminal));
	tw_putc(bar_win,ACS_RARROW(bar_win->terminal));
	tw_move_to(bar_win,0,1);  /* Start block at origin */
    }
    else                            /* Vertical */
    {
	tw_move_to(bar_win,0,0);
	tw_putc(bar_win,ACS_UARROW(bar_win->terminal));
	tw_move_to(bar_win,1,0);
	tw_draw_block(bar_win);
	for (line=2; line<TW_LINES(bar_win)-1; ++line)
	{
	    tw_move_to(bar_win,line,0);
	    tw_putc(bar_win,ACS_CKBOARD(bar_win->terminal));
	}
	tw_move_to(bar_win,line,0);
	tw_putc(bar_win,ACS_DARROW(bar_win->terminal));
	tw_move_to(bar_win,1,0);  /* Start block at origin */
    }
    TW_FLUSH_OUT(bar_win);
    TW_RESTORE_WIN(bar_win);  /* Shouldn't be needed! */
}


int     tw_move_scroll_bar(bar_win,numerator,denominator)
win_t   *bar_win;
int     numerator, denominator;

{
    int     pos, status;
    
    if ( numerator < denominator )
    {
	TW_SET_MODES(bar_win,ACS_MODE);
	if ( TW_LINES(bar_win) == 1 )     /* Horizontal */
	{
	    pos = TW_HBAR_POS(bar_win,numerator,denominator);
	    if ( pos != TW_CUR_COL(bar_win) )
	    {
		tw_putc(bar_win,ACS_CKBOARD(bar_win->terminal));
		tw_move_to(bar_win,0,pos);
		tw_draw_block(bar_win);
		tw_move_to(bar_win,0,pos);    /* Leave cursor at block */
		TW_FLUSH_OUT(bar_win);
		TW_RESTORE_WIN(bar_win);  /* Shouldn't be needed! */
	    }
	}
	else
	{
	    pos = TW_VBAR_POS(bar_win,numerator,denominator);
	    if ( pos != TW_CUR_LINE(bar_win) )
	    {
		tw_putc(bar_win,ACS_CKBOARD(bar_win->terminal));
		tw_move_to(bar_win,pos,0);
		tw_draw_block(bar_win);
		tw_move_to(bar_win,pos,0);    /* Leave cursor at block */
		TW_FLUSH_OUT(bar_win);
		TW_RESTORE_WIN(bar_win);  /* Shouldn't be needed! */
	    }
	}
	status = SUCCESS;
    }
    else
	status = FAILURE;
    return status;
}


void    tw_draw_block(win)
win_t   *win;

{
    int     block_ch = ACS_BLOCK(win->terminal), modes;
    
    if ( block_ch == (ACS_MISSING|' ') )
    {
	modes = TW_CUR_MODES(win);
	TW_SET_MODES(win,modes|REVERSE_MODE);
	tw_putc(win,block_ch);
	TW_FLUSH_OUT(win);
	TW_SET_MODES(win,modes);
    }
    else
	tw_putc(win,block_ch);
}

