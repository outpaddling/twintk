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

/*********************************************************************
* Read a key sequence and return an integer identifying the sequence 
* The strings and associated integer values are stored in the array
* "seq", sorted by string for fast lookup.  When a sequence is entered
* fully, the value in the same structure is returned.
**********************************************************************/

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include "twintk_term.h"

int     get_seq(table, first, stream, key_seq, max_len)
sequ_t  table[];
int     first;
FILE    *stream;
char    key_seq[];
size_t  max_len;

{
    int     s,
	    len = 1,
	    start = 0,
	    diff = 0,
	    chars = 1;
    char    buff[2];
    fd_set  readfds;
    struct timeval timeout;
    
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    
    key_seq[0] = first;
    while ( (len < max_len) && (table[start].string != NULL) && (chars > 0) )
    {
	/* Skip all sequences > key_seq so far */
	for (s=start; (table[s].string != NULL) &&
	    ((diff = memcmp(table[s].string,key_seq,len)) > 0); ++s)
	    ;
	
	/* If match so far, see if end */
	if ( (table[s].string != NULL) && (diff == 0) )
	{
	    if ( table[s].string[len] == '\0' )
	    {
		return(table[s].value);
	    }
	    else
	    {
		FD_ZERO(&readfds);
		FD_SET(0,&readfds);
		chars = select(1,&readfds,NULL,NULL,&timeout);
		if ( chars == 1 )
		{
		    read(fileno(stream),buff,1);
		    key_seq[len++] = *buff;
		    start = s;
		}
	    }
	}
	else    /* If no match, quit now */
	{
	    key_seq[len] = '\0';
	    return(UNDEFINED_KEY);
	}
    }
    
    /* Checked every sequence in table - not found */
    key_seq[len] = '\0';
    if ( len == max_len )
	return SEQ_TOO_LONG;
    else
	return UNDEFINED_KEY;
}

