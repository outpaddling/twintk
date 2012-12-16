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
#include <sys/time.h>
#include <unistd.h>
#include "twintk_term.h"

/************************************************************************
 * Author: Jason Bacon
 * Description:     Wait for key or mouse event and return in arg event
 * Arguments:
 * Return values:   The key pressed or other basic event info
 * Date:            March 5, 1998
 * Modifications:
 ************************************************************************/
 
int     tgetevent(terminal,event)
term_t  *terminal;
event_t *event;

{
    /*
     * All of the below must be static for auto-repeat, since
     * tgetevent() is returned from and reentered between repeats 
     */
    static event_t temp_event = EVENT_INIT;
    static int  button_event_type,
		old_line, old_col;
    static struct timeval   *timeout = NULL, delay;
    fd_set          readfds;
    int             fd_count, status, pointer_moved=1;

    /* If button releases are masked, reset repeat stuff */
    if ( ((terminal->event_mask & BUTTON_RELEASE) == 0) ||
	 (terminal->button_status == BUTTON_UNKNOWN) )
    {
	terminal->button_status = BUTTON_UP;
	timeout = NULL;
	temp_event.repeat = -1;
    }
	
    /* Read events until we find one in the terminal's event mask */
    do
    {
	/* Initialize fd set for select() */
	FD_ZERO(&readfds);  
	FD_SET(fileno(terminal->fpin),&readfds);
	if ( MOUSE_FD(terminal->mouse) > -1 )
	{
	    FD_SET(MOUSE_FD(terminal->mouse),&readfds);
	    fd_count = MAX(fileno(terminal->fpin),MOUSE_FD(terminal->mouse)) + 1;
	}
	else
	    fd_count = fileno(terminal->fpin) + 1;

	/* Make sure all output is displayed before blocking for input */
	if ( (MOUSE_FD(terminal->mouse) > -1) && pointer_moved )
	    tdisplay_mouse_pointer(terminal);
	
	/* Get new event, or time out for mouse button auto-repeat */
	TFLUSH_OUT(terminal);
	if ( (status = select(fd_count,(select_t *)&readfds,NULL,NULL,timeout) != 0) )
	{
	    /* Keyboard event? */
	    if ( FD_ISSET(fileno(terminal->fpin),&readfds) )
	    {
		tget_key_event(terminal,&temp_event);
		timeout = NULL;
	    }
	    /* Mouse event? */
	    else if ( (MOUSE_FD(terminal->mouse) != -1) &&
		       FD_ISSET(MOUSE_FD(terminal->mouse),&readfds) )
	    {
		/* ATI bus mouse sends data even when idle */
		tget_mouse_event(terminal,&temp_event);
		pointer_moved = (old_line != TMOUSE_LINE(terminal)) ||
				(old_col != TMOUSE_COL(terminal));
		if ( pointer_moved )
		{
		    terase_mouse_pointer(terminal,old_line,old_col);
		    old_line = TMOUSE_LINE(terminal);
		    old_col = TMOUSE_COL(terminal);
		}
		else
		    temp_event.type &= ~MOTION;
	    }

	    /* Update timeout value for auto-repeat if button is down */
	    /* If just pressed, set initial timeout for auto-repeat */
	    if (temp_event.type & BUTTON_PRESS)
	    {
		/*sprintw(2,50,"Button press: button_down = %d",button_down);*/
		if ( terminal->button_status == BUTTON_UP )
		{
		    button_event_type = temp_event.type;
		    /* Set button status only if release events are seen */
		    if ( (terminal->event_mask & BUTTON_RELEASE) != 0 )
			terminal->button_status = BUTTON_DOWN;
		    timeout = &delay;
		    delay.tv_sec = 0, delay.tv_usec = 500000;
		    temp_event.repeat = -1; /* Gets incremented later */
		}
		else
		    delay.tv_sec = 0, delay.tv_usec = 50000;
	    }
	    /* If released, cancel timeout and reset repeat count */
	    else if (temp_event.type & BUTTON_RELEASE)
	    {
		terminal->button_status = BUTTON_UP;
		timeout = NULL;
		temp_event.repeat = -1; /* Gets incremented later */
	    }
	}
	else    /* Timed out - set auto-repeat timeout */
	{
	    delay.tv_sec = 0, delay.tv_usec = 50000;
	    temp_event.type = button_event_type;
	}

	if ( terminal->button_status == BUTTON_DOWN )
	    ++temp_event.repeat;
    }   while ( ((temp_event.type & terminal->event_mask) == 0) ||
		((temp_event.type & MOTION) && !pointer_moved) );
    
    /* If event is NULL, caller only wants value */
    if ( event != NULL )
	*event = temp_event;
    return temp_event.value;
}


