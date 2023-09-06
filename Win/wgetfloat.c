#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "twintk.h"

float 
tw_get_float (win_t *win, double incoming, double min, double max, event_t *event)

{
    return (float)
	tw_get_double(win, (long)incoming, (long)min, (long)max, event);
}


double 
tw_get_double (win_t *win, double incoming, double min, double max, event_t *event)

{
    int     len, pos = 0, p, ch, line, col, maxlen, touched = 0, cooked;
    char    temp[20], *tail;
    double  outgoing;
    
    /* Find maximum length of an input value */
    maxlen = 19;
    /*MAX(strlen(xt_ltostrn(temp,max,base,19)),
		strlen(xt_ltostrn(temp,min,base,19)));*/
    
    /* Display incoming string */
    snprintf(temp,19,"%f",incoming);
    len = strlen(temp);

    TW_FIND_CURSOR(win, line, col);
    tw_add_string(win, temp, maxlen);
    tw_move_to(win, line, col);

    /* Check terminal modes */
    cooked = win->terminal->tty_mode.c_lflag & (IEXTEN|ICANON|ISIG|ECHO);
    if ( cooked )
    {
	tpush_tty(win->terminal);
	tunset_tty(win->terminal,C_LFLAG,IEXTEN|ICANON|ISIG|ECHO);
    }

    tpush_event_mask(win->terminal);
    TSELECT_INPUT(win->terminal,KEY_PRESS|BUTTON_PRESS);
    
    do
    {
	ch = tw_get_event(win,event);
	switch(ch)
	{
	    /* Ignore mouse button events */
	    case    KEY_MOUSE:
		break;
	    case    KEY_BACKSPACE:
		if (pos > 0)
		    --pos;      /* And on to KEY_DC... */
		else
		    break;
	    case    KEY_DC:
		if ( len > 0 )
		{
		    --len;
		    memmove(temp+pos,temp+pos+1,len-pos);
		    tw_move_to(win,line,col+pos);
		    for (p=pos; p<len; ++p)
			tw_putc(win,temp[p]);
		    tw_putc(win,' ');
		    tw_move_to(win,line,col+pos);
		}
		break;
	    case    KEY_LEFT:
		if ( pos > 0 )
		{
		    --pos;
		    tw_move_left(win);
		}
		break;
	    case    KEY_RIGHT:
		if ( pos < len )
		{
		    ++pos;
		    tw_move_right(win);
		}
		break;
	    case    '\005':     /* Ctrl-E = goto eoln */
		pos = len;
		tw_move_to(win,line,col+pos);
		break;
	    case    '\013':     /* Ctrl-K = kill rest of line */
		for (p=pos; p<len; ++p)
		    tw_putc(win,' ');
		len = pos;
		tw_move_to(win,line,col+pos);
		break;
	    case    '.':
	    case    'e':
	    case    'E':
		/* Only allow one of these per number */
		tw_insert_float_digit(win,temp,ch,&pos,maxlen,line,col,&len,touched);
		break;
	    case    '-':
		if ( pos != 0 ) /* Only accept - sign at first column */
		    break;
	    default:
		tw_insert_float_digit(win,temp,ch,&pos,maxlen,line,col,&len,touched);
	}
	touched = 1;
    }   while (!TW_END_INPUT(ch,event));

    temp[len] = '\0';
    outgoing = strtod(temp,&tail);
    if (outgoing > max)
	outgoing = max;
    else if (outgoing < min)
	outgoing = min;
    if ( cooked )
	tpop_tty(win->terminal);
    tpop_event_mask(win->terminal);
    return (outgoing);
}


int     tw_insert_float_digit(win_t *win,char *temp,int ch,int *pos,int maxlen,
			    int line, int col, int *len, int touched)

{
    int     p;
    
    if ( (strchr("-.Ee",ch) != NULL) || (digit(ch,10) != TWC_INVALID_DIGIT) )
    {
	if ( !touched )
	{
	    tw_blank_field(win,maxlen);
	    temp[0] = ch;
	    temp[1] = '\0';
	    *len = 1;
	    *pos = 1;
	    tw_putc(win,ch);
	}
	else if ( *len < maxlen )
	{
	    memmove(temp+*pos+1,temp+*pos,*len-*pos);
	    temp[*pos] = ch;
	    for (p=*pos; p<=*len; ++p)
		tw_putc(win,temp[p]);
	    ++*len;
	    ++*pos;
	    if ( *pos < *len )
		tw_move_to(win,line,col+*pos);
	}
    }
    return 0;
}

