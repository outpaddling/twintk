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

#ifndef _DIRECT_H_
#define _DIRECT_H_

/***********************************************************************
 * (c) Copyright 1996 by Jason W. Bacon.  All Rights Reserved.
 *
 * This copyright is protected by United States federal
 * law.  Violators WILL be prosecuted to the full extent of the law.
 *
 * Commercial software may not be copied, except under express permission
 * of the author or current owner of the copyright.
 * 
 * GNU public software may be copied and distributed provided all the
 * conditions specified in LICENSE.DOC are maintained.
 *
 * See the file LICENSE.DOC, provided with this software, for details
 * of your rights and responsibilities regarding this software.
 ***********************************************************************/
 

#if !defined(_STDIO_H_) && !defined(__STDIO_H__)
#include <stdio.h>
#endif

#include "twintk_image.h"    /* Includes term.h and termios.h */
#include "twintk_mouse.h"

enum { C_IFLAG, C_OFLAG, C_CFLAG, C_LFLAG, C_LINE };

#ifndef __PROTO
#ifdef __STDC__
#define __PROTO(p)  p
#else
#define __PROTO(p) ()
#endif
#endif

/*****************/
/* General stuff */

#define TERMINFO_DIR    "share/twintk/terminfo"

#define STACKSIZE       40

#define npush(x)    if (stack_index < STACKSIZE)\
			stack[stack_index++].num = (x);

#define npop()  ((stack_index > 0) ? stack[--stack_index].num : 0)
#define spop()  ((stack_index > 0) ? stack[--stack_index].str : (char *) 0)

typedef union
{
	int         num;
	char        *str;
}       stack_frame;

#define     MAX(a,b)    ((a) > (b) ? (a) : (b))
#define     ABS(a)      ((a) > 0 ? (a) : -(a))

#define CEIL(x) (int)((x)+1.0)

/* Control chars that are modified on output in OPOST mode */
#define TW_EOT '\004'
#define TW_TAB '\t'
#define TW_NL  '\n'
#define TW_DLE '\020'
#define TW_ESC '\033'

/******************/
/* Terminfo stuff */

#define ENV             NULL

#define TBUFF_SIZE      4096
#define MAX_TERM_NAME   40

#define TERM_MAGIC      0432
#define NAME_SIZE       128

/* Don't change these without updating the term_t structure! */
#define MAX_BOOLEANS    37
#define MAX_NUMBERS     33
#define MAX_STRINGS     392

/* Terminfo binary header */
struct header
{
    short   magic;              /* Magic Number (0432)                  */
    short   name_size;          /* Size of names section                */
    short   bool_count;         /* Number of booleans                   */
    short   num_count;          /* Number of numbers                    */
    short   str_count;          /* Number of strings                    */
    short   str_size;           /* Size of string table                 */
};

#define SWAP(x)     ( (x<<8) | ((unsigned short)x >> 8 ))
#define ODD(x)      ( (x) % 2 )

/***************************************/
/* Portable alternate character macros */

#define ACS_MISSING         0x100   /* Bit 8 is flag for missing ACS codes */

#define ACS_ULCORNER(term) ((term)->acs_map['l'])  /* upper left corner */
#define ACS_LLCORNER(term) ((term)->acs_map['m'])  /* lower left corner */
#define ACS_URCORNER(term) ((term)->acs_map['k'])  /* upper right corner */
#define ACS_LRCORNER(term) ((term)->acs_map['j'])  /* lower right corner */
#define ACS_RTEE(term)     ((term)->acs_map['u'])  /* right tee */
#define ACS_LTEE(term)     ((term)->acs_map['t'])  /* left tee */
#define ACS_BTEE(term)     ((term)->acs_map['v'])  /* bottom tee */
#define ACS_TTEE(term)     ((term)->acs_map['w'])  /* top tee */
#define ACS_HLINE(term)    ((term)->acs_map['q'])  /* horizontal line */
#define ACS_VLINE(term)    ((term)->acs_map['x'])  /* vertical line */
#define ACS_PLUS(term)     ((term)->acs_map['n'])  /* plus */
#define ACS_S1(term)       ((term)->acs_map['o'])  /* scan line 1 */
#define ACS_S9(term)       ((term)->acs_map['s'])  /* scan line 9 */
#define ACS_DIAMOND(term)  ((term)->acs_map['`'])  /* diamond */
#define ACS_CKBOARD(term)  ((term)->acs_map['a'])  /* checker board (stipple) */
#define ACS_DEGREE(term)   ((term)->acs_map['f'])  /* degree symbol */
#define ACS_PLMINUS(term)  ((term)->acs_map['g'])  /* plus/minus */
#define ACS_BULLET(term)   ((term)->acs_map['~'])  /* bullet */
#define ACS_LARROW(term)   ((term)->acs_map[','])  /* arrow pointing left */
#define ACS_RARROW(term)   ((term)->acs_map['+'])  /* arrow pointing right */
#define ACS_DARROW(term)   ((term)->acs_map['.'])  /* arrow pointing down */
#define ACS_UARROW(term)   ((term)->acs_map['-'])  /* arrow pointing up */
#define ACS_BOARD(term)    ((term)->acs_map['h'])  /* board of squares */
#define ACS_LANTERN(term)  ((term)->acs_map['i'])  /* lantern symbol */
#define ACS_BLOCK(term)    ((term)->acs_map['0'])  /* solid square block */


