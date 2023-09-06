#include <string.h>
#include <ctype.h>
#include <xtend/math.h>
#include "twintk.h"

void    tw_init_long(tw_panel_t *panel, int line, int col, long min, long max,
		    char *prompt, char *help, long *long_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->data_type = TWC_LONG;
    field->long_min = min;
    field->long_max = max;
    field->prompt = prompt;
    field->help = help;
    field->data.long_ptr = long_ptr;
    field->field_len = MAX(xt_digits(max,10),xt_digits(min,10));
}

