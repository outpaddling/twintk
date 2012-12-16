#include <stdio.h>
#include "twintk.h"

int         tw_move_to(win,line,col)
win_t       *win;
int    line,col;

{
    int     stat;
    
    /* Move virtual cursor */
    if ( (stat=imove_to(&win->image,line,col)) == SUCCESS )
    {
	/* Move physical cursor */
	tmove_to(win->terminal,TW_SCREEN_CUR_LINE(win),TW_SCREEN_CUR_COL(win));
    }
    else
	fprintf(win->terminal->fperr,"tw_move_to(): imove_to failed.");
    return(stat);
}