/******************************/
/* Sequences for keys, screen */

#define SEQ_LEN         64      /* Sequence strings are strduped(), so this
				   can be way bigger than necessary */
#define INITIAL_KEYS    256     /* Start count for term->max_keys */
#define MORE_KEYS       32      /* Increment for expanding term->max_keys */
#define UNDEFINED_KEY   -2
#define SEQ_TOO_LONG    -3

/* add_seq() errors */
#define SEQ_TABLE_FULL  -1
#define NULL_SEQ_STR    -2
#define SEQ_COLLISION   -3

/* DEC vt200/xterm mouse stuff */
#define X10_MOUSE           9
#define NORMAL_MOUSE        1000
#define HIGHLIGHT_MOUSE     1001
#define CELL_MOTION_MOUSE   1002
#define ALL_MOTION_MOUSE    1003

/* init_term flags */
#define MOUSE_ON    0x0001

#define TERMINFO_MOUSE_ON(t,m) /* DECSET with parm */\
    {\
	if ( (t)->get_mouse != NULL )\
	{\
	    raw_print(t,tparm((t)->get_mouse,m));\
	}\
    }

#define TERMINFO_MOUSE_OFF(t)\
    if ( (t)->get_mouse != NULL )\
	raw_print(t,"\033[?1000l")  /* DECRST with parm 1000  */

#define GET_TERMINFO_MOUSE_POS(t,row,col)\
	    col = tgetc(t) - 33,\
	    row = tgetc(t) - 33

/* Sequence table entries */
typedef struct
{   
    int     value;
    char    *string;
}   sequ_t;

/* Event values */
#define KEY_PRESS       0X0001
#define KEY_RELEASE     0X0002

#define BUTTON1_PRESS   0X0004
#define BUTTON2_PRESS   0X0008
#define BUTTON3_PRESS   0X0010
#define BUTTON_PRESS    (BUTTON1_PRESS|BUTTON2_PRESS|BUTTON3_PRESS)

/* Button release x should be (button press x) << 3 for tgetevent */
#define BUTTON1_RELEASE 0X0020
#define BUTTON2_RELEASE 0X0040
#define BUTTON3_RELEASE 0X0080
#define BUTTON_RELEASE  (BUTTON1_RELEASE|BUTTON2_RELEASE|BUTTON3_RELEASE)

#define MOTION          0X0100
#define BUTTON1_MOTION  (BUTTON1_PRESS|MOTION)
#define BUTTON2_MOTION  (BUTTON2_PRESS|MOTION)
#define BUTTON3_MOTION  (BUTTON3_PRESS|MOTION)
#define BUTTON_MOTION   (BUTTON_PRESS|MOTION)

/* button_status values */
#define BUTTON_DOWN     0
#define BUTTON_UP       1
#define BUTTON_UNKNOWN  -1

typedef int event_type;

typedef struct
{
    event_type  type;
    int         repeat;
    short       value;
    char        mouse_line;
    char        mouse_col;
    char        seq[SEQ_LEN+1];
}   event_t;

#define EVENT_INIT  {0,-1,0,0,0,""}

