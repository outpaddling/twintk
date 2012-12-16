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
#include <stdlib.h>
#include "twintk_image.h"

void    new_image(image, lines, cols, traits)
image_t *image;
int lines, cols, traits;

{
    int line;

    /* Set up screen image buffer */
    image->buff = MALLOC(lines,unsigned char *);
    if (image->buff == NULL)
    {
	fprintf(stderr, "new_image(): Cannot allocate %d line image array.\n", lines);
	exit(1);
    }
    image->attr = MALLOC(lines,attr_t *);
    if (image->attr == NULL)
    {
	fprintf(stderr, "new_image(): Cannot allocate screen attribute array.\n");
	exit(1);
    }

    /* Allocate lines */
    for (line = 0; line < lines; ++line)
    {
	image->buff[line] = MALLOC(cols,unsigned char);
	if (image->buff[line] == NULL)
	{
	    fprintf(stderr, "new_image(): Cannot allocate screen buffer.\n");
	    exit(1);
	}
	image->attr[line] = MALLOC(cols,attr_t);
	if (image->attr[line] == NULL)
	{
	    fprintf(stderr, "new_image(): Cannot allocate screen buffer.\n");
	    exit(1);
	}
    }
    init_image(image, lines, cols, traits);
    image->sub_image = 0;
}

