#include "twintk.h"

void 
tw_redraw_line (win_t *win, int line, int leftcol, int rightcol)

{
    int    save_line,save_col;
    
    TW_FIND_CURSOR(win,save_line,save_col);
    tw_redraw_line_leave_cursor(win,line,leftcol,rightcol);
    tw_move_to(win,save_line,save_col);
}


void 
tw_redraw_line_leave_cursor (win_t *win, int line, int leftcol, int rightcol)

{
    unsigned char        *char_ptr,*last_char_ptr;
    int         tcur_mode,tcur_fg,tcur_bg,color;
    attr_t      *attr_ptr;
    term_t      *terminal;
    
    terminal = win->terminal;
    color = TCOLOR_TERM(terminal);
    tcur_mode = TCUR_MODES(terminal);
    tcur_fg = TCUR_FOREGROUND(terminal);
    tcur_bg = TCUR_BACKGROUND(terminal);
    
    attr_ptr = &TW_ATTR(win,line,leftcol);
    char_ptr = &TW_CHAR(win,line,leftcol);
    last_char_ptr = &TW_CHAR(win,line,rightcol);
    //col = leftcol;
    
    tw_move_to(win,line,leftcol);
    while ( char_ptr <= last_char_ptr )
    {
	/* Sync colors */
	if ( color && (attr_ptr->foreground != tcur_fg) )
	{
	    tcur_fg = attr_ptr->foreground;
	    tset_foreground(win->terminal,tcur_fg);
	}
	
	if ( color && (attr_ptr->background != tcur_bg) )
	{
	    tcur_bg = attr_ptr->background;
	    tset_background(win->terminal,tcur_bg);
	}

	/* Sync attributes - must sync colors FIRST for Coherent! */
	if ( attr_ptr->mode != tcur_mode )
	{
	    tcur_mode = attr_ptr->mode;
	    tset_modes(terminal,tcur_mode);
	}
	
	/* Draw character */
	taddc(terminal,*char_ptr++);
	++attr_ptr;
    }
}

