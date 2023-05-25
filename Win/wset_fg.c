#include "twintk.h"

int 
tw_set_foreground (win_t *win, int color)

{
    if ( TMAX_COLORS(win->terminal) != -1 )
    {
	ISET_FOREGROUND(&win->image,color);
	return 0;
    }
    else
    {
	return(-1);
    }
}
