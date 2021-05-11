#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <xtend.h>
#include "twintk.h"

short   tw_get_short(win, incoming, min, max, base, event)
win_t   *win;
int     incoming, min, max, base;
event_t *event;

{
    return (short)
	tw_get_long(win, (long)incoming, (long)min, (long)max, base, event);
}


int     tw_get_int(win, incoming, min, max, base, event)
win_t   *win;
int     incoming, min, max, base;
event_t *event;

{
    return (int)
	tw_get_long(win, (long)incoming, (long)min, (long)max, base, event);
}


unsigned short  tw_get_ushort(win_t *win, unsigned short incoming, unsigned short min, 
		    unsigned short max, int base, event_t *event)

{
    return (unsigned short)
	tw_get_ulong(win, (unsigned long)incoming, (unsigned long)min,
	    (unsigned long)max, base, event);
}


unsigned int    tw_get_uint(win_t *win, unsigned int incoming, unsigned int min, 
		    unsigned int max, int base, event_t *event)

{
    return (unsigned int)
	tw_get_ulong(win, (unsigned long)incoming, (unsigned long)min,
		(unsigned long)max, base, event);
}


long    tw_get_long(win_t *win, long incoming, long min, long max, int base, event_t *event)

{
    char    temp[20], *tail;
    int     maxlen;
    long    outgoing;
    
    /* Find maximum length of an input value */
    maxlen = MAX(strlen(ltostrn(temp,max,base,19)),
		 strlen(ltostrn(temp,min,base,19)));
    
    /* Display incoming string */
    ltostrn(temp,incoming,base,19);
    
    tw_get_int_string(win, maxlen, base, event, temp);

    outgoing = strtol(temp,&tail,base);
    if (outgoing > max)
	outgoing = max;
    else if (outgoing < min)
	outgoing = min;
    return (outgoing);
}


unsigned long   tw_get_ulong(win_t *win, unsigned long incoming, unsigned long min,
			    unsigned long max, int base, event_t *event)

{
    char    temp[20], *tail;
    int     maxlen;
    long    outgoing;
    
    /* Find maximum length of an input value */
    maxlen = MAX(strlen(ltostrn(temp,max,base,19)),
		 strlen(ltostrn(temp,min,base,19)));
    
    /* Display incoming string */
    ltostrn(temp,incoming,base,19);
    
    tw_get_int_string(win, maxlen, base, event, temp);

    outgoing = strtol(temp,&tail,base);
    if (outgoing > max)
	outgoing = max;
    else if (outgoing < min)
	outgoing = min;
    return (outgoing);
}


void    tw_get_int_string(win_t *win, int maxlen, int base, event_t *event, char *temp)

{
    int     len = strlen(temp), line, col, pos = 0, p, ch, touched = 0, cooked;

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
	    case    '-':
		if ( pos != 0 ) /* Only accept - sign at first column */
		    break;
	    default:
		if ( (ch == '-') || (digit(ch,base) != TWC_INVALID_DIGIT) )
		{
		    if ( !touched )
		    {
			tw_blank_field(win,maxlen);
			temp[0] = ch;
			temp[1] = '\0';
			len = 1;
			pos = 1;
			tw_putc(win,ch);
		    }
		    else if ( len < maxlen )
		    {
			memmove(temp+pos+1,temp+pos,len-pos);
			temp[pos] = ch;
			for (p=pos; p<=len; ++p)
			    tw_putc(win,temp[p]);
			++len;
			++pos;
			if ( pos < len )
			    tw_move_to(win,line,col+pos);
		    }
		}
	}
	touched = 1;
    }   while (!TW_END_INPUT(ch,event));
    temp[len] = '\0';

    if ( cooked )
	tpop_tty(win->terminal);
    tpop_event_mask(win->terminal);
}


int     digit(ch,base)
int     ch,base;

{
    int     d;
    
    if ( isdigit(ch) )
	d = ch-'0';
    else if ( isalpha(ch) )
	d = tolower(ch) - 'a' + 10;
    else
	return TWC_INVALID_DIGIT;
    
    if ( d < base )
	return d;
    else
	return TWC_INVALID_DIGIT;
}