typedef enum
{
    KEY_BACKSPACE = 256,
    KEY_CATAB = 257,
    KEY_CLEAR = 258,
    KEY_CTAB = 259,
    KEY_DC = 260,
    KEY_DL = 261,
    KEY_DOWN = 262,
    KEY_EIC = 263,
    KEY_EOL = 264,
    KEY_EOS = 265,
    KEY_F0 = 266,
    KEY_F1 = 267,
    KEY_F10 = 268,
    KEY_F2 = 269,
    KEY_F3 = 270,
    KEY_F4 = 271,
    KEY_F5 = 272,
    KEY_F6 = 273,
    KEY_F7 = 274,
    KEY_F8 = 275,
    KEY_F9 = 276,
    KEY_HOME = 277,
    KEY_IC = 278,
    KEY_IL = 279,
    KEY_LEFT = 280,
    KEY_LL = 281,
    KEY_NPAGE = 282,
    KEY_PPAGE = 283,
    KEY_RIGHT = 284,
    KEY_SF = 285,
    KEY_SR = 286,
    KEY_STAB = 287,
    KEY_UP = 288,
    PKEY_KEY = 289,
    PKEY_LOCAL = 290,
    PKEY_XMIT = 291,
    KEY_A1 = 292,
    KEY_A3 = 293,
    KEY_B2 = 294,
    KEY_C1 = 295,
    KEY_C3 = 296,
    KEY_BTAB = 297,
    KEY_BEG = 298,
    KEY_CANCEL = 299,
    KEY_CLOSE = 300,
    KEY_COMMAND = 301,
    KEY_COPY = 302,
    KEY_CREATE = 303,
    KEY_END = 304,
    KEY_ENTER = 305,
    KEY_EXIT = 306,
    KEY_FIND = 307,
    KEY_HELP = 308,
    KEY_MARK = 309,
    KEY_MESSAGE = 310,
    KEY_MOVE = 311,
    KEY_NEXT = 312,
    KEY_OPEN = 313,
    KEY_OPTIONS = 314,
    KEY_PREVIOUS = 315,
    KEY_PRINT = 316,
    KEY_REDO = 317,
    KEY_REFERENCE = 318,
    KEY_REFRESH = 319,
    KEY_REPLACE = 320,
    KEY_RESTART = 321,
    KEY_RESUME = 322,
    KEY_SAVE = 323,
    KEY_SUSPEND = 324,
    KEY_UNDO = 325,
    KEY_SBEG = 326,
    KEY_SCANCEL = 327,
    KEY_SCOMMAND = 328,
    KEY_SCOPY = 329,
    KEY_SCREATE = 330,
    KEY_SDC = 331,
    KEY_SDL = 332,
    KEY_SELECT = 333,
    KEY_SEND = 334,
    KEY_SEOL = 335,
    KEY_SEXIT = 336,
    KEY_SFIND = 337,
    KEY_SHELP = 338,
    KEY_SHOME = 339,
    KEY_SIC = 340,
    KEY_SLEFT = 341,
    KEY_SMESSAGE = 342,
    KEY_SMOVE = 343,
    KEY_SNEXT = 344,
    KEY_SOPTIONS = 345,
    KEY_SPREVIOUS = 346,
    KEY_SPRINT = 347,
    KEY_SREDO = 348,
    KEY_SREPLACE = 349,
    KEY_SRIGHT = 350,
    KEY_SRSUME = 351,
    KEY_SSAVE = 352,
    KEY_SSUSPEND = 353,
    KEY_SUNDO = 354,
    KEY_F11 = 355,
    KEY_F12 = 356,
    KEY_F13 = 357,
    KEY_F14 = 358,
    KEY_F15 = 359,
    KEY_F16 = 360,
    KEY_F17 = 361,
    KEY_F18 = 362,
    KEY_F19 = 363,
    KEY_F20 = 364,
    KEY_F21 = 365,
    KEY_F22 = 366,
    KEY_F23 = 367,
    KEY_F24 = 368,
    KEY_F25 = 369,
    KEY_F26 = 370,
    KEY_F27 = 371,
    KEY_F28 = 372,
    KEY_F29 = 373,
    KEY_F30 = 374,
    KEY_F31 = 375,
    KEY_F32 = 376,
    KEY_F33 = 377,
    KEY_F34 = 378,
    KEY_F35 = 379,
    KEY_F36 = 380,
    KEY_F37 = 381,
    KEY_F38 = 382,
    KEY_F39 = 383,
    KEY_F40 = 384,
    KEY_F41 = 385,
    KEY_F42 = 386,
    KEY_F43 = 387,
    KEY_F44 = 388,
    KEY_F45 = 389,
    KEY_F46 = 390,
    KEY_F47 = 391,
    KEY_F48 = 392,
    KEY_F49 = 393,
    KEY_F50 = 394,
    KEY_F51 = 395,
    KEY_F52 = 396,
    KEY_F53 = 397,
    KEY_F54 = 398,
    KEY_F55 = 399,
    KEY_F56 = 400,
    KEY_F57 = 401,
    KEY_F58 = 402,
    KEY_F59 = 403,
    KEY_F60 = 404,
    KEY_F61 = 405,
    KEY_F62 = 406,
    KEY_F63 = 407,
    KEY_MOUSE = 408    /* Currently an official terminfo cap */
}   keyseq_t;

/* List of windows linked to this terminal */
/* Used for redraw following xterm resize signal, etc. */

typedef struct tw_win_st   win_t;
typedef struct winlist  winlist_t;
struct winlist
{
    win_t       *win;
    int         parent_line;
    int         parent_col;
    winlist_t   *next;
};

#define MAX_EVENT_STACK 20

/**************************************/
/* Terminal structure for t functions */

