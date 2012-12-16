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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include "twintk_term.h"

void    raw_print(terminal,str)
term_t  *terminal;
char    *str;

{
    struct termios  new,old;
    char            *p;
    int             delay = 0, pad_cnt, c, has_ctrl_chars = 0, pad;

    if ( str == NULL )
	return;

    /* Shut off output processing if control chars present in sequence
     * Use TCSADRAIN to prevent slowdown when input is pending
     * Incredibly slow on SCO OS5 - programs should turn off OPOST
     * in advance to avoid delays */
    if ( terminal->tty_mode.c_oflag & OPOST )
    {
	/* Check for control chars that would be modified on output */
	for (p=str; *p != '\0'; ++p)
	{
	    if ( (*p == TW_TAB) || (*p == TW_NL) || 
		(*p == TW_EOT) || (*p == TW_DLE) )
		has_ctrl_chars = 1;
	}
	if ( has_ctrl_chars )
	{
	    old = new = terminal->tty_mode;
	    new.c_oflag &= ~OPOST;
	    fflush(terminal->fpout);
	    tcsetattr(fileno(terminal->fpout),TCSANOW,&new);
	}
    }
    
    /* Check string for delays */
    while ( *str != '\0' )
    {
	switch(*str)
	{
	    case    '$':
		if ( str[1] == '<' )
		{
		    str += 2;
		    delay = strtol(str,&str,10);
		    /* Skip everything through closing '>' */
		    while ((*str != '\0') && (str[-1] != '>'))
			++str;
		}
		break;
	    case    '\\':   /* tparm() adds \ before special characters e.g. $, \ */
			    /* This will allow cup motions for ibm3101, etc. */
		++str;  /* Skip \ */
		putc(*str,terminal->fpout);
		++str;
		break;
	    default:
		putc(*str,terminal->fpout);
		++str;
	}
    }
    
    /* Send pad characters to implement delay if needed */
    if ( delay && terminal->use_delays )
    {
	/* Send (chars/ms * delay) NULs to delay */
	switch(cfgetospeed(&terminal->tty_mode))
	{
	    case    B1200:
		pad_cnt = CEIL(.120 * delay);
		break;
	    case    B2400:
		pad_cnt = CEIL(.240 * delay);
		break;
	    case    B4800:
		pad_cnt = CEIL(.480 * delay);
		break;
	    case    B9600:
		pad_cnt = CEIL(.960 * delay);
		break;
	    case    B19200:
		pad_cnt = CEIL(1.92 * delay);
		break;
	    case    B38400:
		pad_cnt = CEIL(3.84 * delay);
		break;
	    default:
		pad_cnt = CEIL(delay);
	}
	/* Send padding to implement delay */
	pad = terminal->pad_char == NULL ? '\0' : *terminal->pad_char;
	for (c=0; c<pad_cnt; ++c)
	    putc(pad,terminal->fpout);
    }
    
    /* Restore original terminal mode if OPOST was shut off */
    if ( has_ctrl_chars ) /* Implies OPOST - wouldn't be set if OPOST were false */
    {
	fflush(terminal->fpout);
	tcsetattr(fileno(terminal->fpout),TCSANOW,&old);
    }
}

