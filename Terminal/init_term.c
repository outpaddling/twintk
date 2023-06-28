/*
    Copyright (c) 1994-2003, Jason W. Bacon, Acadix Software Systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer. Redistributions
    in binary form must reproduce the above copyright notice, this list of
    conditions and the following disclaimer in the documentation and/or
    other materials provided with the distribution. 

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sysexits.h>
#include <xtend/string.h>   // strblank()
#include "twintk_term.h"

term_t *
init_term (FILE *fpin, FILE *fpout, FILE *fperr, char *termtype, unsigned flags)

{
    term_t  *terminal;
    int     error;
    char    *buff;
    
    if ( !isatty(fileno(stdin)) || !isatty(fileno(stdout)) ||
	 !isatty(fileno(stderr)) )
    {
	fputs("One of the standard streams is not a tty.  Aborting...\n", stderr);
	exit(EX_USAGE);
    }
    
    /* Allocate terminal structure */
    if ( (terminal = MALLOC(1,term_t)) == NULL )
    {
	fprintf(stderr,"init_term(): Cannot allocate structure.\n");
	exit(EX_UNAVAILABLE);
    }
    
    /* Load terminfo data */
    if ( termtype == NULL )
    {
	termtype = getenv("TWINTERM");
	if ( (termtype == NULL) || strblank(termtype))
	    termtype = getenv("COLORTERM");
	if ( (termtype == NULL) || strblank(termtype) )
	    termtype = getenv("TERM");
	if ( (termtype == NULL) || strblank(termtype) )
	{
	    fprintf(stderr,"init_term(): getenv(\"TERM\") failed.\n");
	    return(NULL);
	}
    }
    strlcpy(terminal->term_type,termtype,MAX_TERM_NAME);
    
    error = load_tinfo(termtype,terminal);
    switch(error)
    {
	case    -1:
	    exit(1);
	case    0:
	    break;
	default:
	    fprintf(stderr,
		"read_terminfo(): Weird return value from load_tinfo().\n");
	    exit(1);
    }

#ifdef COHERENT
    if ( memcmp(terminal->term_type,"ansipc",6) == 0 )
	terminal->buttons = 2;
#endif

    /* Initialize alternate character set map */
    init_acsmap(terminal);

    /* Initialize window list for wredraw_all */
    terminal->winlist_head = terminal->winlist_tail = NULL;
    
    /* Resize - term_type field must be set! */
    resize_terminal(terminal);
    
    /* Initialize terminal */
    terminal->use_delays = 1;   /* Use delays after each magic sequence */
    terminal->event_mask = KEY_PRESS|BUTTON1_PRESS|BUTTON3_PRESS|MOTION;
    terminal->event_top = 0;
    terminal->button_status = BUTTON_UP;
    terminal->windowid = -1;
    
    /* Get terminal settings and initialize image mode stack */
    tcgetattr(fileno(fpout),&terminal->tty_mode);
    terminal->startup_tty_mode = terminal->tty_mode;
    terminal->mode_stack = NULL;
    tpush_tty(terminal);

    /* Initialize file streams */
    terminal->fpin = fpin;
    terminal->fpout = fpout;
    terminal->fperr = fperr;
    INIT_MOUSE(terminal->mouse);
    terminal->mouse_pid = -1;
    *terminal->mouse_pipe = '\0';
    terminal->mouse_pointer_visible = TRUE;

    /* Init mouse position */
    terminal->mouse_x_scale = 5;
    terminal->mouse_y_scale = 9;
    terminal->scaled_mouse_line = 0;
    terminal->scaled_mouse_col = 0;
    
    /* Init keymap */
    terminal->max_keys = INITIAL_KEYS;
    terminal->keymap = MALLOC(terminal->max_keys,sequ_t);

    /* Init input mask */
    TSELECT_INPUT(terminal,KEY_PRESS|BUTTON_PRESS|BUTTON_RELEASE);
    
    if ( (buff = MALLOC(TBUFF_SIZE,char)) == NULL )
    {
	fprintf(fperr,"init_term(): Cannot malloc output buffer.\n");
	exit(1);
    }
    fflush(terminal->fpout);
    setvbuf(terminal->fpout,buff,_IOFBF,TBUFF_SIZE);
    if ( flags & MOUSE_ON )
	tadd_key_mouse(terminal);
    init_keymap(terminal);
    
    /* Force tset_foreground() and tset_background() to send 
       B&W color magic sequences in case terminal has been left in
       some other colors */
    if ( TCOLOR_TERM(terminal) )
    {
	ISET_FOREGROUND(&terminal->image,-1);
	ISET_BACKGROUND(&terminal->image,-1);
    }
    
    /* Set terminal to white on black */
    tset_foreground(terminal,WHITE);
    tset_background(terminal,BLACK);
    
    TENABLE_ACS(terminal);
    return(terminal);
}


