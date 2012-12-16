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

#ifndef _SYS_TERMIOS_H_
#include <termios.h>
#endif

#define __IMAGE_H__

#define SUCCESS 0
#define FAILURE -1

#define MAXPATH 1024

#ifndef MIN
#define MIN(a,b)    ((a) < (b) ? (a) : (b))
#endif

#ifndef MALLOC
#define MALLOC(nelem,type)   (type *)malloc(nelem*sizeof(type))
#endif

#ifndef REALLOC
#define REALLOC(ptr,nelem,type) ((type *)realloc((ptr),(nelem)*sizeof(type)))
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE -1     /* All bits 1 */
#endif

typedef int boolean;

typedef struct
{   
    short  mode;           /* Underline, bright, etc. */
    char   foreground;     /* Foreground color */
    char   background;     /* Background color */
}   attr_t;

typedef struct node_st node_t;

struct node_st
{   
    struct termios  mode;
    node_t          *next;
};

/* upper_left should be first and right_tee last to stay compatible
   with functions that loop through the structure */
typedef struct
{
    unsigned char   upper_left;     /* Keep this one first for looping. */
    unsigned char   upper;
    unsigned char   upper_right;
    unsigned char   right;
    unsigned char   lower_right;
    unsigned char   lower;
    unsigned char   lower_left;
    unsigned char   left;
    unsigned char   divider;
    unsigned char   left_tee;
    unsigned char   right_tee;      /* Keep this one last for looping. */
}   border_t;

#define UPPER_LEFT(b)   ((b)->upper_left)
#define UPPER(b)        ((b)->upper)
#define UPPER_RIGHT(b)  ((b)->upper_right)
#define RIGHT(b)        ((b)->right)
#define LOWER_RIGHT(b)  ((b)->lower_right)
#define LOWER(b)        ((b)->lower)
#define LOWER_LEFT(b)   ((b)->lower_left)
#define LEFT(b)         ((b)->left)
#define DIVIDER(b)      ((b)->divider)
#define LEFT_TEE(b)     ((b)->left_tee)
#define RIGHT_TEE(b)    ((b)->right_tee)

typedef struct
{
    unsigned char   **buff;     /* 2-D array for screen image */
    attr_t          **attr;

    int    lines;               /* terminal screen/window dimensions */
    int    cols;
    
    int    cur_line;            /* Cursor position */
    int    cur_col;
    
    int    scroll_top;          /* Scroll region */
    int    scroll_bottom;

    border_t    border;         /* Border characters ASCII codes */
    attr_t      cur_attr;       /* Current color and attributes */
    boolean     auto_scroll;    /* Scroll when bottom right reached */
    boolean     insert_mode;    /* Enable insert mode */
    boolean     wrap_on;        /* Enable wraparound right edge */
    boolean     sub_image;      /* Image is a sub image */
}   image_t;

extern int TERM_DEBUG;

/**********************
 * Attributes & colors
 **********************/

/* Color */
#define BLACK   0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define MAGENTA 5
#define CYAN    6
#define WHITE   7

/* Window and image traits */
#define PLAIN             0x0000
#define NO_AUTO_SCROLL    0x0001
#define NO_WRAP           0x0002
#define INSERT_MODE       0x0004
#define OUT_BORDER        0x0008
#define IN_BORDER         0x0010
#define SHADOW            0x0020
#define NO_ACS            0x0040
#define NO_COLOR          0x0080

#define BOOL_TO_NO_AUTO_SCROLL(b)  ((b!=0)*NO_AUTO_SCROLL)
#define BOOL_TO_NO_WRAP(b)         ((b!=0)*NO_WRAP)
#define BOOL_TO_NO_ACS(b)          ((b!=0)*NO_ACS)
#define BOOL_TO_NO_COLOR(b)        ((b!=0)*NO_COLOR)

/* Attributes.  draw_modes_bar() depends on NORMAL_MODE being first and
   HIGHLIGHT_MODE last */
#define NORMAL_MODE      0x0001
#define BLINK_MODE       0x0002
#define REVERSE_MODE     0x0004
#define BRIGHT_MODE      0x0008
#define HALF_BRIGHT_MODE 0x0010
#define UNDERLINE_MODE   0x0020
#define STANDOUT_MODE    0x0040
#define HIGHLIGHT_MODE   0x0080
#define ACS_MODE         0x0100

/* Bit masks for terminfo ncv (no color video capability) */
#define NCV_STANDOUT      0x01
#define NCV_UNDERLINE     0x02
#define NCV_REVERSE       0x04
#define NCV_BLINK         0x08
#define NCV_DIM           0x10
#define NCV_BOLD          0x20
#define NCV_INVIS         0x40
#define NCV_PROTECT       0x80
#define NCV_ACS           0x100

