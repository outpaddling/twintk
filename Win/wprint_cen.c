#include <string.h>
#include "twintk.h"
#include <stdarg.h>

#if (defined(__STDC__) || defined(_CFE)) && !defined(osf1)
void    tw_print_center(win_t *win, int line, char *format,...)
#else
void    tw_print_center(win, line, format)
win_t   *win;
int line;
char   *format;
#endif

{
    va_list list;
    char    temp[1024] = "";

    va_start(list, format);
    vsnprintf(temp, 1024, format, list);
    tw_move_to(win,line,TW_CENTER_STRING_COL(win,temp));
    tw_puts(win, temp);
    va_end(list);
}