void 
init_acsmap (term_t *terminal)

{
    unsigned char    *p;
#ifdef osf1
    unsigned char    *p2;
#endif
    int     c;
    
    /* Init to defaults in case ACS char not present */
    for (c=0; c<256; ++c)
	terminal->acs_map[c] = ACS_MISSING;

    /* Some default look-alikes for common ACS chars */
    ACS_ULCORNER(terminal)  |= '*';
    ACS_LLCORNER(terminal)  |= '*';
    ACS_URCORNER(terminal)  |= '*';
    ACS_LRCORNER(terminal)  |= '*';
    ACS_RTEE(terminal)      |= '|';
    ACS_LTEE(terminal)      |= '|';
    ACS_BTEE(terminal)      |= '-';
    ACS_TTEE(terminal)      |= '-';
    ACS_HLINE(terminal)     |= '-';
    ACS_VLINE(terminal)     |= '|';
    ACS_PLUS(terminal)      |= '+';
    ACS_S1(terminal)        |= '-';
    ACS_S9(terminal)        |= '_';
    ACS_DIAMOND(terminal)   |= '*';
    ACS_CKBOARD(terminal)   |= '#';
    ACS_DEGREE(terminal)    |= '`';
    ACS_PLMINUS(terminal)   |= '~';
    ACS_BULLET(terminal)    |= 'o';
    ACS_LARROW(terminal)    |= '<';
    ACS_RARROW(terminal)    |= '>';
    ACS_UARROW(terminal)    |= '^';
    ACS_DARROW(terminal)    |= 'v';
    ACS_BOARD(terminal)     |= '#';
    ACS_LANTERN(terminal)   |= '&';
    ACS_BLOCK(terminal)     |= ' ';
    
    if ( terminal->acs_chars != NULL )
    {
#if 0 /* #ifdef osf1 */
	/* DEC bug: ACS map stored as adjacent sets instead of adjacent characters */
	for (p=terminal->acs_chars, p2=terminal->char_padding; *p != '\0'; ++p, ++p2)
	{
	    terminal->acs_map[*p] = *p2;
	}
#else   /* not osf1 */
	for (p=(unsigned char *)terminal->acs_chars; *p != '\0'; p+=2)
	    terminal->acs_map[*p] = *(p+1);
#endif  /* osf1 */
    }
}


void 
resize_terminal (term_t *terminal)

{
    int     still_no_size;
    struct winsize  wsize;
    char    *termcap, *li = NULL, *co = NULL;
    
    /* Check environment for window size override */
    /* Can't trust TIOGWINSIZE on some systems */
    if ( (li = getenv("LINES")) != NULL )
	terminal->lines = atoi(li);
    if ( (co = getenv("COLUMNS")) != NULL )
	terminal->columns = atoi(co);

    /* Ask driver for window size if not specified in env */
    if ( (li == NULL) && (co == NULL) )
    {
	if ( ioctl(1,TIOCGWINSZ,&wsize) != -1 )
	{
	    still_no_size = FALSE;
	    if ( wsize.ws_row )
		terminal->lines = wsize.ws_row;
	    if ( wsize.ws_col )
		terminal->columns = wsize.ws_col;
	}
	else
	    still_no_size = TRUE;
    }
    else
	still_no_size = FALSE;
    
    /* Check TERMCAP variable for xterm size.  This is the least reliable
     * source since it's usually just fixed values from the termcap entry */
    if ( still_no_size )
    {
	if ( (termcap = getenv("TERMCAP")) != NULL )
	{
	    if ( (li = strstr(termcap,"li#")) != NULL)
		terminal->lines = atoi(li+3);
	    if ( (co = strstr(termcap,"co#")) != NULL)
		terminal->columns = atoi(co+3);
	}
    }
    
    /* Set up screen image buffer */
    new_image(&terminal->image,terminal->lines,terminal->columns,PLAIN);
    ISET_AUTO_SCROLL(&terminal->image);
}
