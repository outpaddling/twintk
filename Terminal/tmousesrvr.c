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

#if 0
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "twintk_term.h"

int     topen_mouse_server(terminal,server,device,type)
term_t  *terminal;
char    *server, *device;
int     type;

{
    char    mouse_type[10];
    int     mouse;

    /* Is mouse configured? */
    if ( type == NO_MOUSE )
	return NO_MOUSE_DEVICE;
    
    /* Start mouse server with output to pipe */
    if ( tmpnam(terminal->mouse_pipe) == NULL )
	return NO_TMPFILE;
    mkfifo(terminal->mouse_pipe,0600);
    
    /* Create mouse server process */
    snprintf(mouse_type,9,"%d",type);
    terminal->mouse_pid = spawnlp(P_NOWAIT,P_NOECHO,NULL,
	    terminal->mouse_pipe,NULL,server,device,mouse_type,NULL);

    /* Open pipe file to receive input from server */
    if ( (mouse = open(terminal->mouse_pipe,O_RDONLY,0600)) != -1 )
    {
	/*bytes = read(mouse,buff,999);*/
	/*tprintf(terminal,"%d bytes read.\n",bytes);
	tgetc(terminal);*/
	MOUSE_TYPE(terminal->mouse) = type;
	MOUSE_FD(terminal->mouse) = mouse;
    }
    return mouse;
}


void    tclose_mouse_server(terminal)
term_t  *terminal;

{
    if ( MOUSE_FD(terminal->mouse) != -1 )
    {
	close(MOUSE_FD(terminal->mouse));
	INIT_MOUSE(terminal->mouse);
	/*MOUSE_FD(terminal->mouse) = -1;*/
	
	/* Remove pipe file */
	if ( *terminal->mouse_pipe != '\0' )
	{
	    unlink(terminal->mouse_pipe);
	    *terminal->mouse_pipe = '\0';
	}
	
	/* Kill mouse server */
	if ( terminal->mouse_pid != -1 )
	{
	    kill(terminal->mouse_pid,SIGKILL);
	    terminal->mouse_pid = -1;
	}
    }
}
#endif

