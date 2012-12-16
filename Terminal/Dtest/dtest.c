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
#include <ctype.h>
#include "direct.h"
#include "bacon.h"

#define     MAXLEN  80

int     main()

{
    term_t  *term;
    int     l, c;
    char    vis[MAXLEN+1],vis2[MAXLEN+1];
    
    term = init_term(stdin,stdout,stderr,NULL,0);
    if ( term == NULL )
    {
	fputs("Cannot open terminal.\n",stderr);
	return 1;
    }
    
    tunset_tty(term,C_IFLAG,ECHO);
    viscpy(vis,"\003xyz4",5);
    tputs(term,vis);
    tputs(term,"Testing tclear_screen()...");
    tgetc(term);
    tclear_screen(term);
    
    tputs(term,"Testing tmove_to()...");
    tgetc(term);
    for (l=0; l<20; ++l)
    {
	c = l + 50;
	tmove_to(term,20,20);
	tprintf(term,"%s %s",viscpy(vis,term->cursor_address,MAXLEN),
		viscpy(vis2,tparm(term->cursor_address,l,c),MAXLEN));
	tputs(term,"    ");
	tgetc(term);
	tmove_to(term,l,c);
	tprintf(term,"* <- %d,%d ",l,c);
    }
    
    tputs(term,"Testing tredraw()...");
    tgetc(term);
    TREDRAW_SCREEN(term);
    tgetc(term);
    
    tputs(term,"Testing standout modes...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->enter_standout_mode,MAXLEN));
    tset_modes(term,STANDOUT_MODE);
    tputs(term,"This is standout mode...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->enter_reverse_mode,MAXLEN));
    tset_modes(term,REVERSE_MODE);
    tputs(term,"This is reverse mode...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->enter_underline_mode,MAXLEN));
    tset_modes(term,UNDERLINE_MODE);
    tputs(term,"This is underline mode...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->enter_blink_mode,MAXLEN));
    tset_modes(term,BLINK_MODE);
    tputs(term,"This is blink mode...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->enter_bold_mode,MAXLEN));
    tset_modes(term,BOLD_MODE);
    tputs(term,"This is bold mode...\n");
    tset_modes(term,HIGHLIGHT_MODE);
    tputs(term,"This is highlight mode...\n");
    tprintf(term,"\"%s\"",viscpy(vis,term->exit_attribute_mode,MAXLEN));
    tset_modes(term,NORMAL_MODE);
    tputs(term,"This is normal mode...\n");
    tgetc(term);
    
    tclear_screen(term);
    tputs(term,"Testing insert mode...");
    tmove_to(term,10,0);
    tputs(term,"This should move over");
    tgetc(term);
    tmove_to(term,10,0);
    tinsert_ch(term,'I');
    tgetc(term);
    return 0;
}

