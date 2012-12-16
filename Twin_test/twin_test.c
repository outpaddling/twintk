#include <twintk.h>

int     main(int argc,char *argv[])

{
    term_t  *terminal;
    
    if ( (terminal = init_term(stdin, stdout, stderr, NULL, MOUSE_ON)) == NULL )
    {
	fprintf(stderr, "Cannot open terminal.\n");
	return 1;
    }
    
    tset_modes(terminal, NORMAL_MODE);
    tset_foreground(terminal, WHITE);
    tset_background(terminal, BLUE);
    tclear_screen(terminal);
    tgetc(terminal);
    
    tset_foreground(terminal, WHITE);
    tset_background(terminal, GREEN);
    tclear_screen(terminal);
    tgetc(terminal);
    
    tset_foreground(terminal, WHITE);
    tset_background(terminal, BLACK);
    tclear_screen(terminal);
    tputs(terminal, "Hello, world!");
    tgetc(terminal);
    
    tset_foreground(terminal, BLACK);
    tset_background(terminal, WHITE);
    tclear_screen(terminal);
    tputs(terminal, "Hello, world!");
    tgetc(terminal);
    
    return 0;
}

