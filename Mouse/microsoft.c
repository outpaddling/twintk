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

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "twintk_mouse.h"

/***************************************************************************
 *
 * Title: General purpose Microsoft serial mouse driver
 * Author: Jason Bacon
 * Created: 03-03-98
 * Description:
 *
 * Modification history:
 * 
 **************************************************************************/

int     get_microsoft_packet(mouse_fd,packet)
int     mouse_fd;
packet_t    *packet;

{
    int     c1, c2, c3;
    static int  button1_down = 0, button3_down = 0;
    char    buff[3];
    
    /* Get 3-byte mouse packet */
    /*
     * Validate 1st byte to get in synch with 3-byte packets.
     * Today's Mouse sends 4th byte (space or NULL) for middle
     * button extension
    */
    do
    {
	read(mouse_fd,buff,1);
	c1 = *buff;
	
	/*
	 * Today's mouse middle button extension uses a 4th byte.
	 * Keep motion info from previous 3 bytes.
	 */
	if (c1 == 32)
	{
	    packet->button2_press = 1;
	    packet->button2_release = 0;
	    return 0;
	}
	if (c1 == 0)
	{
	    packet->button2_press = 0;
	    packet->button2_release = 1;
	    return 0;
	}
    }   while ( (c1 & 0x40) == 0 );
    read(mouse_fd,buff,1);
    c2 = *buff;
    read(mouse_fd,buff,1);
    c3 = *buff;
    
    /* Check input packet for basic events */
    /* Cast to char so 8-bit values will be sign-extended back to int */
    packet->dx = (char)((c1 & 0x03) << 6) | (c2 & 0x3f);
    packet->dy = (char)((c1 & 0x0c) << 4) | (c3 & 0x3f);
    packet->button1_press = (c1 & 0x20) >> 5;
    packet->button3_press = (c1 & 0x10) >> 4;

    /* Cheaper to intialize now than determine when it's appropriate */ 
    packet->button2_press = packet->button2_release = 
	packet->button1_release = packet->button3_release = 0;
    
    /* Mouse won't tell us which button was released, so we have
       to remember which one is down when the release event occurs */
    if ( packet->button1_press )
	button1_down = 1;
    if ( packet->button3_press )
	button3_down = 1;
    
    /* This could be a release, or a middle-button press or release
       Could also be mixed with dx or dy information - button events
       are not necessarily received as separate packets, unless there
       was no motion during the button event */
    if ( !(packet->button1_press || packet->button3_press) )
    {
	packet->button1_release = button1_down;
	if ( button1_down )
	    button1_down = 0;
    
	packet->button3_release = button3_down;
	if ( button3_down )
	    button3_down = 0;
    }
    return 1;
}

