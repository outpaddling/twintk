#include <string.h>
#include <ctype.h>
#include <xtend.h>
#include "twintk.h"

int     tw_get_enum(win_t *win, char *string, char *list[],
		    int str_max, event_t *event)

{
    int     c = 0, ch, x, y, cooked, last_item;
    
    /* Find current string value */
    if ( *string == '\0' )
    {
	// FIXME: Does str_max apply to string?
	strlcpy(string,list[0],str_max);
    }
    else
    {
	for (c=0; (list[c] != NULL) && strcmp(list[c],string); ++c)
	    ;
	if ( (list[c] == NULL) || (*list[c] == '\0') )
	    c = 0;
    }
    
    for (last_item=0; list[last_item] != NULL; ++last_item)
	;
    --last_item;
    
    /* Display incoming string, or 1st if incoming was not in list */
    TW_FIND_CURSOR(win, y, x);
    tw_add_string(win, list[c], str_max);
    tw_blank_field(win, str_max-strlen(list[c]));
    tw_move_to(win, y, x);

    /* Check terminal modes */
    cooked = win->terminal->tty_mode.c_lflag & (IEXTEN|ICANON|ISIG|ECHO);
    if ( cooked )
    {
	tpush_tty(win->terminal);
	tunset_tty(win->terminal,C_LFLAG,IEXTEN|ICANON|ISIG|ECHO);
    }

    tpush_event_mask(win->terminal);
    TIGNORE_EVENT(win->terminal,MOTION);
    /* Update value */
    do
    {
	ch = tw_get_event(win,event);
	switch(ch)
	{
	    /* Toggle on button1 click.  Ignore other mouse button events */
	    case    KEY_MOUSE:
		if ( (event->type != BUTTON1_PRESS) ||
		     (event->mouse_line!=y) || (event->mouse_col<x) ||
		     (event->mouse_col>x+str_max) )
		break;
	    case    ' ':
	    case    KEY_NPAGE:
	    case    KEY_RIGHT:
		c = (list[++c] == NULL) ? 0 : c;
		tw_blank_field(win, str_max);
		TW_FIND_CURSOR(win, y, x);
		tw_add_string(win, list[c], str_max);
		tw_move_to(win, y, x);
		break;
	    case    KEY_PPAGE:
	    case    KEY_LEFT:
		if ( --c == -1 )
		    c = last_item;
		tw_blank_field(win, str_max);
		TW_FIND_CURSOR(win, y, x);
		tw_add_string(win, list[c], str_max);
		tw_move_to(win, y, x);
		break;
	    default:
		break;
	}
    } while (!TW_END_INPUT(ch,event));

    if ( cooked )
	tpop_tty(win->terminal);
    strlcpy(string, list[c], str_max);
    tpop_event_mask(win->terminal);
    return (ch);
}