/************************************************************************
 * Author: Jason Bacon
 * Description: Get a keyboard event
 * Arguments:
 * Return values:   None.
 * Date:            March 12, 1998
 * Modifications:
 ************************************************************************/
 
int     tget_key_event(terminal,event)
term_t  *terminal;
event_t *event;

{
    static int  last_press = 0;
    
    event->value = tgetseq(terminal,event->seq,SEQ_LEN);
    switch(event->value)
    {
	/* Hard-coded vt200/xterm mouse for now */
	case    KEY_MOUSE:
	    switch((tgetc(terminal)-32)&0x03)
	    {
		case    0:
		    last_press = event->type = BUTTON1_PRESS;
		    break;
		case    1:
		    last_press = event->type = BUTTON2_PRESS;
		    break;
		case    2:
		    last_press = event->type = BUTTON3_PRESS;
		    break;
		case    3: /* Some button release */
		    /* BUTTONX_RELEASE is #defined as BUTTONX_PRESS << 3 */
		    event->type = last_press << 3;
		    break;
	    }
	    GET_TERMINFO_MOUSE_POS(terminal,event->mouse_line,event->mouse_col);
	    break;
	default:
	    event->repeat = 0;
	    event->type = KEY_PRESS;
	    break;
    }
    return 0;
}


/************************************************************************
 * Author: Jason Bacon
 * Description: Get a mouse event
 * Arguments:   
 * Return values:   None.
 * Date:            March 12, 1998
 * Modifications:
 ************************************************************************/
 
int     tget_mouse_event(terminal,event)
term_t  *terminal;
event_t *event;

{
    int new_line = terminal->scaled_mouse_line,
	new_col = terminal->scaled_mouse_col,
	status;
    packet_t    packet;
    
    /* Read mouse packet */
    status = tget_mouse_packet(terminal,&packet);
    if ( status != 0 )
    {
	int    max_line = (TLINES(terminal)-1)*terminal->mouse_y_scale,
		    max_col = (TCOLS(terminal)-1)*terminal->mouse_x_scale;
	
	/* Update mouse position */
	update_pos(packet.dy,&new_line,max_line);
	update_pos(packet.dx,&new_col,max_col);
	
	/* Move mouse pointer */
	if ( (new_line < max_line) || (new_col < max_col) )
	{
	    terminal->scaled_mouse_line = new_line;
	    terminal->scaled_mouse_col = new_col;
	}
	
	/* Set up temp_event with mouse info */
	event->value = KEY_MOUSE;
	event->mouse_line = TMOUSE_LINE(terminal);
	event->mouse_col = TMOUSE_COL(terminal);
	event->seq[0] = '\0';
	
	event->type = 0;
	/* Most mice send button status as long as the button is down */
	/* We don't report the same button press more than once */
	if ( packet.button1_press )
	    event->type = BUTTON1_PRESS;
	else if ( packet.button2_press )
	    event->type = BUTTON2_PRESS;
	else if ( packet.button3_press )
	    event->type = BUTTON3_PRESS;
	else if ( packet.button1_release )
	    event->type = BUTTON1_RELEASE;
	else if ( packet.button2_release )
	    event->type = BUTTON2_RELEASE;
	else if ( packet.button3_release )
	    event->type = BUTTON3_RELEASE;
	if ( packet.dx || packet.dy )
	    event->type |= MOTION;
    }
    return status;
}


/************************************************************************
 * Author: Jason Bacon
 * Description:
 * Arguments:
 * Return values:
 * Date:
 * Modifications:
 ************************************************************************/
 
void    tdisplay_mouse_pointer(terminal)
term_t  *terminal;

