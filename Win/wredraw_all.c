#include "twintk.h"

void 
tw_redraw_all (term_t *terminal)

{
    winlist_t *p;
    int     parent_line, parent_col;

    for (p = terminal->winlist_head; p != NULL; p = p->next)
    {
	parent_line = p->parent_line;
	parent_col = p->parent_col;

	/* Move window to desired location */
	if (TW_IS_SUBWIN(p->win))
	{
	    check_position(&parent_line, &parent_col,
			   TW_LINES(p->win), TW_COLS(p->win),
		     TW_LINES(p->win->parent_win), TW_COLS(p->win->parent_win));
	    p->win->screen_line = TW_SCREEN_LINE(p->win->parent_win) + parent_line;
	    p->win->screen_col = TW_SCREEN_COL(p->win->parent_win) + parent_col;
	}
	else
	{
	    check_position(&parent_line, &parent_col,
			   TW_LINES(p->win), TW_COLS(p->win),
			   TLINES(terminal), TCOLS(terminal));
	    p->win->screen_line = parent_line;
	    p->win->screen_col = parent_col;
	}

	/* Redraw */
	TW_REDRAW_WIN(p->win);    /* Was causing crash on shadow parent win. */
	TW_SYNC_CURSOR(p->win);
    }
}

