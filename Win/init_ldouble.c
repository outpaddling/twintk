#include <string.h>
#include <ctype.h>
#include <xtend.h>
#include "twintk.h"

void    tw_init_ldouble(tw_panel_t *panel, int line, int col,
			long double  min, long double max, char *prompt, char *help,
			long double  *ldouble_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->data_type = TWC_LONG_DOUBLE;
    field->ldouble_min = min;
    field->ldouble_max = max;
    field->prompt = prompt;
    field->help = help;
    field->data.ldouble_ptr = ldouble_ptr;
    field->field_len = 20;
}