typedef struct
{
    /* Basic terminal characteristics */
    char    term_type[MAX_TERM_NAME+1]; /* vt100, ansi, vp3ap, etc. */
    char    buff[TBUFF_SIZE];
    char    mouse_pipe[L_tmpnam+1];
    FILE    *fpin,*fpout,*fperr;
    mouse_t mouse;
    int     scaled_mouse_line;
    int     scaled_mouse_col;
    int     mouse_x_scale;
    int     mouse_y_scale;
    int     mouse_pointer_visible;
    int     button_status;
    int     mouse_pid;          /* PID of server process */
    int     windowid;           /* xterm window ID */
    int     use_delays;
    int     use_alt_charset;
    int     event_mask;
    int     event_stack[MAX_EVENT_STACK];
    int     event_top;
    sequ_t  *keymap;
    int     max_keys;
    unsigned short       acs_map[256];
    winlist_t   *winlist_head, *winlist_tail;

    /* Terminal settings */
    struct termios  tty_mode,startup_tty_mode;
    node_t          *mode_stack;

    /* Screen image and control */
    image_t     image;

    /* border characters */
    int     ul,up,ur,ri,lr,lo,ll,le;
    
    /* Terminfo capabilities */
    boolean auto_left_margin;
    boolean auto_right_margin;
    boolean no_esc_ctlc;
    boolean ceol_standout_glitch;
    boolean eat_newline_glitch;
    boolean erase_overstrike;
    boolean generic_type;
    boolean hard_copy;
    boolean has_meta_key;
    boolean has_status_line;
    boolean insert_null_glitch;
    boolean memory_above;
    boolean memory_below;
    boolean move_insert_mode;
    boolean move_standout_mode;
    boolean over_strike;
    boolean status_line_esc_ok;
    boolean dest_tabs_magic_smso;
    boolean tilde_glitch;
    boolean transparent_underline;
    boolean xon_xoff;
    boolean needs_xon_xoff;
    boolean prtr_silent;
    boolean hard_cursor;
    boolean non_rev_rmcup;
    boolean no_pad_char;
    boolean non_dest_scroll_region;
    boolean can_change;
    boolean back_color_erase;
    boolean hue_lightness_saturation;
    boolean col_addr_glitch;
    boolean cr_cancels_micro_mode;
    boolean has_print_wheel;
    boolean row_addr_glitch;
    boolean semi_auto_right_margin;
    boolean cpi_changes_res;
    boolean lpi_changes_res;
    int     columns;
    int     init_tabs;
    int     lines;
    int     lines_of_memory;
    int     magic_cookie_glitch;
    int     padding_baud_rate;
    int     virtual_terminal;
    int     width_status_line;
    int     num_labels;
    int     label_height;
    int     label_width;
    int     max_attributes;
    int     maximum_windows;
    int     max_colors;
    int     max_pairs;
    int     no_color_video;
    int     buffer_capacity;
    int     dot_vert_spacing;
    int     dot_horz_spacing;
    int     max_micro_address;
    int     max_micro_jump;
    int     micro_char_size;
    int     micro_line_size;
    int     number_of_pins;
    int     output_res_char;
    int     output_res_line;
    int     output_res_horz;
    int     output_res_vert;
    int     print_rate;
    int     wide_char_size;
    int     buttons;
    int     bit_image_entwining;
    int     bit_image_type;
#if 0
    /* Twin extensions */
    int     mouse_protocol;
#endif

    char    *back_tab;
    char    *bell;
    char    *carriage_return;
    char    *change_scroll_region;
    char    *clear_all_tabs;
    char    *clear_screen;
    char    *clr_eol;
    char    *clr_eos;
    char    *column_address;
    char    *command_character;
    char    *cursor_address;
    char    *cursor_down;
    char    *cursor_home;
    char    *cursor_invisible;
    char    *cursor_left;
    char    *cursor_mem_address;
    char    *cursor_normal;
    char    *cursor_right;
    char    *cursor_to_ll;
    char    *cursor_up;
    char    *cursor_visible;
    char    *delete_character;
    char    *delete_line;
    char    *dis_status_line;
    char    *down_half_line;
    char    *enter_alt_charset_mode;
    char    *enter_blink_mode;
    char    *enter_bold_mode;
    char    *enter_ca_mode;
    char    *enter_delete_mode;
    char    *enter_dim_mode;
    char    *enter_insert_mode;
    char    *enter_secure_mode;
    char    *enter_protected_mode;
    char    *enter_reverse_mode;
    char    *enter_standout_mode;
    char    *enter_underline_mode;
    char    *erase_chars;
    char    *exit_alt_charset_mode;
    char    *exit_attribute_mode;
    char    *exit_ca_mode;
    char    *exit_delete_mode;
    char    *exit_insert_mode;
    char    *exit_standout_mode;
    char    *exit_underline_mode;
    char    *flash_screen;
    char    *form_feed;
    char    *from_status_line;
    char    *init_1string;
    char    *init_2string;
    char    *init_3string;
    char    *init_file;
    char    *insert_character;
    char    *insert_line;
    char    *insert_padding;
    char    *key_backspace;
    char    *key_catab;
    char    *key_clear;
    char    *key_ctab;
    char    *key_dc;
    char    *key_dl;
    char    *key_down;
    char    *key_eic;
    char    *key_eol;
    char    *key_eos;
    char    *key_f0;
    char    *key_f1;
    char    *key_f10;
    char    *key_f2;
    char    *key_f3;
    char    *key_f4;
    char    *key_f5;
    char    *key_f6;
    char    *key_f7;
    char    *key_f8;
    char    *key_f9;
    char    *key_home;
    char    *key_ic;
    char    *key_il;
    char    *key_left;
    char    *key_ll;
    char    *key_npage;
    char    *key_ppage;
    char    *key_right;
    char    *key_sf;
    char    *key_sr;
    char    *key_stab;
    char    *key_up;
    char    *keypad_local;
    char    *keypad_xmit;
    char    *lab_f0;
    char    *lab_f1;
    char    *lab_f10;
    char    *lab_f2;
    char    *lab_f3;
    char    *lab_f4;
    char    *lab_f5;
    char    *lab_f6;
    char    *lab_f7;
    char    *lab_f8;
    char    *lab_f9;
    char    *meta_off;
    char    *meta_on;
    char    *newline;
    char    *pad_char;
    char    *parm_dch;
    char    *parm_delete_line;
    char    *parm_down_cursor;
    char    *parm_ich;
    char    *parm_index;
    char    *parm_insert_line;
    char    *parm_left_cursor;
    char    *parm_right_cursor;
    char    *parm_rindex;
    char    *parm_up_cursor;
    char    *pkey_key;
    char    *pkey_local;
    char    *pkey_xmit;
    char    *print_screen;
    char    *prtr_off;
    char    *prtr_on;
    char    *repeat_char;
    char    *reset_1string;
    char    *reset_2string;
    char    *reset_3string;
    char    *reset_file;
    char    *restore_cursor;
    char    *row_address;
    char    *save_cursor;
    char    *scroll_forward;
    char    *scroll_reverse;
    char    *set_attributes;
    char    *set_tab;
    char    *set_window;
    char    *tab;
    char    *to_status_line;
    char    *underline_char;
    char    *up_half_line;
    char    *init_prog;
    char    *key_a1;
    char    *key_a3;
    char    *key_b2;
    char    *key_c1;
    char    *key_c3;
    char    *prtr_non;
    char    *char_padding;
    char    *acs_chars;
    char    *plab_norm;
    char    *key_btab;
    char    *enter_xon_mode;
    char    *exit_xon_mode;
    char    *enter_am_mode;
    char    *exit_am_mode;
    char    *xon_character;
    char    *xoff_character;
    char    *ena_acs;
    char    *label_on;
    char    *label_off;
    char    *key_beg;
    char    *key_cancel;
    char    *key_close;
    char    *key_command;
    char    *key_copy;
    char    *key_create;
    char    *key_end;
    char    *key_enter;
    char    *key_exit;
    char    *key_find;
    char    *key_help;
    char    *key_mark;
    char    *key_message;
    char    *key_move;
    char    *key_next;
    char    *key_open;
    char    *key_options;
    char    *key_previous;
    char    *key_print;
    char    *key_redo;
    char    *key_reference;
    char    *key_refresh;
    char    *key_replace;
    char    *key_restart;
    char    *key_resume;
    char    *key_save;
    char    *key_suspend;
    char    *key_undo;
    char    *key_sbeg;
    char    *key_scancel;
    char    *key_scommand;
    char    *key_scopy;
    char    *key_screate;
    char    *key_sdc;
    char    *key_sdl;
    char    *key_select;
    char    *key_send;
    char    *key_seol;
    char    *key_sexit;
    char    *key_sfind;
    char    *key_shelp;
    char    *key_shome;
    char    *key_sic;
    char    *key_sleft;
    char    *key_smessage;
    char    *key_smove;
    char    *key_snext;
    char    *key_soptions;
    char    *key_sprevious;
    char    *key_sprint;
    char    *key_sredo;
    char    *key_sreplace;
    char    *key_sright;
    char    *key_srsume;
    char    *key_ssave;
    char    *key_ssuspend;
    char    *key_sundo;
    char    *req_for_input;
    char    *key_f11;
    char    *key_f12;
    char    *key_f13;
    char    *key_f14;
    char    *key_f15;
    char    *key_f16;
    char    *key_f17;
    char    *key_f18;
    char    *key_f19;
    char    *key_f20;
    char    *key_f21;
    char    *key_f22;
    char    *key_f23;
    char    *key_f24;
    char    *key_f25;
    char    *key_f26;
    char    *key_f27;
    char    *key_f28;
    char    *key_f29;
    char    *key_f30;
    char    *key_f31;
    char    *key_f32;
    char    *key_f33;
    char    *key_f34;
    char    *key_f35;
    char    *key_f36;
    char    *key_f37;
    char    *key_f38;
    char    *key_f39;
    char    *key_f40;
    char    *key_f41;
    char    *key_f42;
    char    *key_f43;
    char    *key_f44;
    char    *key_f45;
    char    *key_f46;
    char    *key_f47;
    char    *key_f48;
    char    *key_f49;
    char    *key_f50;
    char    *key_f51;
    char    *key_f52;
    char    *key_f53;
    char    *key_f54;
    char    *key_f55;
    char    *key_f56;
    char    *key_f57;
    char    *key_f58;
    char    *key_f59;
    char    *key_f60;
    char    *key_f61;
    char    *key_f62;
    char    *key_f63;
    char    *clr_bol;
    char    *clear_margins;
    char    *set_left_margin;
    char    *set_right_margin;
    char    *label_format;
    char    *set_clock;
    char    *display_clock;
    char    *remove_clock;
    char    *create_window;
    char    *goto_window;
    char    *hangup;
    char    *dial_phone;
    char    *quick_dial;
    char    *tone;
    char    *pulse;
    char    *flash_hook;
    char    *fixed_pause;
    char    *wait_tone;
    char    *user0;
    char    *user1;
    char    *user2;
    char    *user3;
    char    *user4;
    char    *user5;
    char    *user6;
    char    *user7;
    char    *user8;
    char    *user9;
    char    *orig_pair;
    char    *orig_colors;
    char    *initialize_color;
    char    *initialize_pair;
    char    *set_color_pair;
    char    *set_foreground;
    char    *set_background;
    char    *change_char_pitch;
    char    *change_line_pitch;
    char    *change_res_horz;
    char    *change_res_vert;
    char    *define_char;
    char    *enter_doublewide_mode;
    char    *enter_draft_quality;
    char    *enter_italics_mode;
    char    *enter_leftward_mode;
    char    *enter_micro_mode;
    char    *enter_nr_letter_quality;
    char    *enter_normal_quality;
    char    *enter_shadow_mode;
    char    *enter_subscript_mode;
    char    *enter_superscript_mode;
    char    *enter_upward_mode;
    char    *exit_doublewide_mode;
    char    *exit_italics_mode;
    char    *exit_leftward_mode;
    char    *exit_micro_mode;
    char    *exit_shadow_mode;
    char    *exit_subscript_mode;
    char    *exit_superscript_mode;
    char    *exit_upward_mode;
    char    *micro_column_address;
    char    *micro_down;
    char    *micro_left;
    char    *micro_right;
    char    *micro_row_address;
    char    *micro_up;
    char    *order_of_pins;
    char    *parm_down_micro;
    char    *parm_left_micro;
    char    *parm_right_micro;
    char    *parm_up_micro;
    char    *select_char_set;
    char    *set_bottom_margin;
    char    *set_bottom_margin_parm;
    char    *set_left_margin_parm;
    char    *set_right_margin_parm;
    char    *set_top_margin;
    char    *set_top_margin_parm;
    char    *start_bit_image;
    char    *start_char_set_def;
    char    *stop_bit_image;
    char    *stop_char_set_def;
    char    *subscript_characters;
    char    *superscript_characters;
    char    *these_cause_cr;
    char    *zero_motion;
    char    *char_set_names;
    char    *key_mouse;
    char    *mouse_info;
    char    *req_mouse_pos;
    char    *get_mouse;
    char    *set_a_foreground;
    char    *set_a_background;
    char    *pkey_plab;
    char    *device_type;
    char    *code_set_init;
    char    *set0_des_seq;
    char    *set1_des_seq;
    char    *set2_des_seq;
    char    *set3_des_seq;
    char    *set_lr_margin;
    char    *set_tb_margin;
    char    *bit_image_repeat;
    char    *bit_image_newline;
    char    *bit_image_carriage_return;
    char    *color_names;
    char    *define_bit_image_region;
    char    *end_bit_image_region;
    char    *set_color_band;
    char    *set_page_length;
    char    *display_pc_char;
    char    *enter_pc_charset_mode;
    char    *exit_pc_charset_mode;
    char    *enter_scancode_mode;
    char    *exit_scancode_mode;
    char    *pc_term_options;
    char    *scancode_escape;
    char    *alt_scancode_esc;
    char    *enter_horizontal_hl_mode;
    char    *enter_left_hl_mode;
    char    *enter_low_hl_mode;
    char    *enter_right_hl_mode;
    char    *enter_top_hl_mode;
    char    *enter_vertical_hl_mode;

    /* Twin extensions */
#if 0
    char    *enter_button_press_mode;   /* xterm X10 compat mode */
    char    *enter_button_event_mode;   /* vt200 mode */
    char    *enter_button_track_mode;   /* vt200 highlighting mode */
    char    *enter_button_motion_mode;  /* xterm button motion extensions */
    char    *exit_mouse_mode;           /* Turn off muose reporting */
#endif
}   term_t;

