#include "twintk.h"

int 
tw_move_down (win_t *win)

{
    int     stat;
    
    stat = imove_down(&win->image);
    if ( stat == SUCCESS )
	tmove_down(win->terminal);
    return(stat);
}

