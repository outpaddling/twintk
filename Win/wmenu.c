#include <string.h>
#include "twintk.h"

win_t  *tw_menu(terminal, row, col, text, border, no_acs, no_color,
	     foreground, background, hl_foreground, hl_background)
term_t      *terminal;
int         row, col;
char        *text[];
border_t    *border;
int         no_acs, no_color, foreground, background,
	    hl_foreground, hl_background;

{
    win_t  *win;
    int     s, c, len, maxlen, cur_modes, flags = NO_AUTO_SCROLL|SHADOW;
    image_t *image;

    maxlen = strlen(text[0]);
    for (c = 0; *text[c] != '\0'; ++c)
    {
	if ((len = strlen(text[c])) > maxlen)
	    maxlen = len - 1;
    }

    if ( no_color )
	flags |= NO_COLOR;
    if ( no_acs )
	flags |= NO_ACS;
    if ( (win = tw_new_win(terminal, c + 2, maxlen + 4, row, col, flags)) == NULL )
	return NULL;
    
    tw_set_win_attr(win,REVERSE_MODE,foreground,background,
		    BOLD_MODE,hl_foreground,hl_background);
    tw_shadow(win);
    TW_SET_BORDER(win,*border);
    
    if (win != NULL)
    {
	image = &win->image;
	if ( TCOLOR_TERM(terminal) && !no_color )
	{
	    ISET_FOREGROUND(image,foreground);
	    ISET_BACKGROUND(image,background);
	    /* Normal */
	    tw_draw_border(win);
	}
	else
	{
	    /* Reverse */
	    if ( THAS_REVERSE_MODE(terminal) )
		TW_SET_MODES(win, REVERSE_MODE);   /* Also for iputc below */
	    else
		TW_SET_MODES(win, HIGHLIGHT_MODE);
	    tw_draw_border(win);
	}
    
	for (s = 0; *text[s] != '\0'; ++s)
	{
	    // FIXME: Change '1' to something descriptive throughout
	    if ( *text[s] == *TWC_HLINE )
	    {
		if ( THAS_ACS(terminal) && !no_acs )
		{
		    imove_to(image, s+1, 0);
		    cur_modes = ICUR_MODES(image);
		    ISET_MODES(image,cur_modes|ACS_MODE);
		    iputc(image,ACS_LTEE(terminal));
		    for (c=0; c<TW_COLS(win)-2; ++c)
			iputc(image,ACS_HLINE(terminal));
		    iputc(image,ACS_RTEE(terminal));
		    ISET_MODES(image,cur_modes);
		}
		else
		{
		    imove_to(image, s+1, 0);
		    iputc(image,LEFT_TEE(border));
		    for (c=0; c<TW_COLS(win)-2; ++c)
			iputc(image,DIVIDER(border));
		    iputc(image,RIGHT_TEE(border));
		}
	    }
	    else
	    {
		char    *text_start;
		
		imove_to(image, s + 1, 1);
		iputc(image, ' ');
		if ( memcmp(text[s], "SUBMENU", 7) == 0 )
		    text_start = text[s] + 8;
		else
		    text_start = text[s];

		tw_high_print(win, foreground, background,
			hl_foreground, hl_background, text_start);
		for (c = strlen(text_start); c < maxlen + 1; ++c)
		    iputc(image, ' ');
		if ( memcmp(text[s], "SUBMENU", 7) == 0 )
		    iputc(image, '>');
		else
		    iputc(image, ' ');
	    }
	}
	TW_REDRAW_WIN(win);
    }
    return (win);
}