/********************************
 * Terminfo macros 
 ********************************/
 
#define TTY_IFLAG_ON(t,m)       ((t)->tty_mode.c_iflag & (m))
#define TTY_OFLAG_ON(t,m)       ((t)->tty_mode.c_oflag & (m))
#define TTY_LFLAG_ON(t,m)       ((t)->tty_mode.c_lflag & (m))
#define TSTARTUP_TTY_IFLAG(t)   ((t)->startup_tty_mode.c_iflag)
#define TSTARTUP_TTY_OFLAG(t)   ((t)->startup_tty_mode.c_oflag)
#define TSTARTUP_TTY_LFLAG(t)   ((t)->startup_tty_mode.c_lflag)

#define TCAN_CLEAR_TO_EOL(t)    ((t)->clr_eol != NULL)
#define TINSERT_MODE_ON(t)      IINSERT_MODE_ON(&(t)->image)

#define TLINES(t)               ((t)->lines)
#define TCOLS(t)                ((t)->columns)
#define TCUR_LINE(t)            ICUR_LINE(&(t)->image)
#define TCUR_COL(t)             ICUR_COL(&(t)->image)
#define TFIND_CURSOR(t,l,c)     IFIND_CURSOR(&(t)->image,l,c)

#define TADDC(t,line,col,c)     IADDC(&(t)->image,line,col,c)
#define TCHAR(t,line,col)       ICHAR(&(t)->image,line,col)
#define TATTR(t,line,col)       IATTR(&(t)->image,line,col)
#define TMODES(t,line,col)      IMODES(&(t)->image,line,col)
#define TFOREGROUND(t,line,col) IFOREGROUND(&(t)->image,line,col)
#define TBACKGROUND(t,line,col) IBACKGROUND(&(t)->image,line,col)

