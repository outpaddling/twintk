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

void 
tredraw (term_t *terminal, int topline, int leftcol, int bottomline, int rightcol)

{
    int    l,save_top,save_bottom,save_line,save_col;
    int         save_mode;
    
    save_mode = TCUR_MODES(terminal);
    save_top = TSCROLL_TOP(terminal);
    save_bottom = TSCROLL_BOTTOM(terminal);
    TFIND_CURSOR(terminal,save_line,save_col);
    tchange_scroll_region(terminal,0,TLINES(terminal)-1);
    
    /* Redraw all but the bottom line of the terminal */
    for (l = topline; (l <= bottomline) && (l < TLINES(terminal)-1); ++l)
	tredraw_line_leave_cursor(terminal,l,leftcol,rightcol);

    /* Redraw bottom line, except last char to avoid scroll */
    if ( l == TLINES(terminal)-1 )
	tredraw_line_leave_cursor(terminal,l,leftcol,rightcol-1);

    tchange_scroll_region(terminal,save_top,save_bottom);
    tmove_to(terminal,save_line,save_col);
    tset_modes(terminal,save_mode);
}

