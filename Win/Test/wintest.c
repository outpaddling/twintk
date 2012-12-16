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
#include <stdlib.h>
#include "windows.h"

#define WINDOWS 4

char    *getenv();

int     main()

{
    char    *termtype,temp[11];
    term_t  *terminal;
    win_t   *wins[WINDOWS],*win;
    int     w,c,l,f,b,last;

    termtype = getenv("TERM");
    if ( termtype == NULL )
    {
        fprintf(stderr,"Terminal not identified in environment.\n");
        exit(1);
    }

    setvbuf(stdout,NULL,_IONBF,BUFSIZ);
    terminal = init_term(stdin,stdout,stderr,termtype);
    tunset_tty(terminal,C_LFLAG,ICANON|ECHO);
    
    add_seq(terminal->keymap,terminal->key_left,KEY_LEFT,MAX_KEYS);
    add_seq(terminal->keymap,terminal->key_right,KEY_RIGHT,MAX_KEYS);

    /* Full screen window */
    wins[2] = new_win(terminal,TLINES(terminal),TCOLS(terminal),0,0,DEFAULT);
    WSET_AUTO_SCROLL(wins[2]);
    
    /* Full width window */
    wins[1] = new_win(terminal,TLINES(terminal)-4,TCOLS(terminal),2,0,DEFAULT);
    WSET_AUTO_SCROLL(wins[1]);
    
    /* Small window */
    wins[0] = new_win(terminal,10,40,10,20,DEFAULT);
    WSET_AUTO_SCROLL(wins[0]);
    
    /* Subwindow */
    wins[3] = sub_win(wins[1],10,TCOLS(terminal),3,0,DEFAULT);

    for (w=0; w<WINDOWS; ++w)
    {
        win = wins[w];
        tclear_screen(terminal);
        printf("Window: %d x %d\n",WLINES(win),WCOLS(win));
        printf("View: %d x %d\n",WVIEW_LINES(win),WVIEW_COLS(win));
        printf("Full-screen: %d  Full-width: %d\n",
                WFULL_SCREEN_VIEW(win),WFULL_WIDTH_VIEW(win));
        printf("Window location: %d %d\n",
                win->screen_line,win->screen_col);        
        printf("Win: %d,%d  Term: %d,%d",WCUR_LINE(win),WCUR_COL(win),
                TCUR_LINE(win->terminal),TCUR_COL(win->terminal));
        printf("wintest: Auto_scroll: %d\n",WAUTO_SCROLL(win));
        tgetc(terminal);
        wouter_border(win,1);
        wmove_to(win,0,0);
        wputs(win,"Press return...");
        wgetc(win);

        /* Test wgetstring */
        wclear_win(win);
        wmove_to(win,2,2);
        wgetstring(win,temp,10,VERBATIM,&last);
        
        /* Test wclear and wputc */
        wclear_win(win);
        wputs(win,"Testing wputc()...\n");
        wgetc(win);
        wclear_win(win);
        for (l=0; l < WLINES(win)+5; ++l)
        {
            for (c=0; c < l; ++c)
                wputc(win,'A'+c%26);
            wputc(win,'\n');
        }
        wmove_to(win,0,0);
        wputs(win,"Press return...");
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);

        /* wscroll_forward()*/
        wscroll_forward(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* wscroll_reverse() */
        wscroll_reverse(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* wclear_eol() */
        wclear_win(win);
        wmove_to(win,5,0);
        for (c=0; c < WCOLS(win); ++c)
            wputc(win,'1');
        wmove_to(win,5,5);
        wgetc(win);
        wclear_eol(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* wclear_eow() */
        wclear_win(win);
        for (c=0; c<WLINES(win)*WCOLS(win)+1; ++c)
            wputc(win,'a'+c/WCOLS(win));
        wmove_to(win,5,5);
        wgetc(win);
        wclear_eow(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* winsert_ch() */
        wclear_win(win);
        wmove_to(win,5,0);
        for (c=0; c < WCOLS(win); ++c)
            wputc(win,'1');
        wmove_to(win,5,5);
        wgetc(win);
        for (c=0; c<10; ++c)
            winsert_ch(win,'a'+c);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* wdelete_ch() */
        wmove_to(win,5,5);
        for (c=0; c<10; ++c)
            wdelete_ch(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* winsert_line() */
        wclear_win(win);
        for (c=0; c<WLINES(win)*WCOLS(win)+1; ++c)
            wputc(win,'0'+c/WCOLS(win));
        wmove_to(win,5,5);
        wgetc(win);
        winsert_line(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* wdelete_line() */
        wmove_to(win,5,5);
        wdelete_line(win);
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* WSET_MODE() */
        wclear_win(win);
        wprintf(win,"Modes: %x %x\n",WCUR_MODE(win),TCUR_MODE(win->terminal));
        wgetc(win);
        WSET_MODE(win,BLINK_MODE);
        wputs(win,"This is blink mode.\n");
        WSET_MODE(win,REVERSE_MODE);
        /*
        wprintf(win,"Modes: %x %x\n",WCUR_MODE(win),TCUR_MODE(win->terminal));
        wgetc(win);
        wputs(win,"This is reverse mode.\n");
        wprintf(win,"Modes: %x %x\n",WCUR_MODE(win),TCUR_MODE(win->terminal));
        wgetc(win);
        */
        WSET_MODE(win,BOLD_MODE);
        wputs(win,"This is bold mode.\n");
        WSET_MODE(win,DIM_MODE);
        wputs(win,"This is dim mode.\n");
        WSET_MODE(win,STANDOUT_MODE);
        wputs(win,"This is standout mode.\n");
        WSET_MODE(win,NORMAL_MODE);
        wputs(win,"This is normal mode.\n");
        WSET_MODE(win,REVERSE_MODE|BOLD_MODE);
        wputs(win,"Reverse + Bold\n");
        for (c=0; c<5; ++c)
        {
            WSET_MODE(win,REVERSE_MODE);
            wputc(win,'R');
            WSET_MODE(win,BOLD_MODE);
            wputc(win,'B');
            WSET_MODE(win,NORMAL_MODE);
            wputc(win,'N');
            WSET_MODE(win,DIM_MODE);
            wputc(win,'D');
            WSET_MODE(win,STANDOUT_MODE);
            wputc(win,'S');
        }
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        
        /* Color */
        wclear_win(win);
        for (b=0; b<8; ++b)
        {
            wset_background(win,b);
            for (f=0; f<8; ++f)
            {
                wset_foreground(win,f);
                wprintf(win,"%d:%d ",f,b);
            }
            wputc(win,'\n');
        }
        wgetc(win);
        WREDRAW_WIN(win);
        wgetc(win);
        wset_foreground(win,WHITE);
        wset_background(win,BLACK);
        WSYNC_COLORS(win);
    }
    wouter_border(wins[1],1);
    WRESTORE_WIN(wins[1]);
    wouter_border(wins[0],1);
    WRESTORE_WIN(wins[0]);
    wgetc(win);
    tset_tty(terminal,C_LFLAG,ECHO);
    return(0);
}
