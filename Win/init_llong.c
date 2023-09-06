#include <string.h>
#include <ctype.h>
#include <xtend/math.h>
#include "twintk.h"

void    tw_init_llong(tw_panel_t *panel, int line, int col, long long min,
			long long max, char *prompt, char *help, long long *llong_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->data_type = TWC_LONG_LONG;
    field->llong_min = min;
    field->llong_max = max;
    field->prompt = prompt;
    field->help = help;
    field->data.llong_ptr = llong_ptr;
    /* FIXME: need digits that takes long long */
    field->field_len = MAX(xt_digits(max,10),xt_digits(min,10));
}

