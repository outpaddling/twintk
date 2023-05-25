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
#include "twintk_term.h"

void 
tputc (term_t *terminal, int ch)

{
    static int     wrapped = 0;

#if 0
    acs_missing = (ch & ACS_MISSING) && (TCUR_MODES(terminal) & ACS_MODE);
    if ( acs_missing )
    {
	modes = TCUR_MODES(terminal);
	tset_modes(terminal,modes&~ACS_MODE);
	ch &= ~ACS_MISSING;     /* Strip off ACS_MISSING flag */
    }
#endif

    /* Check for insert mode */
    if ( TINSERT_MODE_ON(terminal) )
    {
	tinsert_ch(terminal,ch);
    }
    else
    {
	/* Output to terminal */
	putc(ch,terminal->fpout);
    
	/* Scroll manually if newline glitch */
	if ( terminal->eat_newline_glitch )
	{
	    if ( wrapped && (ch == '\n') )
	    {
		if (TCUR_LINE(terminal) == TLINES(terminal)-1)
		    raw_print(terminal,terminal->scroll_forward);
		else
		    putc('\n',terminal->fpout);
	    }
	    if (ICUR_COL(&terminal->image) == ICOLS(&terminal->image)-1) 
		wrapped = 1;
	    else
		wrapped = 0;
	}

	/* Update image - must be done AFTER screen update */
	iputc(&terminal->image,ch);
    }
#if 0
    if ( acs_missing )
	tset_modes(terminal,modes);
#endif
}

