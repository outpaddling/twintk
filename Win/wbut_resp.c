#include <ctype.h>
#include <string.h>
#include "twintk.h"

#define MAX_BUTTONS 20

void    tw_draw_button(win,line,col,button)
win_t   *win;
int     line, col;
char    *button;

{
    int     save = TW_CUR_MODES(win);

    tw_move_to(win,line,col);
    while ( !isalnum(*button) && (*button!='\0') )
	tw_putc(win,*button++);
    if ( THAS_UNDERLINE_MODE(win->terminal) )
	TW_SET_MODES(win,save|UNDERLINE_MODE);
    else
	TW_SET_MODES(win,save|BOLD_MODE);
    tw_putc(win,*button++);
    TW_SET_MODES(win,save);
    while ( *button != '\0' )
	tw_putc(win,*button++);
}


int     tw_button_response(win,button_line,buttons)
win_t   *win;
int     button_line;
char    *buttons[];

{
    event_t event;
    int     ch, center, start[MAX_BUTTONS], len[MAX_BUTTONS], c, total_len,
	    first_col, mouse_col, button_count, c2;
    char    key[MAX_BUTTONS+1];

    if ( buttons[0] == NULL )
	return 'y';
    
    /* Compute locations for buttons */
    for (c=0, total_len=0; (buttons[c]!=NULL) && (c<MAX_BUTTONS); ++c)
    {
	start[c] = total_len;
	key[c] = tw_button_action_key(buttons[c]);
	len[c] = strlen(buttons[c]);
	total_len += len[c]+2;
    }
    total_len -= 2;
    key[c] = '\0';
    button_count = c;

    /* Display all but first button */
    center = TW_COLS(win)/2;
    first_col = center - total_len/2;
    TW_SET_MODES(win,REVERSE_MODE);
    for (c=1; (buttons[c]!=NULL) && (c<MAX_BUTTONS); ++c)
	tw_draw_button(win,button_line,first_col+start[c],buttons[c]);\

    /* Get button selkection from user */
    c = 0;
    tpush_event_mask(win->terminal);
    TIGNORE_EVENT(win->terminal,MOTION);
    do
    {
	TW_SET_MODES(win,NORMAL_MODE);
	tw_draw_button(win,button_line,first_col+start[c],buttons[c]);\
	
	TW_CURSOR_INVISIBLE(win);
	ch = tw_get_event(win,&event);
	TW_CURSOR_NORMAL(win);
	TW_SET_MODES(win,REVERSE_MODE);

	/* slackware 3.2 isupper() doesn't verify isascii */
	if ( (ch < 128) && isupper(ch) )
	    ch = tolower(ch);
	
	switch(ch)
	{
	    case    KEY_MOUSE:
		if ( event.type == BUTTON1_PRESS )
		    if (event.mouse_line == button_line)
		    {
			mouse_col = event.mouse_col - first_col;
			for (c2=0; (buttons[c2] != NULL) && (c2<MAX_BUTTONS); ++c2)
			{
			    if ( (mouse_col>=start[c2]) && (mouse_col < start[c2]+len[c2]) )
				ch = key[c2];
			}
		    }
		break;
	    case    KEY_LEFT:
	    case    KEY_BTAB:
		tw_draw_button(win,button_line,first_col+start[c],buttons[c]);\
		c = c == 0 ? button_count-1 : c-1;
		break;
	    case    KEY_RIGHT:
	    case    '\t':
		tw_draw_button(win,button_line,first_col+start[c],buttons[c]);\
		c = (c+1) % button_count;
		break;
	    case    KEY_ENTER:
		return key[c];
	    default:
		break;
	}
    }   while ( strchr(key,ch) == NULL );
    tpop_event_mask(win->terminal);
    return ch;
}


int     tw_button_action_key(str)
char    *str;

{
    while ( !isalnum(*str) && (*str!='\0') )
	++str;
    return isupper(*str) ? tolower(*str) : *str;
}

