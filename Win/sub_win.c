#include <stdlib.h>
#include "twintk.h"

win_t *
tw_sub_win (win_t *parent_win, int wlines, int wcols, int parent_line, int parent_col, int traits)

{
    win_t  *win;
    int     save_line = parent_line, save_col = parent_col;

    /* Check for special position specifiers */
    check_position(&parent_line, &parent_col, wlines, wcols,
		   TW_LINES(parent_win), TW_COLS(parent_win));

    if ((wlines + parent_line > TW_LINES(parent_win)) ||
	(wcols + parent_col > TW_COLS(parent_win)))
    {
	fprintf(stderr, "tw_sub_win(): %dx%d at %d,%d does not fit in parent window.\n",
		wlines, wcols, parent_line, parent_col);
	return (NULL);
    }

    win = (win_t *) malloc(sizeof(win_t));
    if (win != NULL)
    {
	/* Link to a terminal */
	win->terminal = parent_win->terminal;

	/* Put in terminal list for tw_redraw_all */
	tw_list_win(win, save_line, save_col);

	/* Set window traits (image traits set by sub_image()) */
	win->traits = traits;
	default_win_attr(win);

	/* Set up window image */
	sub_image(&parent_win->image, &win->image, wlines, wcols,
		  parent_line, parent_col, traits);

	/* Set view size */
	init_win(win, wlines, wcols);

	/* Set screen position - already checked above */
	win->screen_line = TW_SCREEN_LINE(parent_win) + parent_line;
	win->screen_col = TW_SCREEN_COL(parent_win) + parent_col;

	/* Set window position on buffer */
	win->first_vis_line = win->first_vis_col = 0;
	win->parent_win = parent_win;
	return (win);
    }
    else
    {
	fprintf(stderr, "tw_sub_win(): Cannot allocate window.\n");
	return (NULL);
    }
}
