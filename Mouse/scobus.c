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

#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include "twintk_mouse.h"

/***************************************************************************
 *
 * Title: General purpose ATI bus mouse driver
 * Author: Jason Bacon
 * Created: 03-03-98
 * Description:
 *
 * Modification history:
 * 
 **************************************************************************/

int     get_scobus_packet(mouse_fd,packet)
int     mouse_fd;
packet_t    *packet;

{
    int     c1;
    static int  button1_down = 0, button2_down = 0, button3_down = 0;
    unsigned char    buff[3];
    
    read(mouse_fd,buff,1);
    c1 = *buff;
    packet->button1_press = packet->button1_release = 0;
    packet->button2_press = packet->button2_release = 0;
    packet->button3_press = packet->button3_release = 0;
    switch(c1)
    {
	case    0x04:
	    packet->button1_press = button1_down = 1;
	    break;
	case    0x05:
	    packet->button2_press = button2_down = 1;
	    break;
	case    0x01:
	    packet->button3_press = button3_down = 1;
	    break;
	case    0x00:
	    if ( button1_down )
		packet->button1_release = 1;
	    if ( button2_down )
		packet->button2_release = 1;
	    if ( button3_down )
		packet->button3_release = 1;
	    button1_down = button2_down = button3_down = 0;
	    break;
	default:
	    memset(packet,0,sizeof(*packet));
	    return 0;
    }
    read(mouse_fd,buff,2);
    /*printf("%02x %02x %02x\n",c1,buff[0],buff[1]);*/
    packet->dx = (char)buff[0];
    packet->dy = (char)buff[1];
    return 1;
}

#ifdef DEBUG
int     main()

{
    packet_t    packet;
    int         fd;
    
    fd = open("/dev/mouse/bus0",O_RDONLY);
    while (1)
    {
	get_scobus_packet(fd,&packet);
	printf("%3d %3d %d %d %d %d %d %d\n",packet.dx,packet.dy,
		packet.button1_press,packet.button2_press,packet.button3_press,
		packet.button1_release,packet.button2_release,packet.button3_release);
    }
}
#endif
