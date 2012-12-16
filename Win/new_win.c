#include <stdio.h>
#include <stdlib.h>
#include "twintk.h"

/*
 *  Create a new window on the specified terminal screen.
 */
 
win_t  *tw_new_win(terminal, lines, cols, screen_line, screen_col, traits)
term_t *terminal;   /* The terminal in which the window should be created */
int     lines,      /* Height of the new window */
	cols,       /* Width of the new window */
	screen_line,    /* Location of top line of window on terminal */
	screen_col,     /* Location of left column of window on terminal */
	traits;     /* Bitmask of features, such as shadow, border */

{
    win_t  *win, *temp_win;
    int     border_line, border_col;
    /* Shut off shadow and outborder for shadow or border win
       to avoid problems with TW_REDRAW_WIN() */
    int     shadow_traits = (traits & ~(SHADOW|OUT_BORDER)) | NO_AUTO_SCROLL;

    /* Check for special position specifiers */
    border_line = screen_line == TWC_CENTER_WIN ? TWC_CENTER_WIN : screen_line - 1;
    border_col = screen_col == TWC_CENTER_WIN ? TWC_CENTER_WIN : screen_col - 1;

    switch (traits & (SHADOW | OUT_BORDER))
    {
    case SHADOW | OUT_BORDER:
	temp_win = simple_win(terminal, lines + 3, cols + 3,
			      border_line, border_col, shadow_traits);
	win = tw_sub_win(temp_win, lines, cols, 1, 1, traits);
	break;
    case OUT_BORDER:
	temp_win = simple_win(terminal, lines + 2, cols + 2,
			      border_line, border_col, shadow_traits);
	win = tw_sub_win(temp_win, lines, cols, 1, 1, traits);
	break;
    case SHADOW:
	temp_win = simple_win(terminal, lines + 1, cols + 1,
			      screen_line, screen_col, shadow_traits);
	win = tw_sub_win(temp_win, lines, cols, 0, 0, traits);
	break;
    default:
	win = simple_win(terminal, lines, cols, screen_line, screen_col, traits);
    }

    /* Store traits */
    win->traits = traits;

    return win;
}


win_t  *simple_win(terminal, lines, cols, screen_line, screen_col, traits)
term_t *terminal;
int     lines, cols, screen_line, screen_col, traits;

{
    win_t  *win;
    int     save_line = screen_line, save_col = screen_col;

    /* Check for special position specifiers */
    check_position(&screen_line, &screen_col, lines, cols,
		   TLINES(terminal), TCOLS(terminal));

    win = (win_t *) malloc(sizeof(win_t));
    if (win == NULL)
    {
	fprintf(stderr, "tw_new_win(): out of memory.\n");
	return (NULL);
    }

    /* Link to a terminal */
    win->terminal = terminal;

    /* Put window in terminal list for tw_redraw_all */
    tw_list_win(win, save_line, save_col);

    /* Set up window image */
    new_image(&win->image, lines, cols, traits);

    /* Check for special position specifiers */
    check_position(&screen_line, &screen_col, lines, cols,
		   TLINES(terminal), TCOLS(terminal));

    /* Set view size */
    init_win(win, lines, cols);

    /* Set screen position */
    if (screen_line >= TLINES(terminal))
    {
	fprintf(stderr, "tw_new_win(): screen_line (%d) is off the screen.\n",
		screen_line);
	return (NULL);
    }
    else
	win->screen_line = screen_line;

    if (screen_col >= TCOLS(terminal))
    {
	fprintf(stderr, "tw_new_win(): screen_col (%d) is off the screen.\n",
		screen_col);
	return (NULL);
    }
    else
	win->screen_col = screen_col;

    /* Set window position on buffer */
    win->first_vis_line = win->first_vis_col = 0;
    win->parent_win = NULL;
    win->traits = traits;       /* OK? Was missing and causeing crash in
				 * resize */
    default_win_attr(win);
    iclear_image(&win->image);
    return (win);
}


void    check_position(line_ptr, col_ptr, lines, cols, parent_lines, parent_cols)
int    *line_ptr, *col_ptr, lines, cols, parent_lines, parent_cols;

{
    int     screen_line = *line_ptr, screen_col = *col_ptr;

    /* Check for CENTER option */
    if (screen_line == TWC_CENTER_WIN)
	screen_line = (parent_lines - lines) >> 1;
    if (screen_col == TWC_CENTER_WIN)
	screen_col = (parent_cols - cols) >> 1;

    /* Check for offset from right or bottom or parent win or term */
    if (screen_line < 0)
	screen_line = parent_lines + screen_line;
    if (screen_col < 0)
	screen_col = parent_cols + screen_col;

    *line_ptr = screen_line;
    *col_ptr = screen_col;
}


void    default_win_attr(win)
win_t  *win;

{
    win->win_attr.normal.foreground = WHITE;
    win->win_attr.normal.background = BLACK;
    win->win_attr.highlight.foreground = BLUE;
    win->win_attr.highlight.background = BLACK;

    if (THAS_BOLD_MODE(win->terminal))
	win->win_attr.normal.mode = BOLD_MODE;
    else
	win->win_attr.normal.mode = NORMAL_MODE;
    if (THAS_REVERSE_MODE(win->terminal))
	win->win_attr.highlight.mode = REVERSE_MODE;
    else
	win->win_attr.highlight.mode = HIGHLIGHT_MODE;
}


void    init_win(win, lines, cols)
win_t  *win;
int     lines, cols;

{
    win->view_lines = lines;
    win->view_cols = cols;
    win->bscroll_bar = win->rscroll_bar = win->tscroll_bar = win->lscroll_bar = NULL;
}
