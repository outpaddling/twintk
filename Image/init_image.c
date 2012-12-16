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

#include "twintk_image.h"

void        init_image(image,lines,cols,traits)
image_t     *image;
int    lines,cols,traits;

{
    /* Initialize screen control variables */
    image->cur_line = image->cur_col = 0;
    image->scroll_top = 0;
    image->scroll_bottom = lines-1;
    image->lines = lines;
    image->cols = cols;
    
    /* Initialize to normal video cur_attr */
    /* Force first tset_*ground to send magic sequences */
    /* If it thinks the current color = requested, it won't */
    image->cur_attr.foreground = WHITE;
    image->cur_attr.background = BLACK;
    image->cur_attr.mode = NORMAL_MODE;
    
    /* Set traits */
    image->wrap_on = !(traits & NO_WRAP);
    image->insert_mode = traits & INSERT_MODE;
    image->auto_scroll = !(traits & NO_AUTO_SCROLL);
}

