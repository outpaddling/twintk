#include <string.h>
#include "twintk.h"

/************************************************
 * Get a single key stroke response and validate
 ************************************************/

int 
tw_get_response (win_t *win, char valid[])

{
    int     ch;

    /* Read until a valid character is entered */
    do
    {
	ch = tw_getc(win);
    } while (strchr(valid, ch) == NULL);
    return (ch);
}

