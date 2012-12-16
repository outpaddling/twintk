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

#include <windows.h>

    int     main()

    {
        term_t  *term;
        win_t   *win;
    
        /* Initialize terminal */
        if ( (term = init_term(stdin,stdout,stderr,NULL)) == NULL)
            return 1;
    
        /* Create a new window */
        if ( (win = new_win(term,10,60,8,10)) == NULL )
            return 1;
    
        /* Clear window */
        wclear_win(win);
        
        /* Draw border around window */
        WSET_MODE(win,BOLD_MODE);
        wborder(win);
        
        /* Print a blinking message in the window */
        WSET_MODE(win,BLINK_MODE);
        wprint_center(win,4,"Hello, world!");
        
        /* Wait for user to press a key */
        WSET_MODE(win,NORMAL_MODE);
        wprint_center(win,5,"Press return to continue...");
        wgetc(win);
        
        /* Delete window and terminal and quit */
        del_win(&win);
        del_term(&term);
        return 0;
    }
