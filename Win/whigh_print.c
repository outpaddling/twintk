#include <ctype.h>
#include "twintk.h"

/**************************************************************
 * Print a string in reverse video with characters preceded by
 * a '.' in normal text.
 **************************************************************/

void    tw_high_print(win, fg, bg, hlfg, hlbg, text)
win_t   *win;
int     fg,bg,hlfg,hlbg;
char   *text;

{
    int     c, ch;
    term_t  *terminal = win->terminal;
    image_t *image = &win->image;

    if ( TCOLOR_TERM(terminal) && !TW_NO_COLOR(win) )
    {
	ISET_FOREGROUND(image, fg);
	ISET_BACKGROUND(image, bg);
	for (c=0; text[c] != '\0'; ++c)
	{
	    if ( text[c] == '.' )
	    {
		ISET_FOREGROUND(image,hlfg);
		ISET_BACKGROUND(image,hlbg);
		if ( islower(ch = text[++c]) )
		    ch = toupper(ch);
		iputc(image,ch);
		ISET_FOREGROUND(image,fg);
		ISET_BACKGROUND(image,bg);
	    }
	    else
		iputc(image, text[c]);
	}
    }
    else
    {
	int     mode;
	
	if ( THAS_REVERSE_MODE(terminal) )
	    mode = REVERSE_MODE;
	else
	    mode = HIGHLIGHT_MODE;
	
	ISET_MODES(image, mode);
	for (c = 0; text[c] != '\0'; ++c)
	{
	    if (text[c] == '.')
	    {
		ISET_MODES(image, BOLD_MODE);
		if ( islower(ch = text[++c]) )
		    ch = toupper(ch);
		iputc(image,ch);
		ISET_MODES(image, mode);
	    }
	    else
		iputc(image, text[c]);
	}
    }
}

