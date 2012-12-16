/*
    Copyright (c) 1994-2003, Jason W. Bacon, Acadix Software Systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer. Redistributions
    in binary form must reproduce the above copyright notice, this list of
    conditions and the following disclaimer in the documentation and/or
    other materials provided with the distribution. 

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

#include <stdio.h>
#include <string.h>

int     main()

{
    char    name[100],type[100],line[1001];
    int     subscript,count;
    FILE    *infile;
#ifdef COHERENT
    char    *format = "#define %s __CUR __TERM %s [%d]\n",
	    *booleans = "(Booleans)",
	    *numbers = "(Numbers)",
	    *strings = "(Strings)",
	    *ostype = "COHERENT",
	    *filename = "/usr/include/term.h";
#endif
#ifdef FreeBSD
    char    *format1 = "#define %s (_CUR_TERM.%5s[%d])\n",
	    *format2 = "#define %s (_CUR_TERM.%4s[%d])\n",
	    *booleans = "bools",
	    *numbers = "nums",
	    *strings = "strs",
	    *ostype = "FreeBSD",
	    *filename = "/usr/include/term.h";
#endif
#ifdef linux
    char    *format1 = "#define %s CUR %8s[%d]\n",
	    *format2 = "#define %s CUR %7s[%d]\n",
	    *booleans = "Booleans",
	    *numbers = "Numbers",
	    *strings = "Strings",
	    *ostype = "linux",
	    *filename = "/usr/include/ncurses/term.h";
#endif

    infile = fopen(filename,"r");
    if ( infile == NULL )
    {
	fprintf(stderr,"Could not open %s.\n",filename);
	exit(1);
    }
    printf("#ifdef %s\n",ostype);
    while ( !feof(infile) )
    {
	fgets(line,1000,infile);
	count = sscanf(line, format1, name, type, &subscript);
	if ( count != 3 )
	    count = sscanf(line, format2, name, type, &subscript);
	if ( count == 3 )
	{
	    if ( strcmp(type,booleans) == 0 )
	    {
		printf("    boolean %s;     /* %d */\n",name,subscript);
	    }
	    else if ( strcmp(type,numbers) == 0 )
	    {
		printf("    int     %s;     /* %d */\n",name,subscript);
	    }
	    else if ( strcmp(type,strings) == 0 )
	    {
		printf("    char    *%s;    /* %d */\n",name,subscript);
	    }
	    else
		fprintf(stderr,"Unknown type: %s\n",type);
	}
    }
    puts("#endif");
    fclose(infile);
    return 0;
}

