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

FILE    *open_mouse_server(device)
char    *device;

{
    pid_t   pid;
    FILE    *mouse = NULL;
    char    Pipe[L_tmpnam+1], pidstr[20];
    
    tmpnam(Pipe);
    mkfifo(Pipe,0600);
    signal(SIGUSR1,(sig_t)mouse_handler);
    snprintf(pidstr,19,"%d",getpid());
    
    switch ( pid = fork() )
    {
	case    0:  /* Child */
	    printf("mouse %s %s %s\n",device,Pipe,pidstr);
	    execlp("mouse","mouse",device,Pipe,pidstr,NULL);
	    fprintf(stderr,"Could not create mouse daemon: exec() failed.\n");
	    return NULL;
	case    -1: /* Error */
	    fprintf(stderr,"Could not create mouse daemon: fork() failed.\n");
	    return NULL;
	default:    /* Parent */
	    mouse = fopen(Pipe,"r");
	    break;
    }
    return mouse;
}


void    mouse_handler()

{
    __Mouse_event = 1;
#if defined(linux)
    signal(SIGUSR1,(sig_t)mouse_handler);
#endif
}


void    close_mouse_server(mouse)
FILE    *mouse;

{
    fclose(mouse);
    __Mouse_event = 0;
    unlink(__Pipe);
}

