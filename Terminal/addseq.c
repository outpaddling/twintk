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
* Add the character sequence "new_str" to array of sequences "seq"
* Each sequence consists of a string and a value.  The get_seq()     
* function will find the string and return the value associated
* with it.  Strings are stored in sorted order for efficient lookup
* by get_seq().
**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <xtend/string.h>   // strviscpy()
#include "twintk_term.h"

int     add_seq(seq_table, new_str, value, max_seq)
sequ_t  seq_table[];
char    new_str[];
int     value, max_seq;

{
    int             s, c, diff = 1;
    static int      count = 0;
    unsigned char   temp[128];

    if (new_str == NULL)
	return(NULL_SEQ_STR);
    
    /* Room left in array? */
    if ( count == max_seq )
	return(SEQ_TABLE_FULL);
    
    /* Mark current end of array for the loop that follows */
    seq_table[count].string = NULL;
    
    /* Find correct spot for new sequence */
    for (s = 0; (s < count) &&
		((diff = strcmp(seq_table[s].string, new_str)) > 0); ++s)
	;
    
    /* Insert new sequence */
    if ( (diff < 0) || (s == count) )
    {
	/*char    temp[101];
	
	strviscpy(temp,new_str,100);
	printf("Adding %s at position %d\r\n",temp,s);*/
	for (c = count; c > s; --c)
	{
	    seq_table[c].string = seq_table[c-1].string;
	    seq_table[c].value = seq_table[c-1].value;
	}
	seq_table[s].string = strdup(new_str);
	seq_table[s].value = value;
	seq_table[++count].string = NULL;
	return (0);
    }
    strviscpy(temp, (unsigned char *)new_str, 127);
    /*fprintf(stderr,"add_seq(): Warning: Sequence %s, key %d collided with existing key %d.\n",
	new_str, value, seq_table[s].value);*/
    return(SEQ_COLLISION);  /* Diff == 0 (collision) */
}

