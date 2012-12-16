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

/****************************************************************************
 * Title:
 * Author:
 * Created:
 * Modifications:
 * Arguments:
 * Return values:
 * Description:
 ****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>

int     main()

{
    int     fd, ch, bytes, c;
    unsigned char    buff[3];
    struct termios  term;
    
    if ( (fd = open("/dev/mouse",O_RDONLY,0755)) == -1 )
    {
	fprintf(stderr,"Cannot open mouse.\n");
	return 1;
    }
    tcgetattr(fd,&term);
    printf("%x %x\n",term.c_cflag,term.c_lflag);
    term.c_lflag &= ~ICANON;
    term.c_cflag |= CREAD;
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(fd,TCSANOW,&term);
    
    /*
    FILE    *fp;
    
    fp = fopen("/dev/mouse","r");
    */
    while (1)
    {
	bytes = read(fd,buff,3);
	if ( bytes > 0 )
	{
	    for (c=0; c<bytes; ++c)
		printf("%02x ",buff[c]);
	    putchar('\n');
	    fflush(stdout);
	}
	*buff = 0;
    }
    close(fd);
    return 0;
}

