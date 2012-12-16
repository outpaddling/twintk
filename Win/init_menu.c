#include "twintk.h"

void    tw_init_menu(tw_panel_t *panel, win_t *win, int string_len, 
		    char *list[], char *help, char *string_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->win = win;
    field->enum_list = list;
    field->string_len = string_len;
    field->data_type = TWC_MENU;
    field->help = help;
    field->data.string_ptr = string_ptr;
}

