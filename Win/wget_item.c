#include <string.h>
#include <ctype.h>
#include <xtend/string.h>   // strlcpy() on Linux
#include "twintk.h"

#define MOVE_UP     '\003'
#define MOVE_RIGHT  '\026'
#define MOVE_DOWN   '\030'
#define MOVE_LEFT   '\032'

/**
 *
 *  \param  selected_text gets a copy of the menu text if not null
 */

int 
tw_get_item (win_t *win, char *menu_text[], event_t *event, int *start_line, int reverse_menu, char selected_text[])

{
    int         line = *start_line,
		old_line = -1,
		save_line,
		ch,
		cooked,
		done=FALSE;
    char        *dot_ptr;
    static int  last_key = 0;
    term_t      *term = win->terminal;
    
    /* Check terminal modes */
    cooked = term->tty_mode.c_lflag & (IEXTEN|ICANON|ISIG|ECHO);
    if ( cooked )
    {
	tpush_tty(term);
	tunset_tty(term,C_LFLAG,IEXTEN|ICANON|ISIG|ECHO);
    }

    tpush_event_mask(win->terminal);
    TIGNORE_EVENT(win->terminal,MOTION);
    do
    {
	/* Highlight current line */
	if ( line != old_line )
	    tw_highlight_menu_item(win,menu_text[line-1],line,
			    TW_NO_COLOR(win),reverse_menu,
			    TW_NORMAL_BG(win), TW_NORMAL_FG(win),
			    TW_HIGHLIGHT_BG(win), TW_HIGHLIGHT_FG(win),
			    TW_HIGHLIGHT_MODE(win), TW_NORMAL_MODE(win),
			    '>','<');
	
	TW_CURSOR_INVISIBLE(win);
	ch = tw_get_event(win,event);
	switch(ch)
	{
	    case    KEY_MOUSE:
		if (((event->type == BUTTON1_PRESS) &&
		     (event->repeat == 0)) ||
		     (event->type == BUTTON1_RELEASE) ||
		     (event->type == BUTTON1_MOTION) )
		{
		    ch = mouse_action(win,menu_text,event);
		    if (ch == TWC_OUTSIDE_WIN)
		    {
			if ( event->type == BUTTON1_PRESS )
			    done = TRUE;
		    }
		    else if (ch != TWC_HIT_HLINE)
		    {
			if ( event->mouse_line != line )
			{
			    tw_highlight_menu_item(win,menu_text[line-1],line,
					TW_NO_COLOR(win),reverse_menu,
					TW_NORMAL_FG(win),TW_NORMAL_BG(win),
					TW_HIGHLIGHT_FG(win),TW_HIGHLIGHT_BG(win),
					TW_NORMAL_MODE(win),TW_HIGHLIGHT_MODE(win),
					' ',' ');
			    line = event->mouse_line;
			    tw_highlight_menu_item(win,menu_text[line-1],line,
					TW_NO_COLOR(win),reverse_menu,
					TW_NORMAL_BG(win),TW_NORMAL_FG(win),
					TW_HIGHLIGHT_BG(win),TW_HIGHLIGHT_FG(win),
					TW_HIGHLIGHT_MODE(win),TW_NORMAL_MODE(win),
					'>','<');
			}
			done = (event->type == BUTTON1_RELEASE);
		    }
		}
		break;
	    case    KEY_UP:
	    case    MOVE_UP:
		tw_highlight_menu_item(win,menu_text[line-1],line,
				    TW_NO_COLOR(win),reverse_menu,
				    TW_NORMAL_FG(win),TW_NORMAL_BG(win),
				    TW_HIGHLIGHT_FG(win),TW_HIGHLIGHT_BG(win),
				    TW_NORMAL_MODE(win),TW_HIGHLIGHT_MODE(win),
				    ' ',' ');
		--line;
		if ( (line>0) && (*menu_text[line-1] == *TWC_HLINE) )
		    --line;
		if ( line == 0 )
		{
		    line = TW_LINES(win)-2;
		    if ( *menu_text[line-1] == *TWC_HLINE )
			--line;
		}
		break;
	    case    KEY_DOWN:
	    case    MOVE_DOWN:
		tw_highlight_menu_item(win,menu_text[line-1],line,
				    TW_NO_COLOR(win),reverse_menu,
				    TW_NORMAL_FG(win),TW_NORMAL_BG(win),
				    TW_HIGHLIGHT_FG(win),TW_HIGHLIGHT_BG(win),
				    TW_NORMAL_MODE(win),TW_HIGHLIGHT_MODE(win),
				    ' ',' ');
		++line;
		if ( *menu_text[line-1] == *TWC_HLINE )
		    ++line;
		if ( line >= TW_LINES(win)-1 )
		    line = 1;
		break;
	    case    KEY_ENTER:
	    case    '\r':
	    case    '\n':
		dot_ptr = strchr(menu_text[line-1],'.');
		if ( dot_ptr != NULL )
		    ch = dot_ptr[1];
		done = TRUE;
		break;
	    case    KEY_LEFT:
	    case    KEY_RIGHT:
	    case    MOVE_LEFT:
	    case    MOVE_RIGHT:
	    case    TW_ESC:
	    case    ' ':
		done = TRUE;
		break;
	    default:
		/* Determine line so we can set selected_text */
		save_line = line;
		for (line=0; (*menu_text[line] != '\0'); ++line)
		{
		    if ( ((dot_ptr = strchr(menu_text[line],'.')) != NULL)
			    && (tolower(dot_ptr[1]) == tolower(ch)) )
		    {
			done = TRUE;
			++line;
			break;
		    }
		}
		if ( !done )
		    line = save_line;
		break;
	}
    }   while ( !done );
    
    *start_line = line;
    /* isascii() broken in RedHat, isupper() in solaris gcc */
    last_key = (ch < 128) && isupper(ch) ? tolower(ch) : ch;
    TW_CURSOR_NORMAL(win);
    if ( cooked )
	tpop_tty(term);
    tpop_event_mask(win->terminal);
    if ( selected_text != NULL )
	strlcpy(selected_text,menu_text[line-1],TWC_MENU_TEXT_LEN);
    return last_key;    /* Also held here as static */
}


int 
mouse_action (win_t *win, char *menu_text[], event_t *event)

{
    char    *dot_ptr;

    /* If mouse click in window, return menu item */
    if ( (event->mouse_line>0) && (event->mouse_line < TW_LINES(win)-1) &&
	 (event->mouse_col>0) && (event->mouse_col<TW_COLS(win)-1) )
    {
	if ( *menu_text[event->mouse_line-1] != *TWC_HLINE )
	{
	    dot_ptr = strchr(menu_text[event->mouse_line-1],'.');
	    if ( dot_ptr != NULL )
		return dot_ptr[1];
	}
	else
	    return TWC_HIT_HLINE;
    }
    
    /* If not, convert to terminal coordinates and return */
    event->mouse_line += TW_SCREEN_LINE(win);
    event->mouse_col += TW_SCREEN_COL(win);
    return TWC_OUTSIDE_WIN;
}

