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

#include <unistd.h>
#include "windows.h"

int     main()

{
    term_t  *term;
    win_t   *win;
    char    *mesg = "  *** Hello, world! ***  *** Eat at Joes... ***   ",
            *p;

    /* Initialize terminal */
    if ( (term = init_term(stdin,stdout,stderr,NULL)) == NULL )
        return 1;
    
    /* Open window for marquee */
    if ( (win = new_win(term,2,40,11,20)) == NULL )
        return 1;
    
    /* Display quitting instructions and border */
    wborder(win);
    wprint_center(win,1,"Press <ctrl>+c to quit");
    
    /* Continually update marquee until user kills program */
    while (1)
    {
        /* Display marquee message once */
        for (p=mesg; *p!='\0'; ++p)
        {
            wmove_to(win,0,39);
            wputc(win,*p);
            wmove_to(win,0,0);
            wdelete_ch(win);
            WFLUSH_OUT(win);
            usleep(200000);
        }
    }
    
    /* Free memory before quitting */
    del_win(&win);
    del_term(&term);
    return 0;
}

