#include <string.h>
#include <ctype.h>
#include "twintk.h"

int 
tw_response_win (term_t *term, int line, int col, char *message[], char *buttons[], int extra_flags, border_t *border)

{
    win_t   *win;
    int     ch, flags = IN_BORDER|SHADOW|NO_AUTO_SCROLL|extra_flags;
    size_t  width = 4, height, l;
    char    **p;
    
    /* Determine optimal window width */
    for (p=buttons; *p != NULL; ++p)
	width += strlen(*p) + 2;
    for (p=message; *p != NULL; ++p)
	width = MAX(strlen(*p)+8,width);
    width = MIN(width,TCOLS(term)-2);
    height = 6 + (p - message);
    
    win = tw_new_win(term,height,width,line,col,flags);
    TW_SET_MODES(win,REVERSE_MODE);
    TW_SET_BORDER(win,*border);
    tw_clear_win(win);
    tw_draw_border(win);
    tw_shadow(win);
    TW_REDRAW_WIN(win);
    for (l=2, p=message; *p != NULL; ++p, ++l)
	tw_print_at(win,l,4,*p);
    ch = tw_button_response(win,l+1,buttons);
    tw_del_win(&win);
    return ch;
}

