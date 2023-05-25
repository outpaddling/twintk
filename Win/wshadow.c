#include "twintk.h"

void 
tw_shadow (win_t *win)

{
    if ( !TW_HAS_SHADOW(win) )
    {
	fprintf(stderr,"shadow(): Window was not defined with a shadow.\n");
	return;
    }
    if ( THAS_DIM_MODE(win->terminal) )
	tw_dim_shadow(win);
    else if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
	tw_blank_shadow(win);
    else if ( THAS_ACS(win->terminal) )
	tw_checker_shadow(win);
    else
	tw_blank_shadow(win);
}


/* Make shadow border */

void 
tw_checker_shadow (win_t *win)

{
    win_t   *shadow_win = win->parent_win;
    image_t *image = &win->parent_win->image;
    int     c, shadow = ACS_CKBOARD(win->terminal);
    int    original_mode = ICUR_MODES(image);
    
    if ( !TW_HAS_SHADOW(win) )
    {
	fprintf(stderr,"tw_checker_shadow(): Window was not defined with a shadow.\n");
	return;
    }
    
    /* Underneath shadow */
    /* See-through corner */
    imove_to(image,ILINES(image)-1,0);
    if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
    {
	ISET_FOREGROUND(image,TFOREGROUND(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
	ISET_BACKGROUND(image,TBACKGROUND(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
    }
    else
	ISET_MODES(image,TMODES(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
    
    /* Checker shadow */
    if ( THAS_ACS(win->terminal) )
	ISET_MODES(image,/*ICUR_MODES(image)|*/ACS_MODE);
    else
	ISET_MODES(image,NORMAL_MODE);
    for (c=0; c<TW_COLS(shadow_win)-1; ++c)
    {
	iputc(image,shadow);
    }
    ISET_MODES(image,NORMAL_MODE);
    
    /* Side shadow */
    /* See-through corner */
    imove_to(image,0,TW_COLS(shadow_win)-1);
    if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
    {
	ISET_FOREGROUND(image,TFOREGROUND(win->terminal,TW_SCREEN_LINE(shadow_win),TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
	ISET_BACKGROUND(image,TBACKGROUND(win->terminal,TW_SCREEN_LINE(shadow_win),TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
    }
    else
	ISET_MODES(image,TMODES(win->terminal,TW_SCREEN_LINE(shadow_win),
				TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win),TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
    
    /* Shadow */
    for (c=0; c<TW_LINES(shadow_win)-2; ++c)
    {
	if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
	{
	    ISET_FOREGROUND(image,TFOREGROUND(win->terminal,TW_SCREEN_LINE(shadow_win)+1+c,TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
	    ISET_BACKGROUND(image,TBACKGROUND(win->terminal,TW_SCREEN_LINE(shadow_win)+1+c,TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
	}
	else
	    ISET_MODES(image,NORMAL_MODE);
	if ( THAS_ACS(win->terminal) )
	    ISET_MODES(image,ICUR_MODES(image)|ACS_MODE);
	iaddc(image,c+1,TW_COLS(shadow_win)-1,shadow);
    }
    ISET_MODES(image,original_mode);
}


/* Make shadow border */

void 
tw_blank_shadow (win_t *win)

{
    win_t   *shadow_win = win->parent_win;
    image_t *image = &win->parent_win->image;
    int     c, shadow = ' ', save_bg = ICUR_BACKGROUND(image);
    int    original_mode = ICUR_MODES(image);
    
    if ( !TW_HAS_SHADOW(win) )
    {
	fprintf(stderr,"tw_blank_shadow(): Window was not defined with a shadow.\n");
	return;
    }
    
    /* Underneath shadow */
    imove_to(image,TW_LINES(shadow_win)-1,0);
    tw_match_term_ch(shadow_win,TW_LINES(shadow_win)-1,0);
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
    
    ISET_MODES(image,NORMAL_MODE);
    ISET_BACKGROUND(image,BLACK);
    for (c=0; c<TW_COLS(shadow_win)-1; ++c)
	iputc(image,shadow);
    
    /* Side shadow */
    imove_to(image,0,TW_COLS(shadow_win)-1);
    tw_match_term_ch(shadow_win,0,TW_COLS(shadow_win)-1);
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win),TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
    
    ISET_MODES(image,NORMAL_MODE);
    ISET_BACKGROUND(image,BLACK);
    for (c=0; c<TW_LINES(shadow_win)-1; ++c)
	iaddc(image,c+1,TW_COLS(shadow_win)-1,shadow);
    
    ISET_MODES(image,original_mode);
    ISET_BACKGROUND(image,save_bg);
}


void 
tw_dim_shadow (win_t *win)

{
    win_t   *shadow_win = win->parent_win;
    image_t *image = &win->parent_win->image;
    term_t  *terminal = win->terminal;
    int     c, tline, tcol;
    int    original_mode = ICUR_MODES(image);
    
    if ( !TW_HAS_SHADOW(win) )
    {
	fprintf(stderr,"tw_dim_shadow(): Window was not defined with a shadow.\n");
	return;
    }
    
    /* Underneath shadow */
    imove_to(image,ILINES(image)-1,0);
    tw_match_term_ch(shadow_win,TW_LINES(shadow_win)-1,0);
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1,TW_SCREEN_COL(shadow_win)));
    ISET_FOREGROUND(image,WHITE);
    ISET_BACKGROUND(image,BLACK);
    tline = TW_SCREEN_LINE(shadow_win)+TW_LINES(shadow_win)-1;
    for (c=0; c<TW_COLS(shadow_win)-1; ++c)
    {
	tcol = TW_SCREEN_COL(shadow_win)+c+1;
	ISET_MODES(image,IMODES(&terminal->image,tline,tcol)|DIM_MODE);
	iputc(image,TCHAR(win->terminal,tline,tcol));
    }
    
    /* Side shadow */
    imove_to(image,0,TW_COLS(shadow_win)-1);
    tw_match_term_ch(shadow_win,0,TW_COLS(shadow_win)-1);
    iputc(image,TCHAR(win->terminal,TW_SCREEN_LINE(shadow_win),TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1));
    ISET_FOREGROUND(image,WHITE);
    ISET_BACKGROUND(image,BLACK);
    tcol = TW_SCREEN_COL(shadow_win)+TW_COLS(shadow_win)-1;
    for (c=0; c<TW_LINES(shadow_win)-1; ++c)
    {
	tline = TW_SCREEN_LINE(shadow_win)+c+1;
	ISET_MODES(image,IMODES(&terminal->image,tline,tcol)|DIM_MODE);
	iaddc(image,c+1,TW_COLS(shadow_win)-1,TCHAR(win->terminal,tline,tcol));
    }
    ISET_MODES(image,original_mode);
}


void 
tw_match_term_ch (win_t *win, int line, int col)

{
    line += TW_SCREEN_LINE(win);
    col += TW_SCREEN_COL(win);
    if ( TCOLOR_TERM(win->terminal) && !TW_NO_COLOR(win) )
    {
	ISET_FOREGROUND(&win->image,TFOREGROUND(win->terminal,line,col));
	ISET_BACKGROUND(&win->image,TBACKGROUND(win->terminal,line,col));
    }
    ISET_MODES(&win->image,TMODES(win->terminal,line,col));
}

