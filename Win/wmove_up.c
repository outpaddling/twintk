#include "twintk.h"

int 
tw_move_up (win_t *win)

{
    int     stat;
    
    stat = imove_up(&win->image);
    if ( stat == SUCCESS )
	tmove_up(win->terminal);
    return(stat);
}

