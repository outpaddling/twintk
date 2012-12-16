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

#include <string.h>
#include "twintk_image.h"

void    iscroll_forward(image,topline,bottomline)
image_t *image;
int    topline,bottomline;

{
    int    l,c;
    unsigned char        *tempbuff;
    attr_t      *tempattr;

    if ( !IS_SUB_IMAGE(image) )
    {
	/* Scroll screen image */
	tempbuff = image->buff[topline];
	tempattr = image->attr[topline];
	for (l = topline; l < bottomline; ++l)
	{
	    image->buff[l] = image->buff[l + 1];
	    image->attr[l] = image->attr[l + 1];
	}
	image->buff[bottomline] = tempbuff;
	image->attr[bottomline] = tempattr;
    }
    else
    {
	/* Scroll screen image */
	for (l = topline; l < bottomline; ++l)
	{
	    memcpy(image->buff[l],image->buff[l + 1],image->cols);
	    memcpy(image->attr[l],image->attr[l + 1],image->cols*sizeof(attr_t));
	}
    }
    
    /* Blank bottom line */
    for (c=0; c<image->cols; ++c)
    {
	image->buff[bottomline][c] = ' ';
	image->attr[bottomline][c] = image->cur_attr;
    }
}

