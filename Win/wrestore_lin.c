#include <string.h>
#include "twintk.h"

void 
tw_restore_line (win_t *win, int line, int leftcol, int rightcol)

{
    int     save_line, save_col;

    TW_FIND_CURSOR(win, save_line, save_col);
    tw_restore_line_leave_cursor(win, line, leftcol, rightcol);
    tw_move_to(win, save_line, save_col);
}


void 
tw_restore_line_leave_cursor (win_t *win, int line, int leftcol, int rightcol)

{
    unsigned char *char_ptr, *last_char_ptr, *tchar_ptr;
    attr_t *attr_ptr, *last_attr_ptr, *tattr_ptr;
    term_t *terminal;

    terminal = win->terminal;
    //tcur_mode = TCUR_MODES(terminal);

    /* Find first modified character position */
    char_ptr = &TW_CHAR(win, line, leftcol);
    attr_ptr = &TW_ATTR(win, line, leftcol);
    tchar_ptr = &TCHAR(terminal, TW_SCREEN_LINE(win) + line,
		       TW_SCREEN_COL(win) + leftcol);
    tattr_ptr = &TATTR(terminal, TW_SCREEN_LINE(win) + line,
		       TW_SCREEN_COL(win) + leftcol);
    last_char_ptr = &TW_CHAR(win, line, rightcol);

    while ((*char_ptr == *tchar_ptr) &&
	   (memcmp(attr_ptr, tattr_ptr, sizeof(*attr_ptr)) == 0) &&
	   (char_ptr != last_char_ptr))
	++leftcol, ++char_ptr, ++tchar_ptr, ++attr_ptr, ++tattr_ptr;

    /*sprintw(2,50,"%d %d %d %d %d %d",
	    attr_ptr->mode,tattr_ptr->mode,
	    attr_ptr->foreground,tattr_ptr->foreground,
	    attr_ptr->background,tattr_ptr->background);*/
    
    /* Find last modified char position */
    last_attr_ptr = &TW_ATTR(win, line, rightcol);
    tchar_ptr = &TCHAR(terminal, TW_SCREEN_LINE(win) + line,
		       TW_SCREEN_COL(win) + rightcol);
    tattr_ptr = &TATTR(terminal, TW_SCREEN_LINE(win) + line,
		       TW_SCREEN_COL(win) + rightcol);
    while ((*last_char_ptr == *tchar_ptr) &&
	   (memcmp(last_attr_ptr, tattr_ptr, sizeof(*tattr_ptr)) == 0) &&
	   (last_char_ptr >= char_ptr))
	--rightcol, --last_char_ptr, --tchar_ptr, --last_attr_ptr, --tattr_ptr;

    /* Redraw modified portion of line */
    if (char_ptr <= last_char_ptr)
    {
    /*sprintw(2,50,"Redrawing %d chars, line %d",last_char_ptr-char_ptr,line);*/
	tw_redraw_line_leave_cursor(win, line, leftcol, rightcol);
    }
}

