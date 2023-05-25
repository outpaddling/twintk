#include <string.h>
#include <ctype.h>
#include "twintk.h"

void 
tw_blank_field (win_t *win, int len)

{
    int     c, y, x;

    TW_FIND_CURSOR(win, y, x);
    for (c = 0; c < len; ++c)
	tw_putc(win, ' ');
    tw_move_to(win, y, x);
}
