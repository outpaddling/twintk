#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <xtend/string.h>   // strlcpy() on Linux
#include "twintk.h"

#define GOTO_FIELD(win,field)\
    tw_move_to(win, (field)->line, (field)->col + strlen((field)->prompt));\
    if ( !TNO_COLOR_VIDEO(win->terminal,NCV_BOLD) )\
	TW_SET_MODES(win,original_mode|BOLD_MODE);\
    TCURSOR_NORMAL(win->terminal)

int     tw_input_panel(win, panel, help_line)
win_t   *win;
tw_panel_t *panel;
int     help_line;

{
    int     c, c2, ch, nfields = panel->count;
    char   *help;
    int    original_mode = TW_CUR_MODES(win);
    event_t event;
    int         *menu_str_link = (int *)malloc(nfields*sizeof(int));
    tw_field_t *fields = panel->fields;
    
    /* Go into cbreak mode for input session */
    tpush_tty(win->terminal);
    tcbreak_mode(win->terminal);
    
    /* Display current data in fields */
    tw_print_current(win, fields, nfields);

    /* Display help string */
    help = "Move: (Arrows or TAB),  [ OK ] (Enter),  [ Cancel ] (Esc)";
    tw_print_center(win, help_line, help);

    /* Initialize fields */
    tw_init_fields(fields,menu_str_link,nfields);
    
    /* Input new data */
    c = 0;
    do
    {
	TCURSOR_INVISIBLE(win->terminal);
	TW_SET_MODES(win,original_mode);

	/* Print help message */
	tw_move_to(win,help_line-1,1);
	tw_blank_field(win,(int)(TW_COLS(win)-strlen(fields[c].help)-1)>>1);
	tw_print_center(win, help_line-1, fields[c].help);
	tw_blank_field(win,(int)(TW_COLS(win)-strlen(fields[c].help)-1)>>1);

	if ( fields[c].data_type != TWC_MENU )
	    GOTO_FIELD(win,fields+c);
	switch (fields[c].data_type)
	{
	    case TWC_STRING:
		tw_get_string(win, fields[c].temp_data.string_ptr,
		    fields[c].string_len, fields[c].field_len,
		    fields[c].string_type, &event);
		break;
	    case TWC_SHORT:
		*fields[c].temp_data.short_ptr = (short)tw_get_int(win,
		    (long)*fields[c].temp_data.short_ptr,
		    fields[c].short_min, fields[c].short_max, 10, &event);
		break;
	    case TWC_INT:
		*fields[c].temp_data.int_ptr = (int)tw_get_int(win,
		    (long)*fields[c].temp_data.int_ptr, 
		    fields[c].int_min, fields[c].int_max, 10, &event);
		break;
	    case TWC_UINT:
		*fields[c].temp_data.int_ptr = (int)tw_get_uint(win,
		    (long)*fields[c].temp_data.uint_ptr, 
		    fields[c].uint_min, fields[c].uint_max, 10, &event);
		break;
	    case TWC_LONG:
		*fields[c].temp_data.long_ptr = tw_get_int(win,
		    *fields[c].temp_data.long_ptr,
		    fields[c].long_min, fields[c].long_max, 10, &event);
		break;
	    case TWC_ENUM:
		ch = tw_get_enum(win, fields[c].temp_data.string_ptr,
		    fields[c].enum_list, fields[c].string_len, &event);
		break;
	    case TWC_MENU:
		ch = tw_get_menu(fields[c].win, fields[c].temp_data.string_ptr,
			fields[c].enum_list, fields[c].string_len, &event);
		/* If this menu is linked to a string field, copy */
		/* the new value to the string field's temp data */
		if ( menu_str_link[c] >= 0 )
		{
		    c2 = menu_str_link[c];
		    strlcpy(fields[c2].temp_data.string_ptr,
			    fields[c].temp_data.string_ptr,fields[c2].string_len);
		    GOTO_FIELD(win,fields+c2);
		    tw_blank_field(win,fields[c2].field_len);
		    tw_puts(win,fields[c2].temp_data.string_ptr);
		}
		if ( ch == KEY_MOUSE )
		{
		    event.mouse_line += 
			(TW_SCREEN_LINE(fields[c].win)-TW_SCREEN_LINE(win));
		    event.mouse_col +=
			(TW_SCREEN_COL(fields[c].win)-TW_SCREEN_COL(win));
		}
		break;
	    case TWC_FLOAT:
		*fields[c].temp_data.float_ptr = tw_get_float(win,
		    *fields[c].temp_data.float_ptr,
		    fields[c].float_min, fields[c].float_max, &event);
		break;
	    case TWC_DOUBLE:
		*fields[c].temp_data.double_ptr = tw_get_double(win,
		    *fields[c].temp_data.double_ptr,
		    fields[c].double_min, fields[c].double_max, &event);
		break;
	    default:
		ch = ' ';   /* OK? */
		break;
	}
	
	/* Return value from any above input function determines where
	   we go from here */
	ch = event.value;
	
	switch(ch)
	{
	    case    16:
	    case    KEY_UP:     /* Go to previous field */
		c = ((c - 1) + nfields) % nfields;
		break;
	    case    KEY_ENTER:  /* Go to next field */
	    case    '\n':
	    case    '\r':
		ch = TWC_INPUT_DONE;
		break;
	    case    '\033':
		ch = TWC_INPUT_CANCEL;
		break;
	    case    KEY_DOWN:
	    case    '\t':
		c = (c + 1) % nfields;
		break;
	    case    KEY_MOUSE:
		if ( event.type != BUTTON1_PRESS )
		    break;
		ch = tw_mouse_event(win,fields,&event,help,help_line,
				    nfields,ch,&c);
		break;
	    default:
		break;
	}
    }   while ( (ch != TWC_INPUT_DONE) && (ch != TWC_INPUT_CANCEL) );
    
    if ( ch == TWC_INPUT_DONE )
	tw_copy_data(fields,nfields,menu_str_link);
    tw_free_temp_fields(fields,nfields);
    
    TW_SET_MODES(win,original_mode);
    tpop_tty(win->terminal);
    free(menu_str_link);
    return c | (ch << 8);   /* Return the field # existed from */
}


