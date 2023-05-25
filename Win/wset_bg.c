#include "twintk.h"

int 
tw_set_background (win_t *win, int color)

{
    if ( TMAX_COLORS(win->terminal) != -1 )
    {
	ISET_BACKGROUND(&win->image,color);
	return 0;
    }
    else
    {
	return(-1);
    }
}
