#include "twintk.h"

int     tw_get_event(win,event)
win_t   *win;
event_t *event;

{
    event_t temp_event;
    
    tgetevent(win->terminal,&temp_event);
    switch(temp_event.value)
    {
	case    KEY_MOUSE:
	    temp_event.mouse_line = temp_event.mouse_line - TW_SCREEN_LINE(win);
	    temp_event.mouse_col = temp_event.mouse_col - TW_SCREEN_COL(win);
	    break;
	default:
	    break;
    }
    if ( event != NULL )
	*event = temp_event;
    return temp_event.value;
}

