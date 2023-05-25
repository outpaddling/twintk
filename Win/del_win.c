#include <stdlib.h>
#include "twintk.h"

void 
tw_del_win (win_t **win)

{
    win_t   *w = *win, *shadow = NULL;

    if ( w != NULL )
    {
	/* Remove from terminal winlist */
	tw_unlist_win(w);
	
	/* Delete scroll bars if necessary */
	if ( w->bscroll_bar != NULL )
	    tw_del_win(&w->bscroll_bar);
	if ( w->rscroll_bar != NULL )
	    tw_del_win(&w->rscroll_bar);
	if ( w->tscroll_bar != NULL )
	    tw_del_win(&w->tscroll_bar);
	if ( w->lscroll_bar != NULL )
	    tw_del_win(&w->lscroll_bar);
	
	/* Delete shadow and outer border if necessary */
	if ( TW_HAS_SHADOW(w) || TW_HAS_OUTBORDER(w) )
	    shadow = w->parent_win;
	
	/* Delete window - subwin first if shadow or border */
	TW_SYNC_COLORS(w);
	tw_sync_modes(w);
	del_image(&(w->image),TW_IS_SUBWIN(w));
	free(w);
	*win = NULL;

	if ( shadow != NULL )
	    tw_del_win(&shadow);
    }
}
