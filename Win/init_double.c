#include <string.h>
#include <ctype.h>
#include <bacon.h>
#include "twintk.h"

void    tw_init_double(tw_panel_t *panel, int line, int col, double min, double max,
			char *prompt, char *help, double  *double_ptr)

{
    tw_field_t *field;
    
    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);
    
    field->line = line;
    field->col = col;
    field->data_type = TWC_DOUBLE;
    field->double_min = min;
    field->double_max = max;
    field->prompt = prompt;
    field->help = help;
    field->data.double_ptr = double_ptr;
    field->field_len = 10;
}

