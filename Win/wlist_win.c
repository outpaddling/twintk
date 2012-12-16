#include <stdlib.h>
#include "twintk.h"

/*
 * Enhancement: Include location, possibly TWC_CENTER_WIN or - values
 * for offset from right or bottom.  ( Like tw_new_win and tw_sub_win )
 */
 
void    tw_list_win(win,parent_line,parent_col)
win_t   *win;
int     parent_line,parent_col;

{
    winlist_t   *temp;
    term_t      *terminal = win->terminal;
    
    /* Empty list? */
    if ( terminal->winlist_head == NULL )
    {
	terminal->winlist_head = terminal->winlist_tail = (winlist_t *)malloc(sizeof(winlist_t));
	terminal->winlist_head->win = win;
	terminal->winlist_head->parent_line = parent_line;
	terminal->winlist_head->parent_col = parent_col;
	terminal->winlist_head->next = NULL;
    }
    else
    {
	temp = (winlist_t *)malloc(sizeof(winlist_t));
	temp->win = win;
	temp->parent_line = parent_line;
	temp->parent_col = parent_col;
	temp->next = NULL;
	terminal->winlist_tail->next = temp;
	terminal->winlist_tail = temp;
    }
}

