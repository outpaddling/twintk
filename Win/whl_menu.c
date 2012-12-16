#include <ctype.h>
#include <string.h>
#include "twintk.h"

void    highlight_last_char(win_t *win, int submenu)

{
    if ( submenu )
	tw_putc(win, '>');
    else
	tw_putc(win, ' ');
}


void    tw_highlight_menu_item(win,text,row,no_color,reverse_menu,
			    fg,bg,hl_fg,hl_bg,main_mode,highlight_mode,lc,rc)
win_t   *win;
char    *text;
int     row, no_color, reverse_menu,
	fg,bg,hl_fg,hl_bg,main_mode,highlight_mode,lc,rc;

{
    int     c, ch, submenu;
    char    *text_ptr;
    
    if ( memcmp(text, "SUBMENU", 7) == 0 )
    {
	text_ptr = text + 8;
	submenu = 1;
    }
    else
    {
	text_ptr = text;
	submenu = 0;
    }
    
    if ( reverse_menu )
    {
	tw_move_to(win,row,1);
	if ( TCOLOR_TERM(win->terminal) && !no_color )
	{
	    tw_set_foreground(win,fg);
	    tw_set_background(win,bg);
	    tw_putc(win,' ');
	    for (c=0; *text_ptr != '\0'; ++c)
	    {
		if ( *text_ptr == '.' )
		{
		    ++text_ptr;
		    tw_set_foreground(win,hl_fg);
		    tw_set_background(win,hl_bg);
		    if ( islower(ch = *text_ptr++) )
			ch = toupper(ch);
		    tw_putc(win,ch);
		    tw_set_foreground(win,fg);
		    tw_set_background(win,bg);
		}
		else
		    tw_putc(win,*text_ptr++);
	    }
	    while ( c++ < TW_COLS(win)-4 )
		tw_putc(win,' ');
	    highlight_last_char(win, submenu);
	}
	else
	{
	    TW_SET_MODES(win,main_mode);
	    tw_putc(win,' ');
	    for (c=0; *text_ptr != '\0'; ++c)
	    {
		if ( *text_ptr == '.' )
		{
		    ++text_ptr;
		    TW_SET_MODES(win,highlight_mode);
		    if ( islower(ch = *text_ptr++) )
			ch = toupper(ch);
		    tw_putc(win,ch);
		    TW_SET_MODES(win,main_mode);
		}
		else
		    tw_putc(win,*text_ptr++);
	    }
	    while ( c++ < TW_COLS(win)-4 )
		tw_putc(win,' ');
	    highlight_last_char(win, submenu);
	}
    }
    else
    {
	tw_move_to(win,row,1);
	tw_putc(win,lc);
	tw_move_to(win,row,TW_COLS(win)-2);
	tw_putc(win,rc);
    }
}

