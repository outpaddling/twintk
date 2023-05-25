#include "twintk.h"

int 
tw_move_right (win_t *win)

{
    int     stat;
    
    stat = imove_right(&win->image);
    if ( stat == SUCCESS )
	tmove_right(win->terminal);
    return(stat);
}

