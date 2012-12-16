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

void    tclear_eos(terminal)
term_t  *terminal;

{   int    l,start_line,start_col;
    
    /* Use terminal magic sequence if possible */
    if ( (terminal->clr_eos != NULL) && (terminal->back_color_erase ||
	!TCOLOR_TERM(terminal)) )
	/*|| (TCUR_BACKGROUND(terminal) == BLACK)) )*/
    {   
	/* Clear physical screen to eos */
	raw_print(terminal,terminal->clr_eos);
	
	/* Clear image to eos */
	iclear_eos(&terminal->image);
    }
    else    /* Brute force method */
    {
	TFIND_CURSOR(terminal,start_line,start_col);
	tclear_eol(terminal);
	for (l=TCUR_LINE(terminal)+1; l<terminal->lines; ++l)
	{
	    tmove_to(terminal,l,0);
	    tclear_eol(terminal);
	}
	tmove_to(terminal,start_line,start_col);
    }
}


