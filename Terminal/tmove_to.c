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
#include <stdlib.h>
#include "twintk_term.h"

int         tmove_to(terminal,line,col)
term_t      *terminal;
int    line,col;

{
    int     stat, old_modes;

    /*
    if ( (TCUR_LINE(terminal) == line) && (TCUR_COL(terminal) == col) )
	return SUCCESS;
    */
    
    if ( terminal->cursor_address != NULL )
    {
	if ( (stat=imove_to(&terminal->image,line,col)) == SUCCESS )
	{
	    if ( !terminal->move_standout_mode )
	    {
		old_modes = TCUR_MODES(terminal);
		tset_modes(terminal,NORMAL_MODE);
		raw_print(terminal,tparm(terminal->cursor_address,line,col));
		tset_modes(terminal,old_modes);
	    }
	    /*
	    else if ( strcmp(terminal->term_type,"xterm-color") == 0 )
	    {
		int     fg,bg;
		
		fg = TCUR_FOREGROUND(terminal);
		bg = TCUR_BACKGROUND(terminal);
		tset_foreground(terminal,TFOREGROUND(terminal,
			TCUR_LINE(terminal),TCUR_COL(terminal)));
		tset_background(terminal,TBACKGROUND(terminal,
			TCUR_LINE(terminal),TCUR_COL(terminal)));
		raw_print(terminal,tparm(terminal->cursor_address,line,col));
		TFLUSH_OUT(terminal);
		tset_foreground(terminal,fg);
		tset_background(terminal,bg);
	    }
	    */
	    else
		raw_print(terminal,tparm(terminal->cursor_address,line,col));

	}
	else
	    fprintf(stderr,"tmove_to(): Cannot update image.");
    }
    else
    {   
	fprintf(stderr,"tmove_to(): Terminal cannot address cursor.\n");
	exit(FAILURE);
    }
    return stat;
}
