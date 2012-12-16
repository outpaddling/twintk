#include "twintk.h"

int     tw_dump_win(win,filename)
win_t   *win;
char    *filename;

{
    FILE    *fp;
    int     line,col;
    
    fp = fopen(filename,"w");
    if ( fp == NULL )
	return FAILURE;
    for (line=0; line<TW_LINES(win); ++line)
    {
	for (col=0; col<TW_COLS(win); ++col)
	    putc(TW_CHAR(win,line,col),fp);
	putc('\n',fp);
    }
    fclose(fp);
    return SUCCESS;
}