void    tw_init_fields(fields,menu_str_link,nfields)
tw_field_t fields[];
int     menu_str_link[], nfields;

{
    int     c, c2;
    
    /*sprintw(2,50,"Init_fields: nfields = %d",nfields);
    getchar();*/
    for (c=0; c<nfields; ++c)
    {
	menu_str_link[c] = -1;
	
	/* Allocate temp space */
	switch(fields[c].data_type)
	{
	    case    TWC_MENU:
		for (c2=0; c2<nfields; ++c2)
		    if ( (fields[c2].data_type == TWC_STRING) &&
			 (fields[c2].data.string_ptr == fields[c].data.string_ptr) )
			menu_str_link[c] = c2;
	    case    TWC_STRING:
	    case    TWC_ENUM:
		fields[c].temp_data.string_ptr = 
		    MALLOC(fields[c].string_len+1,char);
		strlcpy(fields[c].temp_data.string_ptr,
			fields[c].data.string_ptr,fields[c].string_len);
		break;
	    case    TWC_LONG:
		fields[c].temp_data.long_ptr = MALLOC(1,long);
		*fields[c].temp_data.long_ptr = *fields[c].data.long_ptr;
		break;
	    case    TWC_INT:
		fields[c].temp_data.int_ptr = MALLOC(1,int);
		*fields[c].temp_data.int_ptr = *fields[c].data.int_ptr;
		break;
	    case    TWC_UINT:
		fields[c].temp_data.uint_ptr = MALLOC(1,unsigned int);
		*fields[c].temp_data.uint_ptr = *fields[c].data.uint_ptr;
		break;
	    case    TWC_SHORT:
		fields[c].temp_data.short_ptr = MALLOC(1,short);
		*fields[c].temp_data.short_ptr = *fields[c].data.short_ptr;
		break;
	    case    TWC_FLOAT:
		fields[c].temp_data.float_ptr = MALLOC(1,float);
		*fields[c].temp_data.float_ptr = *fields[c].data.float_ptr;
		break;
	    case    TWC_DOUBLE:
		fields[c].temp_data.double_ptr = MALLOC(1,double);
		*fields[c].temp_data.double_ptr = *fields[c].data.double_ptr;
		break;
	    case    TWC_CHAR:
	    case    TWC_UCHAR:
	    case    TWC_USHORT:
	    case    TWC_ULONG:
	    case    TWC_LONG_LONG:
	    case    TWC_ULONG_LONG:
	    case    TWC_LONG_DOUBLE:
		break;
	}
    }
}


