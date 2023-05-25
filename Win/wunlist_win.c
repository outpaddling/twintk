#include <stdlib.h>
#include "twintk.h"

void 
tw_unlist_win (win_t *win)

{
    winlist_t   *p, *parent;
    term_t      *terminal = win->terminal;
    
    /* Empty list? */
    if ( terminal->winlist_head == NULL )
	return;
    
    /* Only one window? */
    if ( (terminal->winlist_head->next == NULL) &&
	 (win == terminal->winlist_head->win) )
    {
	free(terminal->winlist_head);
	terminal->winlist_head = terminal->winlist_tail = NULL;
    }
    else    /* At least two windows */
    {
	if ( win == terminal->winlist_head->win )
	{
	    p = terminal->winlist_head;
	    terminal->winlist_head = terminal->winlist_head->next;
	    free(p);
	}
	else
	{
	    p = terminal->winlist_head;
	    parent = NULL;
	    while ((p!=NULL) && (p->win != win))
	    {
		parent = p;
		p = p->next;
	    }
	    if ( p != NULL )
	    {
		parent->next = p->next;
		if ( p == terminal->winlist_tail )
		    terminal->winlist_tail = parent;
		free(p);
	    }
	}
    }
}


int 
tw_show_list (term_t *terminal)

{
    winlist_t   *p;

    tmove_to(terminal,TLINES(terminal)-10,0);
    tputs(terminal,"Windows: ");
    for (p=terminal->winlist_head; p!=NULL; p=p->next)
	tprintf(terminal,"%p->",p->win);
    tprintf(terminal,"\n\r");
    TFLUSH_OUT(terminal);
    tgetc(terminal);
    return 0;
}

