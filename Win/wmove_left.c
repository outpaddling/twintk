#include "twintk.h"

int 
tw_move_left (win_t *win)

{
    int     stat;
    
    stat = imove_left(&win->image);
    if ( stat == SUCCESS )
	tmove_left(win->terminal);
    return(stat);
}

