#include <string.h>
#include <ctype.h>
#include <xtend/string.h>   // strlcpy() on Linux
#include <xtend/time.h>
#include "twintk.h"

#define LINE_POS(c) (1 + ((c)-first_str) % (TW_LINES(win)-2))
#define COL_POS(c)  (2 + ((c)-first_str) / (TW_LINES(win)-2) * (maxlen+1))
#define MAX_DISPLAYED(w,ml) ( MAX((TW_COLS(w) - 4) / (ml + 1),1) *\
			    (TW_LINES(w) - 2))

int             tw_get_menu(win, string, list, str_max, event)
win_t          *win;
char           *string;
char           *list[];
int        str_max;
event_t        *event;

{
    int             ch,
		    cooked,
		    maxlen = 0,
		    len,
		    line,
		    col,
		    lines = TW_LINES(win)-2,
		    cols,
		    c,
		    total_strings,
		    cur_str = 0,
		    last_str,
		    outside_click = 0,
		    prev_str = -1,
		    first_str = 0,
		    done;
    char          **p;
    struct timeval  curtime, prevtime;

    /* Initialize close to curtime to prevent diff overflow */
    gettimeofday(&prevtime,NULL);
    
    /* Check terminal modes */
    cooked = win->terminal->tty_mode.c_lflag & (IEXTEN | ICANON | ISIG | ECHO);
    if (cooked)
    {
	tpush_tty(win->terminal);
	tunset_tty(win->terminal, C_LFLAG, IEXTEN | ICANON | ISIG | ECHO);
    }

    /* Determine number of columns */
    for (p = list; *p != NULL; ++p)
	if ((len = strlen(*p)) > maxlen)
	    maxlen = len;
    total_strings = p - list;
    maxlen = MIN(maxlen,TW_COLS(win)-4);
    cols = MAX_DISPLAYED(win,maxlen) / lines;

    /* Set cur_str if incoming event is a mouse click from another
       tw_input_panel() field */
    if ( (event->value == TWC_CLICK_MENU) &&
	 (event->mouse_line > 0) && (event->mouse_col > 0) &&
	 (event->mouse_line < TW_LINES(win)-1) &&
	 (event->mouse_col < TW_COLS(win)-1) )
    {
	line = event->mouse_line - 1;
	col = (event->mouse_col-2) / (maxlen + 1);
	last_str = MIN(total_strings-1,first_str +
		      MAX_DISPLAYED(win,maxlen) - 1);
	cur_str = MIN(first_str + col * lines + line,last_str);
    }

    tpush_event_mask(win->terminal);
    TIGNORE_EVENT(win->terminal,MOTION);
    do
    {
	/* Update scroll bar */
	if (win->bscroll_bar != NULL)
	    tw_move_scroll_bar(win->bscroll_bar,first_str,total_strings);
	done = 0;
	
	/* Display strings */
	if (cur_str != prev_str)
	{
	    display_strings(win,list,first_str);
	    prev_str = cur_str;
	}

	/* Highlight current selection */
	if ( list[cur_str] != NULL )
	{
	    tw_move_to(win, LINE_POS(cur_str), COL_POS(cur_str));
	    TW_SET_HIGHLIGHT(win);
	    tw_nputs(win, list[cur_str], maxlen);
	}
	
	/* Get next action */
	ch = tw_get_event(win, event);
	switch (ch)
	{
	    case KEY_DOWN:
		if (cur_str < total_strings - 1)
		    ++cur_str;
		else
		    done = 1;
		break;
	    case KEY_UP:
		if (cur_str > 0)
		    --cur_str;
		else
		    done = 1;
		break;
	    case KEY_LEFT:
		if (cur_str >= lines)
		    cur_str -= lines;
		else
		    cur_str = 0;
		break;
	    case KEY_RIGHT:
		if (cur_str <= total_strings - lines - 1)
		    cur_str += lines;
		else
		    cur_str = total_strings - 1;
		break;
	    case KEY_MOUSE:
		if (event->type == BUTTON1_PRESS)
		{
		    if ( tw_mouse_in_win(win,win,event) &&
			 (event->mouse_line > 0) && (event->mouse_col > 0) &&
			 (event->mouse_col < TW_COLS(win)-1) )
		    {
			/* Click on scroll bar? */
			if ( (win->bscroll_bar != NULL) &&
			     (event->mouse_line == TW_LINES(win)-1) )
			{
			    int block_pos = TW_HBAR_POS(win->bscroll_bar,
						    first_str,total_strings);
			    
			    /* Click on right arrow? */
			    if ( (event->mouse_col == TW_COLS(win)-2 ) &&
				 (first_str < total_strings - lines) )
				cur_str = first_str += lines;
			    /* Click on bar to right of block? */
			    else if (event->mouse_col > block_pos+1)
			    {
				for (c=cols; first_str >= total_strings - lines * c; --c)
				    ;
				cur_str = first_str += lines * c;
			    }
			    /* Click on left arrow? */
			    else if ( (event->mouse_col == 1 ) &&
				      (first_str >= lines) )
				cur_str = first_str -= lines;
			    /* Click on bar to left of block? */
			    else if (event->mouse_col < block_pos+1)
			    {
				if (first_str >= lines * cols)
				    cur_str = first_str -= lines * cols;
				else
				    cur_str = first_str = 0;
			    }
			}
			else
			{
			    gettimeofday(&curtime,NULL);
			    line = event->mouse_line - 1;
			    col = (event->mouse_col-2) / (maxlen + 1);
			    last_str = MIN(total_strings-1,
				first_str + MAX_DISPLAYED(win,maxlen) - 1);
			    cur_str = MIN(first_str + col * lines + line,
					  last_str);
			    
			    /* Fake ctrl+D on double click */
			    if ( difftimeofday(&curtime,&prevtime) < 300000 )
			    {
				event->type = KEY_PRESS;
				ch = event->value = '\004';
			    }
			    else
				prevtime = curtime;
			}
		    }
		    else
			outside_click = 1;
		}
		break;
	    default:
		break;
	}
	if (cur_str != prev_str)
	{
	    if (cur_str >= first_str + MAX_DISPLAYED(win,maxlen))
		first_str += lines;
	    else if (cur_str < first_str)
		first_str -= lines;
	}
	
	TW_SET_NORMAL(win);
	tw_move_to(win, LINE_POS(cur_str), COL_POS(cur_str));
	tw_nputs(win, list[cur_str], maxlen);
    }
    while ((ch != '\t') && (ch != TWC_INPUT_DONE) && (ch != TWC_INPUT_CANCEL) &&
	    (ch != 16) && (ch != '\n') && (ch != '\r') &&
	    (ch != KEY_ENTER) && !outside_click && !done);

    /* Return string selected */
    strlcpy(string, list[cur_str], str_max);

    /* Reset terminal modes */
    if (cooked)
	tpop_tty(win->terminal);
    tpop_event_mask(win->terminal);
    return ch;
}


