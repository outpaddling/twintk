#include <stdarg.h>
#include "twintk.h"

#if defined(__STDC__) || defined(SCO_SV) || defined(UnixWare)
int     tw_print_at(win_t *win,int line,int col,char *format,...)
#else
int     tw_print_at(win,line,col,format)
win_t   *win;
int line,col;
char    *format;
#endif

{
    va_list list;
    char    temp[1024];
    int     numchars;
    
    if (tw_move_to(win,line,col) == SUCCESS)
    {
	va_start(list,format);
	numchars = vsnprintf(temp,1024,format,list);
	tw_puts(win,temp);
	return(numchars);
    }
    else
	return -1;  /* Cheezy */
}
