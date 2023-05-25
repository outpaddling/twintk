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
#include "twintk_term.h"

int 
tset_modes (term_t *terminal, int mode)

{
    int     count = 0, fg, bg, save_mode = mode, color;

    /* Don't bother with cookie glitch terminals - screen flashes, etc. */
    if ( (terminal->magic_cookie_glitch > 0) )
    {
	ISET_MODES(&terminal->image,save_mode);
	return 0;
    }
    
    /* Some terminals can't change modes in color */
    fg = TCUR_FOREGROUND(terminal);
    bg = TCUR_BACKGROUND(terminal);
    color = TCOLOR_TERM(terminal) && ((fg != WHITE) || (bg != BLACK));

    /* Should be able to eliminate this */
    /* Needed for BSD console */
    if ( TCOLOR_TERM(terminal) )
    {
	tset_foreground(terminal,WHITE);
	tset_background(terminal,BLACK);
    }
    
    if ( mode & HIGHLIGHT_MODE )
    {
	if ( terminal->enter_standout_mode != NULL )
	    mode |= STANDOUT_MODE;
	else if ( terminal->enter_reverse_mode != NULL )
	    mode |= REVERSE_MODE;
	else if ( terminal->enter_underline_mode != NULL )
	    mode |= UNDERLINE_MODE;
	else if ( terminal->enter_blink_mode != NULL )
	    mode |= BLINK_MODE;
	else if ( terminal->enter_bold_mode != NULL )
	    mode |= BOLD_MODE;
	else if ( terminal->enter_dim_mode != NULL )
	    mode |= DIM_MODE;
    }

    /* Shut off all current attribute modes: return to normal mode */
    if (terminal->exit_attribute_mode != NULL)
    {
	++count;
	raw_print(terminal,terminal->exit_attribute_mode);
    }
    else    /* Works around missing exit_attr code in some cases */
    {
	/* adm5 uses same sequence to enter and exit standout */
	if ( (terminal->exit_standout_mode != NULL) &&
	    (TCUR_MODES(terminal) != NORMAL_MODE) )
	{
	    raw_print(terminal,terminal->exit_standout_mode);
	}
	if ( (terminal->exit_underline_mode != NULL ) &&
	    (TCUR_MODES(terminal)&UNDERLINE_MODE) )
	{
	    raw_print(terminal,terminal->exit_underline_mode);
	}
    }
    if ( terminal->exit_alt_charset_mode != NULL )
	raw_print(terminal,terminal->exit_alt_charset_mode);

    /* SCO OS5 - ACS mode must come before REVERSE_MODE */
    if ( (mode & ACS_MODE) && (terminal->enter_alt_charset_mode != NULL) )
    {
	++count;
	raw_print(terminal,terminal->enter_alt_charset_mode);
    }

    /* Exit standout resets colors.  This is bad on xterm or rxvt
       with a white on black scheme, since we want black on white
       as a default */
    if ( TCOLOR_TERM(terminal) )
    {
	tset_foreground(terminal,fg);
	tset_background(terminal,bg);
    }
    
    if ( (mode & REVERSE_MODE) && (terminal->enter_reverse_mode != NULL) &&
	 !(color && TNCV(terminal,NCV_REVERSE)) )
    {
	++count;
	raw_print(terminal,terminal->enter_reverse_mode);
    }
	
    if ( (mode & BLINK_MODE) && (terminal->enter_blink_mode != NULL) &&
	 !(color && TNCV(terminal,NCV_BLINK)) )
    {
	++count;
	raw_print(terminal,terminal->enter_blink_mode);
    }
	
    if ((mode & BOLD_MODE) && (terminal->enter_bold_mode != NULL)
	 && !(color && TNCV(terminal,NCV_BOLD)) )
    {
	++count;
	/*if ( memcmp(terminal->term_type,"xterm",5) != 0 )*/
	    raw_print(terminal,terminal->enter_bold_mode);
    }
	
    if ( (mode & DIM_MODE) && (terminal->enter_dim_mode != NULL) &&
	 !(color && TNCV(terminal,NCV_DIM)) )
    {
	++count;
	raw_print(terminal,terminal->enter_dim_mode);
    }
	
    if ( (mode & UNDERLINE_MODE) && (terminal->enter_underline_mode != NULL) &&
	 !(color && TNCV(terminal,NCV_UNDERLINE)) )
    {
	++count;
	raw_print(terminal,terminal->enter_underline_mode);
    }
	
    if ( (mode & STANDOUT_MODE) && (terminal->enter_standout_mode != NULL) &&
	 !(color && TNCV(terminal,NCV_STANDOUT)) )
    {
	++count;
	raw_print(terminal,terminal->enter_standout_mode);
    }

    ISET_MODES(&terminal->image,save_mode);
    return(count);
}

