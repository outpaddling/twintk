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
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "direct.h"

/***************************************************************************
 *
 * Title: mouse driver
 * Author:
 * Created:
 * Description:
 *      Mouse daemon for programs running on the Unix console.
 *      Generally invoked as a child of the application that wants
 *      mouse events.
 *
 * Command line arguments:
 *      device: The special file referring to the mouse
 *      pipe:   Optional.  Named pipe file to send output to for
 *              communicating with application.  Uses stdout otherwise
 *
 * Modification history:
 * 
 **************************************************************************/

int     get_raw_mouse_packet(fd,packet)
int     fd;
packet_t    *packet;

{
    return 0;
}


int     main(argc,argv)
int     argc;
char    *argv[];

{
    struct termios  attr;
    int             fd, mouse_type;
    FILE            *outfile;
    char            *device;
    packet_t        packet;
    
    /* Check for proper command-line arguments */
    switch(argc)
    {
	case    3:
	    device = argv[1];
	    mouse_type = atoi(argv[2]);
	    outfile = stdout;
	    break;
	default:
	    fprintf(stderr,"%s: <mouse-device> <mouse-type>\n",argv[0]);
	    return 1;
    }
    
    /* Open mouse */
    if ( (fd = open(device,O_RDONLY)) != -1 )
    {
	/* Set parameters if necessary (for serial mice) */
	tcgetattr(fd,&attr);
	tcsetattr(fd,TCSANOW,&attr);
	
	while ( 1 )
	{
	    /* User needs to write get_raw_mouse_packet routine */
	    if ( get_raw_mouse_packet(fd,&packet) != 0 )
	    {
		/* Print in readable form on terminal for testing purposes */
		if ( isatty(fileno(outfile)) )
		    fprintf(outfile,"%-4d %-4d %d %d %d %d %d %d\n",
			packet.dx,packet.dy,
			packet.button1_press,packet.button2_press,packet.button3_press,
			packet.button1_release,packet.button2_release,packet.button3_release);
		else    /* Print to pipe in binary for speed and simplicity */
		    fprintf(outfile,"%c%c%c%c%c%c%c%c",
			packet.dx,packet.dy,
			packet.button1_press,packet.button2_press,packet.button3_press,
			packet.button1_release,packet.button2_release,packet.button3_release);
		fflush(outfile);
	    }
	}
	return 0;
    }
    else
    {
	fprintf(stderr,"Cannot open %s.\n",device);
	return 1;
    }
}