int     tw_mouse_event(win,fields,event,help,help_line,nfields,ch,c)
win_t   *win;
tw_field_t fields[];
event_t *event;
char    *help;
int     help_line,nfields,ch,*c;

{
    int     done_col, cancel_col, c2;
    
    /* Clicked on "Done" or "Cancel"? */
    done_col = TW_CENTER_STRING_COL(win,help) + strstr(help,"[ OK ]") - help;
    cancel_col = TW_CENTER_STRING_COL(win,help) + strstr(help,"[ Cancel ]") - help;
    if (event->mouse_line == help_line)
    {
	if ( (event->mouse_col >= done_col) &&
	     (event->mouse_col < done_col+6) )
	    ch = TWC_INPUT_DONE;    /* Done */
	else if ( (event->mouse_col >= cancel_col) &&
	     (event->mouse_col < cancel_col+10) )
	    ch = TWC_INPUT_CANCEL;    /* Done */
    }
    else    /* Didn't click done or cancel */
    {
	for (c2=0; c2<nfields; ++c2)
	{
	    if ( fields[c2].data_type == TWC_MENU )
	    {
		if ( tw_mouse_in_win(fields[c2].win,win,event) )
		{
		    /* Set field number to go to menu field */
		    *c = c2;
		    
		    /* Adjust coordinates of event to menu window */
		    event->mouse_line += TW_SCREEN_LINE(win) -
			    TW_SCREEN_LINE(fields[c2].win);
		    event->mouse_col += TW_SCREEN_COL(win) -
			    TW_SCREEN_COL(fields[c2].win);
		    event->value = TWC_CLICK_MENU;
		    break;
		}
	    }
	    /* Non-menu field */
	    else if ( (event->mouse_line == fields[c2].line) &&
		 (event->mouse_col >= fields[c2].col) &&
		 (event->mouse_col < fields[c2].col+
		    (int)strlen(fields[c2].prompt)+fields[c2].field_len) )
	    {
		*c = c2;
		break;
	    }
	}
    }
    return ch;
}


void    tw_copy_data(fields,nfields,menu_str_link)
tw_field_t fields[];
int     nfields, menu_str_link[];

{
    int     c, c2;
    
    for (c=0; c<nfields; ++c)
    {
	switch(fields[c].data_type)
	{
	    case    TWC_STRING:
	    case    TWC_ENUM:
	    case    TWC_MENU:
		if ( (c2 = menu_str_link[c]) >= 0 )
		    strlcpy(fields[c].data.string_ptr,
			    fields[c2].temp_data.string_ptr,
			    fields[c].string_len);
		else
		    strlcpy(fields[c].data.string_ptr,
			    fields[c].temp_data.string_ptr,
			    fields[c].string_len);
		break;
	    case    TWC_LONG:
		*fields[c].data.long_ptr = *fields[c].temp_data.long_ptr;
		break;
	    case    TWC_INT:
		*fields[c].data.int_ptr = *fields[c].temp_data.int_ptr;
		break;
	    case    TWC_UINT:
		*fields[c].data.uint_ptr = *fields[c].temp_data.uint_ptr;
		break;
	    case    TWC_SHORT:
		*fields[c].data.short_ptr = *fields[c].temp_data.short_ptr;
		break;
	    case    TWC_DOUBLE:
		*fields[c].data.double_ptr = *fields[c].temp_data.double_ptr;
		break;
	    case    TWC_FLOAT:
		*fields[c].data.float_ptr = *fields[c].temp_data.float_ptr;
		break;
	    case    TWC_CHAR:
	    case    TWC_UCHAR:
	    case    TWC_USHORT:
	    case    TWC_ULONG:
	    case    TWC_LONG_LONG:
	    case    TWC_ULONG_LONG:
	    case    TWC_LONG_DOUBLE:
		break;
	}
    }
}


void    tw_free_temp_fields(fields,nfields)
tw_field_t fields[];
int     nfields;

{
    int         c;

    /* It's a union, so string_ptr == int_ptr == ... */
    for (c=0; c<nfields; ++c)
	free(fields[c].temp_data.string_ptr);
}

