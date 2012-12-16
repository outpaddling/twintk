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

void        tredraw_line(terminal,line,leftcol,rightcol)
term_t      *terminal;
int    line,leftcol,rightcol;

{
    int    save_line,save_col;

    TFIND_CURSOR(terminal,save_line,save_col);
    tredraw_line_leave_cursor(terminal,line,leftcol,rightcol);
    tmove_to(terminal,save_line,save_col);
}


void        tredraw_line_leave_cursor(terminal,line,leftcol,rightcol)
term_t      *terminal;
int    line,leftcol,rightcol;

{
    unsigned char    *cptr,*endcptr;
    attr_t  *attrptr;
    int     current_mode, current_fg, current_bg;
    
    tmove_to(terminal,line,leftcol);
    attrptr = &TATTR(terminal,line,leftcol);
    cptr = &TCHAR(terminal,line,leftcol);
    endcptr = &TCHAR(terminal,line,rightcol);
    
    current_mode = TCUR_MODES(terminal);
    current_fg = TCUR_FOREGROUND(terminal);
    current_bg = TCUR_BACKGROUND(terminal);
    
    for ( ; cptr <= endcptr; ++cptr, ++attrptr)
    {
	if ( attrptr->foreground != current_fg )
	{
	    current_fg = attrptr->foreground;
	    tset_foreground(terminal,current_fg);
	}

	if ( attrptr->background != current_bg )
	{
	    current_bg = attrptr->background;
	    tset_background(terminal,current_bg);
	}
	
	/* Check attribute mode */
	if ( attrptr->mode != current_mode )
	{
	    current_mode = attrptr->mode;
	    tset_modes(terminal,current_mode);
	}
	
	/* Print character */
	putc(*cptr,terminal->fpout);
    }
    
    /*
    if ( TMOUSE_LINE(terminal) == line )
	tdisplay_mouse_pointer(terminal);
    */
}

