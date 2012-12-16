#include <string.h>
#include <ctype.h>
#include "twintk.h"

void    tw_init_enum(panel, line, col, len, list, prompt, help, string_ptr)
tw_panel_t *panel;
int line, col, len;
char    *list[], *prompt, *help, *string_ptr;

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->string_len = field->field_len = len;
    field->data_type = TWC_ENUM;
    field->prompt = prompt;
    field->enum_list = list;
    field->help = help;
    field->data.string_ptr = string_ptr;
}
