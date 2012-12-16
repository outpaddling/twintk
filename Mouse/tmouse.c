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
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "bacon.h"
#include "twintk_mouse.h"

/***************************************************************************
 *
 * Title: General purpose mouse daemon
 * Author: Jason Bacon
 * Created: 03-03-98
 * Description:
 *      Mouse daemon for programs running on the Unix console.
 *      Generally invoked as a child of the application that wants
 *      mouse events.
 *
 * Command line arguments:
 *      device: The special file referring to the mouse
 *      mouse type: constant defined in direct.h
 *
 * Modification history:
 * 
 **************************************************************************/

mouse_t Mouse;  /* Must be global for orphan() signal handler */

int     main(argc,argv)
int     argc;
char    *argv[];

{
    char        *device;
    packet_t    packet;
    int         mouse, mouse_type, tty = isatty(fileno(stdout));

    switch(argc)
    {
	case    3:
	    device = argv[1];
	    mouse_type = atoi(argv[2]);
	    break;
	default:
	    usage("%s: <mouse-device> <mouse-type>\n",argv[0]);
	    return 1;
    }

#ifdef SCO_SV
    sigset(SIGPIPE,(sig_t)orphan);
#else
    signal(SIGPIPE,(sig_t)orphan);
#endif
    
    if ( !tty )
    {
#ifdef SCO_SV
	sigset(SIGINT,SIG_IGN);
	sigset(SIGQUIT,SIG_IGN);
	sigset(SIGTERM,SIG_IGN);
#else
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	signal(SIGTERM,SIG_IGN);
#endif
    }

    if ( (mouse = open_mouse(&Mouse,device,mouse_type)) < 0 )
    {
	/* Should this be stderr? */
	if ( isatty(fileno(stdout)) )
	    fprintf(stderr,"Cannot open mouse device %s.\n",device);
	return mouse;
    }
    
    while ( 1 )
    {
	if ( get_raw_mouse_packet(&Mouse,&packet) != 0 )
	{
	    /* Print in readable form on Terminal for testing purposes */
	    if ( tty )
		printf("%-4d %-4d %d %d %d %d %d %d\n",
		    packet.dx,packet.dy,
		    packet.button1_press,packet.button2_press,packet.button3_press,
		    packet.button1_release,packet.button2_release,packet.button3_release);
	    else    /* Print to pipe in binary for speed and simplicity */
		printf("%c%c%c%c%c%c%c%c",
		    packet.dx,packet.dy,
		    packet.button1_press,packet.button2_press,packet.button3_press,
		    packet.button1_release,packet.button2_release,packet.button3_release);
	    fflush(stdout);
	}
    }
}


void    orphan()

{
    close_mouse(&Mouse);
    exit(0);
}