void    display_strings(win,list,first_str)
win_t   *win;
char    *list[];
int     first_str;

{
    int     c, c2, total_strings, maxlen = 0, len;
    char    *p;

    /* Get stats on string list */
    for (total_strings=0; list[total_strings] != NULL; ++total_strings)
	if ( (len = strlen(list[total_strings])) > maxlen )
	    maxlen = len;
    maxlen = MIN(maxlen,TW_COLS(win)-4);
    
    /* Print as many strings as will fit in the window */
    for (c = first_str; (c < total_strings) &&
	 (c < first_str + MAX_DISPLAYED(win,maxlen)); ++c)
    {
	imove_to(&win->image, LINE_POS(c), COL_POS(c));
	for (p = list[c]; (*p != '\0') && (p-list[c] < maxlen); ++p)
	    iputc(&win->image, *p);
	while ((p++ - list[c]) <= maxlen)
	    iputc(&win->image, ' ');
    }

    /* Blank remaining positions */
    while (c < first_str + MAX_DISPLAYED(win,maxlen))
    {
	imove_to(&win->image, LINE_POS(c), COL_POS(c));
	for (c2 = 0; c2 <= maxlen; ++c2)
	    iputc(&win->image, ' ');
	++c;
    }
    TW_RESTORE_WIN(win);
}

