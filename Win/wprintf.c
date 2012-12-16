#include <stdio.h>
#include <stdarg.h>
#include "twintk.h"

#if defined(__STDC__) || defined(SCO_SV)
int     tw_printf(win_t *win,const char *format,...)
#else
int     tw_printf(win,format)
win_t   *win;
const char    *format;
#endif

{
    va_list list;
    char    temp[1024];
    int     numchars;
    
    va_start(list,format);
    numchars = vsprintf(temp,format,list);
    tw_puts(win,temp);
    return(numchars);
}
