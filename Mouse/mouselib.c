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
#include <fcntl.h>
#include <signal.h>
#include <termios.h>
#include "bacon.h"
#include "twintk_mouse.h"

int     open_mouse(mouse,device,type)
mouse_t *mouse;
char    *device;
int     type;

{
    int     mouse_fd;
    struct termios  term_attr;

    if (type == NO_MOUSE)
	return NO_MOUSE_DEVICE;
    
    if ( (mouse_fd = open(device,O_RDONLY)) == -1)
	return -1;
    
    /*
    flags = fcntl(mouse_fd,F_GETFL,0);
    printf("flags = %02x\n",flags);
    fflush(stdout);
    */
    
    mouse->type = type;
    mouse->fd = mouse_fd;
    
    /* For SCO_BUS and Serial mice */
    tcgetattr(mouse_fd,&term_attr);
    term_attr.c_cc[VMIN] = 1;
    term_attr.c_cc[VTIME] = 0;

    /* Set serial driver parameters */
    switch(type)
    {
	/* Microsoft compatible */
	case    MICROSOFT_SERIAL:
	    if ( !isatty(mouse_fd) )
	    {
		close(mouse_fd);
		mouse->fd = -1;
		return DEVICE_NOT_SERIAL;
	    }
	    term_attr.c_iflag = 0;
	    term_attr.c_cflag = CS7|CREAD|CLOCAL;
	    term_attr.c_lflag = 0;
	    cfsetispeed(&term_attr,B1200);
	    cfsetospeed(&term_attr,B1200);  /* Needed by SCO for NAR. */
	    if ( tcsetattr(mouse_fd,TCSANOW,&term_attr) == -1 )
	    {
		fprintf(stderr,"Cannot set parameters.\n");
		return NO_TTY;
	    }
	    break;
	case    SCO_BUS:
	    if ( tcsetattr(mouse_fd,TCSANOW,&term_attr) == -1 )
	    {
		fprintf(stderr,"Cannot set parameters.\n");
		return NO_TTY;
	    }
	    break;
	case    MOUSE_SYSTEMS:
	    if ( !isatty(mouse_fd) )
	    {
		close(mouse_fd);
		mouse->fd = -1;
		return DEVICE_NOT_SERIAL;
	    }
	    term_attr.c_iflag = 0;
	    term_attr.c_cflag = CS8|CREAD|CLOCAL|CSTOPB;
	    term_attr.c_lflag = 0;
	    cfsetispeed(&term_attr,B1200);
	    cfsetospeed(&term_attr,B1200);  /* Needed by SCO for NAR. */
	    if ( tcsetattr(mouse_fd,TCSANOW,&term_attr) == -1 )
	    {
		fprintf(stderr,"Cannot set parameters.\n");
		return NO_TTY;
	    }
	    break;
	case    ATI_BUS:
	case    MOUSE_MAN:
	case    MICROSOFT_BUS:
	case    PS2:
	    break;
	default:
	    break;
    }
    return mouse_fd;
}


void    close_mouse(mouse)
mouse_t *mouse;

{
    if ( mouse->fd != -1 )
    {
	close(mouse->fd);
	mouse->fd = -1;
    }
}


int     get_raw_mouse_packet(mouse,packet)
mouse_t  *mouse;
packet_t    *packet;

{
    int     status;
    
    switch(mouse->type)
    {
	case    MICROSOFT_SERIAL:
	    status = get_microsoft_packet(mouse->fd,packet);
	    break;
	case    MOUSE_SYSTEMS:
	    status = get_mouse_systems_packet(mouse->fd,packet);
	    break;
	case    ATI_BUS:
	    status = get_ati_packet(mouse->fd,packet);
	    break;
	case    SCO_BUS:
	    status = get_scobus_packet(mouse->fd,packet);
	    break;
	case    PS2:
	    status = get_ps2_packet(mouse->fd,packet);
	    break;
	default:
	    status = -1;
	    break;
    }
    return status;
}

