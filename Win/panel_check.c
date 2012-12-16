#include <stdlib.h>
#include "twintk.h"

void    tw_expand_panel_if_needed(tw_panel_t *panel)

{
    if ( panel->count == 0 )
    {
	panel->array_size = 20;
	panel->fields = malloc(panel->array_size * sizeof(tw_field_t));
    }
    if ( panel->count == panel->array_size )
    {
	panel->array_size *= 2;
	panel->fields = realloc(panel->fields,
				panel->array_size * sizeof(tw_field_t));
    }
}