#define TCUR_FOREGROUND(t)      ICUR_FOREGROUND(&(t)->image)
#define TCUR_BACKGROUND(t)      ICUR_BACKGROUND(&(t)->image)
#define TCUR_MODES(t)           ICUR_MODES(&(t)->image)
#define TCUR_ATTR(t)            ICUR_ATTR(&(t)->image)

#define TSCROLL_TOP(t)          ISCROLL_TOP(&(t)->image)
#define TSCROLL_BOTTOM(t)       ISCROLL_BOTTOM(&(t)->image)
#define TAUTO_SCROLL(t)         IAUTO_SCROLL(&(t)->image)
#define TSET_AUTO_SCROLL(t)     ISET_AUTO_SCROLL(&(t)->image)
#define TUNSET_AUTO_SCROLL(t)   IUNSET_AUTO_SCROLL(&(t)->image)

/* Replace tset_modes with individual macros */
#define TCURSOR_INVISIBLE(t)\
	if ( (t)->cursor_invisible != NULL )\
	    raw_print((t),(t)->cursor_invisible);\
	else\
	    tmove_to((t), TLINES((t))-1,TCOLS((t))-1);

#define TCURSOR_NORMAL(t)\
	  if ( (t)->cursor_normal != NULL )\
		raw_print((t),(t)->cursor_normal)

