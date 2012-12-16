#include "twintk.h"

void    tw_sync_modes(win)
win_t   *win;

{
    if ( TW_CUR_MODES(win) != TCUR_MODES((win)->terminal) )
    {
	/* Some terminals can't change modes in color */
	tset_modes((win)->terminal,TW_CUR_MODES(win));
    }
}

