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

void    tscroll_forward(terminal)
term_t *terminal;

{
    int     hard_scroll;

    /* Get current screen position */
    // TFIND_CURSOR(terminal,curline,curcol);
    
    /* Scroll screen image */
    iscroll_forward(&terminal->image,TSCROLL_TOP(terminal),
	TSCROLL_BOTTOM(terminal));

    /* Scroll terminal */
    hard_scroll = (terminal->scroll_forward != NULL) &&
		  ((terminal->change_scroll_region != NULL) ||
		   ((TSCROLL_TOP(terminal) == 0) &&
		   (TSCROLL_BOTTOM(terminal) == TLINES(terminal)-1)));
    if ( hard_scroll )
    {
	tmove_to(terminal,TSCROLL_BOTTOM(terminal),TCOLS(terminal)-1);
	raw_print(terminal,terminal->scroll_forward);
    }
    else
    {
	tredraw(terminal,TSCROLL_TOP(terminal),0,
		TSCROLL_BOTTOM(terminal),TCOLS(terminal)-1);
    }
}

