#include <string.h>
#include <ctype.h>
#include <xtend.h>
#include "twintk.h"

void    tw_print_current(win, fields, nfields)
win_t   *win;
tw_field_t *fields;
int nfields;

{
    int c,digits,original_modes = TW_CUR_MODES(win);

    for (c = 0; c < nfields; ++c)
    {
	if ( fields[c].data_type != TWC_MENU )
	{
	    tw_move_to(win, fields[c].line, fields[c].col);
	    tw_puts(win, fields[c].prompt);
	    if ( !TNO_COLOR_VIDEO(win->terminal,NCV_BOLD) )
		TW_SET_MODES(win,original_modes|BOLD_MODE);
	}
	switch (fields[c].data_type)
	{
	    case TWC_STRING:
	    case TWC_ENUM:
		tw_blank_field(win, fields[c].field_len);
		tw_add_string(win, fields[c].data.string_ptr, 
			fields[c].field_len);
		break;
	    case TWC_SHORT:
	    case TWC_INT:
		digits = MAX(num_size(fields[c].int_max, 10),
			     num_size(fields[c].int_min, 10));
		tw_blank_field(win, digits);
		tw_printf(win, "%d", *fields[c].data.int_ptr);
		break;
	    case TWC_UINT:
		digits = MAX(num_size(fields[c].uint_max,10),
			     num_size(fields[c].uint_min,10));
		tw_blank_field(win, digits);
		tw_printf(win, "%u", *fields[c].data.uint_ptr);
		break;
	    case TWC_LONG:
		digits = MAX(num_size(fields[c].long_max,10),
			     num_size(fields[c].long_min,10));
		tw_blank_field(win, digits);
		tw_printf(win, "%ld", *fields[c].data.long_ptr);
		break;
	    case TWC_DOUBLE:
		digits = MAX(num_size(fields[c].double_max,10),
			     num_size(fields[c].double_min,10));
		tw_blank_field(win, digits);
		tw_printf(win, "%f", *fields[c].data.double_ptr);
		break;
	    case TWC_MENU:
		/* Display border */
		iclear_image(&fields[c].win->image);
		tw_draw_border(fields[c].win);
		if ( fields[c].win->bscroll_bar != NULL )
		    tw_draw_scroll_bar(fields[c].win->bscroll_bar);
		display_strings(fields[c].win,fields[c].enum_list,0);
		TW_RESTORE_WIN(fields[c].win);
		break;
	    default:
		fprintf(stderr, "tw_print_current(): Invalid data type.\n");
		return;
	}
	TW_SET_MODES(win,original_modes);
    }
}