#define TCURSOR_VERY_VISIBLE(t)\
	  if ( (t)->cursor_visible != NULL )\
		raw_print((t),(t)->cursor_visible)

#define TREDRAW_SCREEN(t)       tredraw(t,0,0,TLINES(t)-1,TCOLS(t)-1)
#define TFLUSH_OUT(t)           fflush((t)->fpout)

/* Capability tests */
#define TCAN_CHANGE_COLORS(t)   (t)->can_change
#define THAS_ACS(t)             ((t)->acs_chars != NULL)
#define THAS_DIM_MODE(t)        ((t)->enter_dim_mode != NULL)
#define THAS_BOLD_MODE(t)       ((t)->enter_bold_mode != NULL)
#define THAS_UNDERLINE_MODE(t)  ((t)->enter_underline_mode != NULL)
#define THAS_REVERSE_MODE(t)    ((t)->enter_reverse_mode != NULL)
#define THAS_STANDOUT_MODE(t)   ((t)->enter_standout_mode != NULL)
#define THAS_BRIGHT_MODE(t)     THAS_BOLD_MODE(t)
#define THAS_HALFBRIGHT_MODE(t) THAS_DIM_MODE(t)

#define TMAX_COLORS(t)          (t)->max_colors
#define TCOLOR_TERM(t)          ((t)->max_colors > 2)
#define TNO_COLOR_VIDEO(t,m)    ( ((t)->no_color_video != -1) &&\
				  ((t)->no_color_video & (m)) )
#define TNCV(t,m)               ((t)->no_color_video & (m))

#define USE_DELAYS(t)           ((t)->use_delays = 1)
#define DONT_USE_DELAYS(t)      ((t)->use_delays = 0)

#define THAS_TERMINFO_MOUSE(t)  (((t)->get_mouse != NULL) || ((t)->key_mouse != NULL))
#define THAS_MOUSE(t)           (((t)->buttons > 0) || THAS_TERMINFO_MOUSE(t))
#define TMOUSE_LINE(t)          ((t)->scaled_mouse_line / (t)->mouse_y_scale)
#define TMOUSE_COL(t)           ((t)->scaled_mouse_col / (t)->mouse_x_scale)
#define TMOUSE_POINTER_INVISIBLE(t)\
				((t)->mouse_pointer_visible = FALSE)
#define TMOUSE_POINTER_VISIBLE(t)\
				((t)->mouse_pointer_visible = TRUE)
#define TRESET_MOUSE(t)         (t)->button_down =\
				(t)->scaled_mouse_line =\
				(t)->scaled_mouse_col = 0;\
				TMOUSE_POINTER_VISIBLE(t)
#define TRESET_TERM(t)          if ((t)->reset_1string != NULL)\
				    raw_print((t),(t)->reset_1string)
#define TENABLE_ACS(t)          if ((t)->ena_acs != NULL)\
				    raw_print((t),(t)->ena_acs)

/* Terminal event handling */
#define TEVENT_MASK(t)          ((t)->event_mask)
#define TSELECT_INPUT(t,m)      ((t)->event_mask = (m))
#define TSEND_EVENT(t,m)        ((t)->event_mask |= (m))
#define TIGNORE_EVENT(t,m)      ((t)->event_mask &= ~(m))

#ifndef __PROTO
#if __STDC__ || defined(__cplusplus)
#define __PROTO(s) s
#else
#define __PROTO(s) ()
#endif
#endif


/* addseq.c */
int add_seq __PROTO((
	sequ_t seq_table[], 
	char new_str[], 
	int value, 
	int max_seq));

/* del_term.c */
void del_term __PROTO((
	term_t **term));

/* getseq.c */
int get_seq __PROTO((
	sequ_t table[], 
	int first, 
	FILE *stream, 
	char key_seq[], 
	size_t max_len));

/* init_keymap.c */
void init_keymap __PROTO((
	term_t *terminal));
void vtxxx_keys __PROTO((
	term_t *term));

/* init_names.c */
void init_names __PROTO((
	term_t *terminal, 
	char booleans[], 
	short numbers[], 
	char *strings[]));

/* init_term.c */
term_t *init_term __PROTO((
	FILE *fpin, 
	FILE *fpout, 
	FILE *fperr, 
	char *termtype, 
	unsigned flags));
void init_acsmap __PROTO((
	term_t *terminal));
void resize_terminal __PROTO((
	term_t *terminal));

/* load_termcap.c */
int load_termcap __PROTO((
	char *term_name, 
	term_t *terminal));

/* load_tinfo.c */
int load_tinfo __PROTO((
	char *term_name, 
	term_t *terminal));

/* mouseserver.c */
void mouse_handler __PROTO((void));

/* raw_print.c */
void raw_print __PROTO((
	term_t *terminal, 
	char *str));

/* tadd_kmous.c */
void tadd_key_mouse __PROTO((
	term_t *terminal));

/* taddc.c */
void taddc __PROTO((
	term_t *terminal, 
	int ch));