{
    int     mouse_line = TMOUSE_LINE(terminal),
	    mouse_col = TMOUSE_COL(terminal),
	    curline, curcol;
    int save_modes, save_fg, save_bg, use_attr, fg=WHITE, bg=BLACK, modes;
    
    if ( !terminal->mouse_pointer_visible )
	return;
    
    /* Maybe should use lower level move than tmove_to */
    TFIND_CURSOR(terminal,curline,curcol);
    if ( (TCUR_LINE(terminal) != mouse_line) || (TCUR_COL(terminal) != mouse_col) )
	tmove_to(terminal,mouse_line,mouse_col);
    modes = TMODES(terminal,mouse_line,mouse_col);
    if ( TCOLOR_TERM(terminal) )
    {
	fg = TFOREGROUND(terminal,mouse_line,mouse_col);
	bg = TBACKGROUND(terminal,mouse_line,mouse_col);
	use_attr = (fg == WHITE) && (bg == BLACK);
    }
    else
	use_attr = 1;
    
    if ( use_attr )
    {
	save_modes = TCUR_MODES(terminal);
	if ( TCOLOR_TERM(terminal) )
	{
	    if ( TCUR_FOREGROUND(terminal) != WHITE )
		tset_foreground(terminal,WHITE);
	    if ( TCUR_BACKGROUND(terminal) != BLACK )
		tset_background(terminal,BLACK);
	}
	if ( modes & REVERSE_MODE )
	    tset_modes(terminal,NORMAL_MODE|(modes&ACS_MODE));
	else
	    tset_modes(terminal,REVERSE_MODE|(modes&ACS_MODE));
	
	/* Display on screen without changing attributes in image */
	putc(TCHAR(terminal,mouse_line,mouse_col),terminal->fpout);
	
	tset_modes(terminal,save_modes);
    }
    else
    {
	save_fg = TCUR_FOREGROUND(terminal);
	save_bg = TCUR_BACKGROUND(terminal);
	tset_foreground(terminal,bg);
	tset_background(terminal,fg);
	tset_modes(terminal,modes&ACS_MODE);

	putc(TCHAR(terminal,mouse_line,mouse_col),terminal->fpout);
	
	tset_foreground(terminal,save_fg);
	tset_background(terminal,save_bg);
    }
    tmove_to(terminal,curline,curcol);
}


/************************************************************************
 * Author: Jason Bacon
 * Description:
 * Arguments:
 * Return values:
 * Date:
 * Modifications:
 ************************************************************************/
 
void    terase_mouse_pointer(terminal,mouse_line,mouse_col)
term_t  *terminal;
int mouse_line, mouse_col;

{
    int     curline, curcol;
    int save_modes, save_fg, save_bg, use_attr, fg=WHITE, bg=BLACK;
    
    if ( !terminal->mouse_pointer_visible )
	return;
    
    /* Maybe should use lower level move than tmove_to */
    if ( TCOLOR_TERM(terminal) )
    {
	fg = TFOREGROUND(terminal,mouse_line,mouse_col);
	bg = TBACKGROUND(terminal,mouse_line,mouse_col);
	use_attr = (fg == WHITE) && (bg == BLACK);
    }
    else
	use_attr = 1;
    
    TFIND_CURSOR(terminal,curline,curcol);
    if ( (curline != mouse_line) || (curcol != mouse_col) )
	tmove_to(terminal,mouse_line,mouse_col);

    if ( use_attr )
    {
	save_modes = TCUR_MODES(terminal);
	if ( TCOLOR_TERM(terminal) )
	{
	    if ( TCUR_FOREGROUND(terminal) != WHITE )
		tset_foreground(terminal,WHITE);
	    if ( TCUR_BACKGROUND(terminal) != BLACK )
		tset_background(terminal,BLACK);
	}
	tset_modes(terminal,TMODES(terminal,mouse_line,mouse_col));

	/* Display on screen without changing attributes in image */
	putc(TCHAR(terminal,mouse_line,mouse_col),terminal->fpout);
	
	tset_modes(terminal,save_modes);
    }
    else
    {
	save_fg = TCUR_FOREGROUND(terminal);
	save_bg = TCUR_BACKGROUND(terminal);
	tset_foreground(terminal,fg);
	tset_background(terminal,bg);
	tset_modes(terminal,TMODES(terminal,mouse_line,mouse_col));

	/* Display on screen without changing attributes in image */
	putc(TCHAR(terminal,mouse_line,mouse_col),terminal->fpout);
    
	tset_foreground(terminal,save_fg);
	tset_background(terminal,save_bg);
    }
    tmove_to(terminal,curline,curcol);
}


/************************************************************************
 * Author: Jason Bacon
 * Description:
 * Arguments:
 * Return values:
 * Date:
 * Modifications:
 ************************************************************************/
 
void    update_pos(d,pos,max)
int     d, max, *pos;

{
    double  new_pos;
    
    if ( d != 0 )
    {
	new_pos = *pos + d;
	if ( new_pos < 0 )
	    *pos = 0;
	else if ( new_pos > max )
	    *pos = max;
	else
	    *pos = new_pos;
    }
}

