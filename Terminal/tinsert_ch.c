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

void    tinsert_ch(terminal,ch)
term_t  *terminal;
int     ch;

{
    /* Update screen image */
    iinsert_ch(&terminal->image,ch);

    /* Update physical screen */
    if ( terminal->insert_character != NULL )
    {   
	raw_print(terminal,terminal->insert_character);
	putc(ch,terminal->fpout);
    }
    else if ( terminal->enter_insert_mode != NULL )
    {
	if ( !TINSERT_MODE_ON(terminal) )
	    raw_print(terminal,terminal->enter_insert_mode);
	putc(ch,terminal->fpout);
	if ( !TINSERT_MODE_ON(terminal) )
	    raw_print(terminal,terminal->exit_insert_mode);
    }
    else
    {
	int     line,lastcol,c;
	
	line = TCUR_LINE(terminal);
	lastcol = TCOLS(terminal)-2;
	c = ICHAR(&terminal->image,line,lastcol+1);
	while ( (ICHAR(&terminal->image,line,lastcol) == c) && (lastcol>0) )
	    --lastcol;
	tredraw_line(terminal,TCUR_LINE(terminal),TCUR_COL(terminal)-1,lastcol);
    }
}

