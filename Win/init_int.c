#include <string.h>
#include <ctype.h>
#include <xtend.h>
#include "twintk.h"

void    tw_init_int(tw_panel_t *panel, int line, int col, int min, int max,
	char   *prompt, char *help, int *int_ptr)

{
    tw_field_t *field;

    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);

    field->line = line;
    field->col = col;
    field->int_min = min;
    field->int_max = max;
    field->prompt = prompt;
    field->help = help;

    field->data_type = TWC_INT;
    field->data.int_ptr = int_ptr;
    field->field_len = MAX(num_size((long)max,10),num_size((long)min,10));
}


void    tw_init_uint(tw_panel_t *panel, int line, int col,
		    unsigned int min, unsigned int max,
		    char   *prompt, char *help, unsigned int *uint_ptr)

{
    tw_field_t *field;

    tw_expand_panel_if_needed(panel);
    field = TW_NEXT_FIELD(panel);

    field->line = line;
    field->col = col;
    field->uint_min = min;
    field->uint_max = max;
    field->prompt = prompt;
    field->help = help;

    field->data_type = TWC_UINT;
    field->data.uint_ptr = uint_ptr;
    /* FIXME: Need unsigned num_size */
    field->field_len = MAX(num_size(max,10),num_size(min,10));
}

