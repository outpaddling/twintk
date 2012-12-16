#include <string.h>
#include <ctype.h>
#include "twintk.h"

void    tw_init_string(tw_panel_t *panel, int line, int col, int string_len, int field_len,
			tw_str_t tw_str_type, char *prompt, char *help, char *string_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->string_len = string_len;
    field->field_len = field_len;
    field->data_type = TWC_STRING;
    field->string_type = tw_str_type;
    field->prompt = prompt;
    field->help = help;
    field->data.string_ptr = string_ptr;
}

