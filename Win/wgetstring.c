#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <glob.h>
#include <xtend.h>
#include "twintk.h"

void    tw_get_string_putc(win_t *win, int ch,tw_str_t string_type)

{
    if ( string_type == TWC_SECURE )
	tw_putc(win, '*');
    else
	tw_putc(win,ch);
}


char    *tw_get_string(win, string, maxlen, field_width, string_type, event)
win_t  *win;
char   *string;
int maxlen, field_width;
tw_str_t   string_type;
event_t *event;

{
    return tw_get_string_hist(win,string,maxlen,field_width,string_type,
			    event,NULL);
}


char    *tw_get_string_hist(win, string, maxlen, field_width, string_type,
	    event,history)
win_t  *win;
char   *string;
int maxlen, field_width;
tw_str_t   string_type;
event_t *event;
char    *history[];

{
    int     len, pos = 0, first_pos = 0, p, ch, line, col, touched = 0,
	    cooked, hist_index=0, hist_count=0;
    char    *temp = (char *)malloc(maxlen+1);
    term_t  *term = win->terminal;
    
    /* If secure (invisible) input, always start with a blank string. */
    if ( string_type == TWC_SECURE )
	*string = '\0';
    
    /* Go to end of history list */
    if ( history != NULL )
    {
	for (hist_index = 0; history[hist_index] != NULL; ++hist_index)
	    ;
	hist_count = hist_index;
    }
    
    /* Check terminal modes */
    cooked = term->tty_mode.c_lflag & (IEXTEN|ICANON|ISIG|ECHO);
    if ( cooked )
    {
	tpush_tty(term);
	tunset_tty(term,C_LFLAG,IEXTEN|ICANON|ISIG|ECHO);
    }
    tpush_event_mask(term);
    TIGNORE_EVENT(term,BUTTON_RELEASE);
    
    /* Display incoming string */
    /* FIXME: Display in reverse video to indicate that it will be
       blanked by the first keystroke */
    len = strlen(string);
    strlcpy(temp, string, maxlen);

    TW_FIND_CURSOR(win, line, col);
    tw_move_to(win, line, col);
    tw_add_string(win, string, field_width);
    tw_blank_field(win, MAX(field_width-len,0));
    tw_move_to(win, line, col);
    
    TIGNORE_EVENT(win->terminal,MOTION);
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
		/* Don't try to delete if at end of string */
		if ( pos < len )
		{
		    --len;
		    memmove(temp+pos,temp+pos+1,len-pos);
		    if ( (first_pos > 0) && (ch == KEY_BACKSPACE) )
		    {
			--first_pos;
			tw_move_to(win,line,col);
			for (p=first_pos; p<MIN(len,first_pos+field_width); ++p)
			    tw_get_string_putc(win,temp[p],string_type);
			tw_move_to(win,line,col+pos-first_pos);
		    }
		    else
		    {
			tw_move_to(win,line,col+pos-first_pos);
			for (p=pos; p<MIN(len,first_pos+field_width); ++p)
			    tw_get_string_putc(win,temp[p],string_type);
			tw_putc(win,' ');
			tw_move_to(win,line,col+pos-first_pos);
		    }
		}
		break;
	    case    KEY_LEFT:
		if ( pos > 0 )
		{
		    --pos;
		    if ( pos >= first_pos )
		    {
			tw_move_left(win);
		    }
		    else
		    {
			if ( first_pos > 0 )
			{
			    --first_pos;
			    for (p=first_pos; p<MIN(len,first_pos+field_width); ++p)
				tw_get_string_putc(win,temp[p],string_type);
			    tw_move_to(win,line,col);
			}
		    }
		}
		break;
	    case    KEY_RIGHT:
		if ( pos < len )
		{
		    ++pos;
		    if ( pos <= first_pos + field_width )
			tw_move_right(win);
		    else
		    {
			++first_pos;
			tw_move_to(win,line,col);
			for (p=pos-field_width; p<pos; ++p)
			    tw_get_string_putc(win,temp[p],string_type);
		    }
		}
		break;
	    /* Filename completion.  Fall into eoln code if successful */
	    case    '\t':   /* Filename completion */
		break;
		/*
		snprintf(pattern,PATH_MAX,"%s*",temp);
		glob(pattern,0,NULL,&glob_list);
		if ( glob_list.gl_matchc != 1 )
		    break;
		strlcpy(temp,glob_list.gl_pathv[0],maxlen);
		globfree(&glob_list);
		*/
	    case    '\002':     /* Ctrl+B = goto beginning */
	    case    KEY_HOME:
		pos = first_pos = 0;
		tw_move_to(win,line,col);
		for (p=0; (p<field_width) && (p<len); ++p)
		    tw_get_string_putc(win,temp[p],string_type);
		tw_move_to(win,line,col);
		break;
	    case    '\005':     /* Ctrl-E = goto eoln */
	    case    KEY_END:
		pos = len;
		if ( pos > first_pos + field_width )
		{
		    first_pos = len - field_width;
		    tw_move_to(win, line, col);
		    for (p=first_pos; p < len; ++p)
			tw_get_string_putc(win,temp[p],string_type);
		}
		else
		    tw_move_to(win, line, col + pos - first_pos);
		break;
	    case    '\013':     /* Ctrl-K = kill rest of line */
		for (p=pos; (p<len) && (p<first_pos+field_width); ++p)
		    tw_putc(win,' ');
		len = pos;
		tw_move_to(win,line,col+pos-first_pos);
		break;
	    case    KEY_UP:
		if (history != NULL)
		    if ( hist_index > 0 )
		    {
			--hist_index;
			strlcpy(temp,history[hist_index],maxlen);
			len = strlen(temp);
			pos = 0;
		    }
		break;
	    case    KEY_DOWN:
		if ( history != NULL )
		    if ( history[hist_index] != NULL )
		    {
			++hist_index;
			strlcpy(temp,history[hist_index],maxlen);
			len = strlen(temp);
			pos = 0;
		    }
		break;
	    default:
		if ( isascii(ch) && isprint(ch) )
		{
		    if ( !touched )
		    {
			tw_blank_field(win,len);
			len = 0;
		    }
		    if ( len < maxlen )
		    {
			memmove(temp+pos+1,temp+pos,len-pos);
			switch(string_type)
			{
			    case    TWC_UPPER_CASE:
				temp[pos] = toupper(ch);
				break;
			    case    TWC_CAPS:
				if ((pos == 0) || ((pos > 0) && isspace(temp[pos - 1])))
				{
				    temp[pos] = toupper(ch);
				    break;
				}
				/* No break - let it run to default */
			    default:
				temp[pos] = ch;
			}
			
			/* Insert char into window */
			if ( pos < first_pos+field_width )
			{
			    for (p=pos; p<=MIN(len,first_pos+field_width-1); ++p)
				tw_get_string_putc(win,temp[p],string_type);
			}
			else    /* Scroll left and add new char */
			{
			    ++first_pos;
			    tw_move_to(win,line,col);
			    for (p=first_pos; p<=MIN(len,first_pos+field_width-1); ++p)
				tw_get_string_putc(win,temp[p],string_type);
			}
			++len;
			++pos;
			if ( pos < len )
			    tw_move_to(win,line,col+pos-first_pos);
		    }
		}
	}   /* Switch */
	touched = 1;
    }   while (!TW_END_INPUT(ch,event));

    temp[len] = '\0';
    tw_move_to(win,line,col);
    for (p=0; p<MIN(len,field_width); ++p)
	tw_get_string_putc(win,temp[p],string_type);
    strlcpy(string, temp, maxlen);
    
    if ( history != NULL )
    {
	history[hist_count++] = strdup(string);
	history[hist_count] = NULL;
    }
    
    if ( cooked )
	tpop_tty(term);
    tpop_event_mask(term);
    
    free(temp);
    return (string);
}