/* Synonyms */
#define BOLD_MODE        BRIGHT_MODE
#define DIM_MODE         HALF_BRIGHT_MODE
#define FLASH_MODE       BLINK_MODE

#define ISET_FOREGROUND(i,c)     ((i)->cur_attr.foreground = (c))
#define ISET_BACKGROUND(i,c)     ((i)->cur_attr.background = (c))
#define IFOREGROUND(i,line,col) ((i)->attr[line][col].foreground)
#define IBACKGROUND(i,line,col) ((i)->attr[line][col].background)

#define ICUR_FOREGROUND(i)      ((i)->cur_attr.foreground)
#define ICUR_BACKGROUND(i)      ((i)->cur_attr.background)

#define IADDC(i,l,c,ch)         ( (i)->buff[l][c] = ch,\
				  (i)->attr[l][c] = (i)->cur_attr )
#define IHOME(i)                ((i)->cur_line=0, (i)->cur_col=0)
#define IINSERT_MODE_ON(i)      ((i)->insert_mode)
#define IMODES(i,line,col)      ((i)->attr[line][col].mode)
#define IATTR(i,line,col)       ((i)->attr[line][col])
#define ISET_MODES(i,m)         ((i)->cur_attr.mode = (m))
#define ICUR_MODES(i)           ((i)->cur_attr.mode)
#define ICUR_ATTR(i)            ((i)->cur_attr)
#define ICHAR(i,line,col)       ((i)->buff[line][col])

#define ICOLS(i)                ((i)->cols)
#define ILINES(i)               ((i)->lines)
#define ICUR_COL(i)             ((i)->cur_col)
#define ICUR_LINE(i)            ((i)->cur_line)
#define IFIND_CURSOR(i,l,c)     ((l) = (i)->cur_line, (c) = (i)->cur_col)
#define ISCROLL_TOP(i)          ((i)->scroll_top)
#define ISCROLL_BOTTOM(i)       ((i)->scroll_bottom)
#define ISET_AUTO_SCROLL(i)     ((i)->auto_scroll = 1)
#define IUNSET_AUTO_SCROLL(i)   ((i)->auto_scroll = 0)
#define IAUTO_SCROLL(i)         ((i)->auto_scroll)
#define IS_SUB_IMAGE(i)         ((i)->sub_image)

#ifndef __PROTO
#if defined(__STDC__) || defined(__cplusplus)
# define __PROTO(p) p
#else
# define __PROTO(p) ()
#endif
#endif


/* del_image.c */
void del_image __PROTO((image_t *image, int subwin));

/* iaddc.c */
void iaddc __PROTO((image_t *image, int l, int c, int ch));

/* ichange_sreg.c */
int ichange_scroll_region __PROTO((image_t *image, int top, int bottom));

/* iclear_eol.c */
void iclear_eol __PROTO((image_t *image));

/* iclear_eos.c */
void iclear_eos __PROTO((image_t *image));

/* iclr_image.c */
void iclear_image __PROTO((image_t *image));

/* idelete_ch.c */
void idelete_ch __PROTO((image_t *image));

/* idump.c */
void idump __PROTO((image_t *image));

/* iinsert_ch.c */
void iinsert_ch __PROTO((image_t *image, int ch));

/* imove_down.c */
int imove_down __PROTO((image_t *image));

/* imove_left.c */
int imove_left __PROTO((image_t *image));

/* imove_right.c */
int imove_right __PROTO((image_t *image));

/* imove_to.c */
int imove_to __PROTO((image_t *image, int line, int col));

/* imove_up.c */
int imove_up __PROTO((image_t *image));

/* inext_line.c */
void inext_line __PROTO((image_t *image));

/* init_image.c */
void init_image __PROTO((image_t *image, int lines, int cols, int traits));

/* iputc.c */
void iputc __PROTO((image_t *image, int ch));

/* iputs.c */
void iputs __PROTO((image_t *image, char *str));

/* iscroll_for.c */
void iscroll_forward __PROTO((image_t *image, int topline, int bottomline));

/* iscroll_rev.c */
void iscroll_reverse __PROTO((image_t *image, int topline, int bottomline));

/* new_image.c */
void new_image __PROTO((image_t *image, int lines, int cols, int traits));

/* sub_image.c */
void sub_image __PROTO((image_t *parent_image, image_t *image, int lines, int cols, int parent_line, int parent_col, int traits));
