#include <string.h>
#include "twintk.h"

win_t *
tw_message_win (term_t *term, char *message[], int fg, int bg, int attr)

{
    char    **p;
    int     lines = 0, cols = 0, len, line;
    win_t   *win;
    
    /* Determine size of window needed */
    for (p=message; *p != NULL; ++p)
    {
	++lines;
	if ( (len=strlen(*p)) > cols )
	    cols = len;
    }
    
    /* Open new window */
    win = tw_new_win(term,lines+2,cols+4,(TLINES(term)-lines)/2-1,
		    (TCOLS(term)-cols)/2-1,PLAIN);
    if ( win != NULL )
    {
	tw_set_foreground(win,fg);
	tw_set_background(win,bg);
	TW_SET_MODES(win,attr);
	tw_clear_win(win);
	tw_outer_border(win);
	
	/* Display text */
	for (line=0; message[line] != NULL; ++line)
	{
	    tw_move_to(win,line+1,2);
	    tw_puts(win,message[line]);
	}
    }
    return win;
}

