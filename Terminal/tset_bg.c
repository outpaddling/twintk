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

#include "twintk_term.h"

int 
tset_background (term_t *terminal, int color)

{
    if ( TMAX_COLORS(terminal) != -1 )
    {
	/* FIXME: twintk gets confused by this condition if the terminal
	   is not white on black.  Perhaps clearing the screen at init time
	   to set every character to a known state would solve the problem.
	   It would be nice to eliminate unnecessary color setting sequences
	   from the stream.
	if ((color != TCUR_BACKGROUND(terminal))
	    || (terminal->no_color_video != -1) )
	{*/
	    ISET_BACKGROUND(&terminal->image,color);
	    if ( terminal->set_a_background != NULL )
		raw_print(terminal,tparm(terminal->set_a_background,color));
	    else
		raw_print(terminal,tparm(terminal->set_background,color));
	/*}*/
	return 0;
    }
    else
    {
	return(-1);
    }
}