/* taddseq.c */
int taddseq __PROTO((
	term_t *term, 
	char *new_str, 
	int value));

/* tcbreak_mode.c */
void tcbreak_mode __PROTO((
	term_t *terminal));

/* tchange_sreg.c */
int tchange_scroll_region __PROTO((
	term_t *terminal, 
	int top, 
	int bottom));

/* tclear_eol.c */
void tclear_eol __PROTO((
	term_t *terminal));

/* tclear_eos.c */
void tclear_eos __PROTO((
	term_t *terminal));

/* tclr_screen.c */
void tclear_screen __PROTO((
	term_t *terminal));

/* tdelete_ch.c */
void tdelete_ch __PROTO((
	term_t *terminal));

/* tdelete_line.c */
void tdelete_line __PROTO((
	term_t *terminal));

/* tdump.c */
int tdump __PROTO((
	term_t *term, 
	char *filename));

/* tget_resp.c */
int tget_resp __PROTO((
	term_t *term, 
	char *valid));

/* tgetc.c */
int tgetc __PROTO((
	term_t *terminal));

/* tgetevent.c */
int tgetevent __PROTO((
	term_t *terminal, 
	event_t *event));
int tget_key_event __PROTO((
	term_t *terminal, 
	event_t *event));
int tget_mouse_event __PROTO((
	term_t *terminal, 
	event_t *event));
void tdisplay_mouse_pointer __PROTO((
	term_t *terminal));
void terase_mouse_pointer __PROTO((
	term_t *terminal, 
	int mouse_line, 
	int mouse_col));
void update_pos __PROTO((
	int d, 
	int *pos, 
	int max));

/* tgetmouse.c */
int tget_mouse_packet __PROTO((
	term_t *terminal, 
	packet_t *packet));

/* tgets.c */
char *tgets __PROTO((
	term_t *terminal, 
	char string[], 
	int maxlen));

/* tgetseq.c */
int tgetseq __PROTO((
	term_t *terminal, 
	char seq[], 
	size_t max_len));

/* thome.c */
void thome __PROTO((
	term_t *terminal));

/* tinsert_ch.c */
void tinsert_ch __PROTO((
	term_t *terminal, 
	int ch));

/* tinsert_line.c */
void tinsert_line __PROTO((
	term_t *terminal));

/* tinsert_mode.c */
int tinsert_mode __PROTO((
	term_t *term));

/* tmousesrvr.c */
int topen_mouse_server __PROTO((
	term_t *terminal, 
	char *server, 
	char *device, 
	int type));
void tclose_mouse_server __PROTO((
	term_t *terminal));

/* tmove_down.c */
int tmove_down __PROTO((
	term_t *terminal));

/* tmove_left.c */
int tmove_left __PROTO((
	term_t *terminal));

/* tmove_right.c */
int tmove_right __PROTO((
	term_t *terminal));

/* tmove_to.c */
int tmove_to __PROTO((
	term_t *terminal, 
	int line, 
	int col));

/* tmove_up.c */
int tmove_up __PROTO((
	term_t *terminal));

/* tparm.c */
char *tparm __PROTO((
	char *string, 
	...));

/* tpop_tty.c */
int tpop_tty __PROTO((
	term_t *terminal));

/* tprintf.c */
int tprintf __PROTO((
	term_t *terminal, 
	char *format, 
	...));

/* tpush_event.c */
int tpush_event_mask __PROTO((
	term_t *terminal));
int tpop_event_mask __PROTO((
	term_t *terminal));

/* tpush_tty.c */
int tpush_tty __PROTO((
	term_t *terminal));

/* tputc.c */
void tputc __PROTO((
	term_t *terminal, 
	int ch));

/* tputs.c */
void tputs __PROTO((
	term_t *terminal, 
	char *str));

/* traw_mode.c */
void traw_mode __PROTO((
	term_t *terminal));

/* tredraw.c */
void tredraw __PROTO((
	term_t *terminal, 
	int topline, 
	int leftcol, 
	int bottomline, 
	int rightcol));

/* tredraw_line.c */
void tredraw_line __PROTO((
	term_t *terminal, 
	int line, 
	int leftcol, 
	int rightcol));
void tredraw_line_leave_cursor __PROTO((
	term_t *terminal, 
	int line, 
	int leftcol, 
	int rightcol));

/* tscroll_for.c */
void tscroll_forward __PROTO((
	term_t *terminal));

/* tscroll_rev.c */
void tscroll_reverse __PROTO((
	term_t *terminal));

/* tset_bg.c */
int tset_background __PROTO((
	term_t *terminal, 
	int color));

/* tset_dim.c */
void tset_dimensions __PROTO((
	term_t *terminal, 
	int lines, 
	int cols));

/* tset_fg.c */
int tset_foreground __PROTO((
	term_t *terminal, 
	int color));

/* tset_modes.c */
int tset_modes __PROTO((
	term_t *terminal, 
	int mode));

/* tset_start.c */
int tset_startup_tty __PROTO((
	term_t *terminal));

/* tset_tty.c */
int tset_tty __PROTO((
	term_t *terminal, 
	int flag, 
	int mode));

/* tunset_tty.c */
int tunset_tty __PROTO((
	term_t *terminal, 
	int flag, 
	int mode));

#undef __PROTO
#endif
