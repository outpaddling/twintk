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


/********************************
 * Mouse stuff
 ********************************/
 
/* Mouse types */
#define NO_MOUSE            0
#define CUSTOM              1
#define MICROSOFT_SERIAL    2
#define TODAYS_MOUSE        3
#define MOUSE_SYSTEMS       4
#define MOUSE_MAN           5
#define MICROSOFT_BUS       6
#define ATI_BUS             7
#define SCO_BUS             8   /* Generic bus mouse driver */
#define PS2                 9

/* Console mouse error codes */
#define OPEN_FAILED         -1
#define MOUSE_LOCKED        -2
#define NO_TTY              -3
#define DEVICE_NOT_SERIAL   -4
#define DEVICE_NOT_BUSMOUSE -5
#define NO_TMPFILE          -6
#define NO_MOUSE_DEVICE     -7

#define INIT_MOUSE(m)   (m).fd = -1
#define MOUSE_FD(m)     (m).fd
#define MOUSE_TYPE(m)   (m).type

#define select_t    fd_set

typedef struct
{
    int     dx;
    int     dy;
    int     button1_press;
    int     button2_press;
    int     button3_press;
    int     button1_release;
    int     button2_release;
    int     button3_release;
}   packet_t;

typedef struct
{
    int     fd;
    int     type;
}   mouse_t;


/* atibus.c */
int get_ati_packet (
	int mouse_fd, 
	packet_t *packet);

/* microsoft.c */
int get_microsoft_packet (
	int mouse_fd, 
	packet_t *packet);

/* mouselib.c */
int open_mouse (
	mouse_t *mouse, 
	char *device, 
	int type);
void close_mouse (
	mouse_t *mouse);
int get_raw_mouse_packet (
	mouse_t *mouse, 
	packet_t *packet);

/* mousesys.c */
int get_mouse_systems_packet (
	int mouse_fd, 
	packet_t *packet);

/* ps2.c */
int get_ps2_packet (
	int mouse_fd, 
	packet_t *packet);

/* scobus.c */
int get_scobus_packet (
	int mouse_fd, 
	packet_t *packet);

/* tmouse.c */
void orphan (void);

