#include "twintk.h"

/*
 * Set default attributes for a window
 */
 
void 
tw_set_win_attr (win_t *win, int normal_mode, int normal_fg, int normal_bg, int highlight_mode, int highlight_fg, int highlight_bg)

{
    win->win_attr.normal.mode = normal_mode;
    win->win_attr.normal.foreground = normal_fg;
    win->win_attr.normal.background = normal_bg;
    win->win_attr.highlight.mode = highlight_mode;
    win->win_attr.highlight.foreground = highlight_fg;
    win->win_attr.highlight.background = highlight_bg;
}

