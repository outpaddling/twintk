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

#include <stdlib.h>
#include <bacon.h>
#include "twintk_term.h"
    
void    tadd_key_mouse(term_t *terminal)

{
    if ( ((terminal->key_mouse == NULL) || (*terminal->key_mouse == '\0')) &&
	 (getenv("WINDOWID") != NULL) &&
	 /* xterm, xterm-color, etc. */
	 ((memicmp(terminal->term_type,"xterm",5) == 0) ||
	  (memicmp(terminal->term_type,"rxvt",4) == 0) ||
	  /* SCO ansi */
	  (memicmp(terminal->term_type,"ansi",4) == 0) ||
	  /* scoansi and scocons */
	  (memicmp(terminal->term_type,"sco",3) == 0 ) ||
	  /* vt100, vt102, vt200, etc. */
	  (memicmp(terminal->term_type,"vt",2) == 0)) )
    {
	terminal->get_mouse = "\033[?%p1%dh";
	terminal->key_mouse = "\033[M";
	taddseq(terminal,terminal->key_mouse,KEY_MOUSE);
    }
}

