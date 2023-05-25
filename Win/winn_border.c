#include "twintk.h"

void 
tw_draw_border (win_t *win)

{
    int     c, top_left, top, top_right, right,
	    bottom_right, bottom, bottom_left, left;
    int    original_mode = TW_CUR_MODES(win);
    image_t *image = &(win->image);

    if ( !TW_NO_ACS(win) )
    {
	top_left = ACS_ULCORNER(win->terminal);
	top = ACS_HLINE(win->terminal);
	top_right = ACS_URCORNER(win->terminal);
	right = ACS_VLINE(win->terminal);
	bottom_right = ACS_LRCORNER(win->terminal);
	bottom = ACS_HLINE(win->terminal);
	bottom_left = ACS_LLCORNER(win->terminal);
	left = ACS_VLINE(win->terminal);
	if ( THAS_ACS(win->terminal) )
	    ISET_MODES(image,original_mode|ACS_MODE);
    }
    else
    {
	top_left = UPPER_LEFT(&image->border);
	top = UPPER(&image->border);
	top_right = UPPER_RIGHT(&image->border);
	right = RIGHT(&image->border);
	bottom_right = LOWER_RIGHT(&image->border);
	bottom = LOWER(&image->border);
	bottom_left = LOWER_LEFT(&image->border);
	left = LEFT(&image->border);
    }

    /* Should use IADDC instead of iputc */
    /* Draw top line */
    imove_to(image, 0, 0);
    iputc(image, top_left);
    for (c = 0; c < ICOLS(image) - 2; ++c)
	iputc(image, top);
    iputc(image, top_right);

    /* Draw side lines */
    for (c = 0; c < ILINES(image) - 2; ++c)
    {
	imove_to(image, c + 1, 0);
	iputc(image, left);
	imove_to(image, c + 1, ICOLS(image) - 1);
	iputc(image, right);
    }

    /* Draw bottom line */
    imove_to(image, ILINES(image) - 1, 0);
    iputc(image, bottom_left);
    for (c = 0; c < ICOLS(image) - 2; ++c)
	iputc(image, bottom);
    iputc(image, bottom_right);
    
    /* Restore original mode */
    TW_SET_MODES(win,original_mode);
}

