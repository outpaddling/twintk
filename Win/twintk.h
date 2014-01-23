#define _WINDOWS_H_

#define TWC_MAX_VIEW_COLS 256

#include "twintk_term.h"

typedef struct
{
    attr_t      normal;
    attr_t      highlight;
}   tw_attr_t;

/* Window stuff */
/* win_t typedefed in direct.h for winlist */
struct tw_win_st
{                               /* Virtual screen image information */
    int     first_vis_line;     /* Position of view win on virtal win */
    int     first_vis_col;

    /* View window information */
    int     view_lines;         /* Size of view window */
    int     view_cols;
    int     screen_line;        /* Position of view window on screen */
    int     screen_col;

    win_t   *parent_win;        /* Parent window if subwin, NULL otherwise */
    win_t   *bscroll_bar;
    win_t   *rscroll_bar;
    win_t   *tscroll_bar;
    win_t   *lscroll_bar;
    term_t *terminal;           /* Link to parent terminal */
    int traits;            /* Window traits */
    tw_attr_t  win_attr;          /* Parameters to use for menus, etc. */
    image_t image;
};

/* tw_get_item stuff */
#define TWC_HLINE           "\001"
#define TWC_OUTSIDE_WIN     -1
#define TWC_HIT_HLINE       -2
#define TWC_MENU_TEXT_LEN   30

/* Screen input stuff */
#define TWC_BUFF_LEN    256
#define TWC_BELL            '\007'
#define TWC_INVALID_DIGIT   -1
#define TWC_CLICK_MENU -2

/* tw_new_win() chooses screen_line and screen_col if given for line or col */
#define TWC_CENTER_WIN    32767

#ifndef MAX
#define MAX(n1,n2)  ( ((n1) > (n2)) ? (n1) : (n2) )
#endif

#ifndef ABS
#define ABS(n)      ( ((n) >= 0) ? (n) : -(n) )
#endif

/* screen_input() related definitions */
#define TWC_INPUT_DONE     '\004'
#define TWC_INPUT_CANCEL   '\003'

#define TW_LAST_FIELD(s)    ((s)&0xff)  /* Index of field when exited */
#define TW_EXIT_KEY(s)      ((s)>>8)    /* Which key exited screen_input() */

#define TW_END_INPUT(c,e) ( (c==KEY_ENTER) || (c=='\r') || (c=='\n') || \
			(c==16) || (c==TWC_INPUT_DONE) || \
			(c==TWC_INPUT_CANCEL) || (c == '\033') || \
			(c==KEY_DOWN) || (c == KEY_UP) || (c == '\t') ||\
			(c == KEY_MOUSE) )

/* Column where centered string should begin */
#define TW_CENTER_STRING_COL(w,s)  ((unsigned)(TW_COLS(w) - strlen(s)) >> 1)

typedef enum { TWC_ENUM, TWC_STRING, TWC_CHAR, TWC_UCHAR, TWC_SHORT, TWC_USHORT,
	    TWC_INT, TWC_UINT, TWC_LONG, TWC_ULONG, TWC_LONG_LONG, TWC_ULONG_LONG,
	    TWC_FLOAT, TWC_DOUBLE, TWC_LONG_DOUBLE, TWC_MENU }   tw_data_t;
typedef enum { TWC_VERBATIM, TWC_CAPS, TWC_UPPER_CASE, TWC_LOWER_CASE, TWC_SECURE } tw_str_t;
typedef union
{
    char                *string_ptr;
    short               *short_ptr;
    unsigned short      *ushort_ptr;
    int                 *int_ptr;
    unsigned int        *uint_ptr;
    long                *long_ptr;
    unsigned long       *ulong_ptr;
    long long           *llong_ptr;
    unsigned long long  *ullong_ptr;
    float               *float_ptr;
    double              *double_ptr;
    long double         *ldouble_ptr;
}   tw_datap_t;

/* Structure for input panels */
typedef struct
{
    win_t       *win;           /* For tw_get_menu() to list items in */
    int         line;
    int         col;
    tw_data_t   data_type;
    tw_str_t    string_type;
    char        *prompt;
    char        *help;
    int         string_len;
    int         field_len;
    union
    {
	short           short_min;
	unsigned short  ushort_min;
	int             int_min;
	unsigned int    uint_min;
	long            long_min;
	unsigned long   ulong_min;
	long long       llong_min;
	unsigned long long ullong_min;
	float           float_min;
	double          double_min;
	long double     ldouble_min;
    };
    union
    {
	short           short_max;
	unsigned short  ushort_max;
	int             int_max;
	unsigned int    uint_max;
	long            long_max;
	unsigned long   ulong_max;
	long long       llong_max;
	unsigned long long ullong_max;
	float           float_max;
	double          double_max;
	long double     ldouble_max;
    };
    char        **enum_list;    /* List of strings for enumerated input */
    tw_datap_t  data;           /* Points to actual input variables */
    tw_datap_t  temp_data;      /* Temporary data in case of cancel */
}   tw_field_t;

#define     TWC_PANEL_INIT  { 0, 0, NULL }
#define     TW_NEXT_FIELD(panel)   ((panel)->fields + (panel)->count++)
#define     TW_RESET_PANEL(panel)  ((panel)->count = 0)

/* Input panel - array of fields */
typedef struct
{
    int         count;
    int         array_size;
    tw_field_t  *fields;
}   tw_panel_t;

/* Constants for tw_get_pathname */
#define TWC_SPEC_LEN        28
#define TWC_FILENAME_LEN    FILENAME_MAX
#define TWC_MAX_FILENAMES   2000
#define TWC_ST_LEN          54
#define TWC_MAX_SPECS       20

extern int TWC_WIN_DEBUG;

#define TW_COLS(win)              ICOLS(&(win)->image)
#define TW_LINES(win)             ILINES(&(win)->image)
#define TW_CUR_COL(win)           ICUR_COL(&(win)->image)
#define TW_CUR_LINE(win)          ICUR_LINE(&(win)->image)
#define TW_CUR_VIEW_COL(win)      ( ICUR_COL(&(win)->image) \
				    - (win)->first_vis_col )
#define TW_CUR_VIEW_LINE(win)     ( ICUR_LINE(&(win)->image) \
				    - (win)->first_vis_line )
#define TW_SCREEN_LINE(win)       (win)->screen_line
#define TW_SCREEN_COL(win)        (win)->screen_col
#define TW_SCREEN_CUR_LINE(win)   (ICUR_LINE(&(win)->image) + (win)->screen_line )
#define TW_SCREEN_CUR_COL(win)    (ICUR_COL(&(win)->image) + (win)->screen_col )
#define TW_VIEW_LINES(win)        (win)->view_lines
#define TW_VIEW_COLS(win)         (win)->view_cols
#define TW_FIND_CURSOR(win,l,c)   IFIND_CURSOR(&(win)->image,l,c)

#define TW_SET_MODES(win,mode)    ISET_MODES(&(win)->image,mode)
#define TW_CUR_MODES(win)         ICUR_MODES(&(win)->image)
#define TW_CUR_FOREGROUND(win)    ICUR_FOREGROUND(&(win)->image)
#define TW_CUR_BACKGROUND(win)    ICUR_BACKGROUND(&(win)->image)
#define TW_CUR_ATTR(win)          ICUR_ATTR(&(win)->image)

#define TW_CHAR(win,l,c)          ICHAR(&(win)->image,l,c)
#define TW_ATTR(win,l,c)          IATTR(&(win)->image,l,c)
#define TW_MODES(win,l,c)         IMODES(&(win)->image,l,c)
#define TW_FOREGROUND(win,l,c)    IFOREGROUND(&(win)->image,l,c)
#define TW_BACKGROUND(win,l,c)    IBACKGROUND(&(win)->image,l,c)

#define TW_INSERT_MODE_ON(win)    IINSERT_MODE_ON(&(win)->image)
#define TW_TTY_MODE_ON(win)       ITTY_MODE_ON(&(win)->image)

#define TW_SCROLL_TOP(win)        ISCROLL_TOP(&(win)->image)
#define TW_SCROLL_BOTTOM(win)     ISCROLL_BOTTOM(&(win)->image)
#define TW_SET_AUTO_SCROLL(win)   ISET_AUTO_SCROLL(&(win)->image)
#define TW_UNSET_AUTO_SCROLL(win) IUNSET_AUTO_SCROLL(&(win)->image)
#define TW_AUTO_SCROLL(win)       IAUTO_SCROLL(&(win)->image)

#define TW_REDRAW_WIN(win)        if ( TW_HAS_SHADOW((win)) || TW_HAS_OUTBORDER((win)) )\
				    tw_redraw((win)->parent_win,0,0,TW_LINES((win)->parent_win)-1,TW_COLS((win)->parent_win)-1);\
				else\
				    tw_redraw((win),0,0,TW_LINES((win))-1,TW_COLS((win))-1)

#define TW_RESTORE_WIN(win)       if ( TW_HAS_SHADOW((win)) || TW_HAS_OUTBORDER((win)) )\
				    tw_restore((win)->parent_win,0,0,TW_LINES((win)->parent_win)-1,TW_COLS((win)->parent_win)-1);\
				else\
				    tw_restore((win),0,0,TW_LINES((win))-1,TW_COLS((win))-1)

#define TW_FLUSH_OUT(win)         TFLUSH_OUT((win)->terminal)

/* Scroll bar stuff */
#define TW_HBAR_POS(win,num,den)   (1 + (TW_COLS(win)-2) * (num) / (den))
#define TW_VBAR_POS(win,num,den)   (1 + (TW_LINES(win)-2) * (num) / (den))


/* Window (and image) traits */
#define TW_SET_TRAIT(win,t)       ((win)->traits |= (t))
#define TW_UNSET_TRAIT(win,t)     ((win)->traits &= ~(t))

#define TW_HAS_SHADOW(win)        ((win)->traits & SHADOW)
#define TW_HAS_OUTBORDER(win)     ((win)->traits & OUT_BORDER)
#define TW_NO_COLOR(win)          ((win)->traits & NO_COLOR)
#define TW_NO_ACS(win)            ((win)->traits & NO_ACS)

#define TW_IS_SUBWIN(win)         ((win)->parent_win != NULL)

#define TW_SET_HIGHLIGHT(win)\
	if ( !TW_NO_COLOR(win) && TCOLOR_TERM((win)->terminal) )\
	{\
	    tw_set_foreground(win,(win)->win_attr.highlight.foreground);\
	    tw_set_background(win,(win)->win_attr.highlight.background);\
	}\
	else\
	    TW_SET_MODES(win,(win)->win_attr.highlight.mode)

#define TW_SET_NORMAL(win)\
	if ( !TW_NO_COLOR(win) && TCOLOR_TERM((win)->terminal) )\
	{\
	    tw_set_foreground((win),(win)->win_attr.normal.foreground);\
	    tw_set_background((win),(win)->win_attr.normal.background);\
	}\
	else\
	    TW_SET_MODES((win),(win)->win_attr.normal.mode)

#define TW_NORMAL_MODE(win) ((win)->win_attr.normal.mode)
#define TW_HIGHLIGHT_MODE(win) ((win)->win_attr.highlight.mode)
#define TW_NORMAL_FG(win) ((win)->win_attr.normal.foreground)
#define TW_NORMAL_BG(win) ((win)->win_attr.normal.background)
#define TW_HIGHLIGHT_FG(win) ((win)->win_attr.highlight.foreground)
#define TW_HIGHLIGHT_BG(win) ((win)->win_attr.highlight.background)

#define TW_FULL_WIDTH_VIEW(win) \
	((win)->view_cols == TCOLS((win)->terminal))
	
#define TW_FULL_SCREEN_VIEW(win) \
	( ((win)->view_cols == TCOLS((win)->terminal)) && \
	  ((win)->view_lines == TLINES((win)->terminal)) )
	  
#define TW_BOTTOM_OF_SCREEN(win) \
	( (win)->screen_line + TW_LINES(win) == TLINES((win)->terminal) )
	
#define TW_CURSOR_IN_VIEW(win) \
	( ( TW_CUR_LINE(win) <= (win)->first_vis_line + (win)->view_lines ) && \
	  ( TW_CUR_COL(win) <= (win)->first_vis_col + (win)->view_cols ) )
	  
#define TW_SYNC_CURSOR(win) \
	if ( (TW_SCREEN_CUR_LINE((win)) != TCUR_LINE((win)->terminal)) || \
	     (TW_SCREEN_CUR_COL((win)) != TCUR_COL((win)->terminal)) ) \
	    tmove_to((win)->terminal,TW_SCREEN_CUR_LINE((win)),\
		    TW_SCREEN_CUR_COL((win)))
		    
#define TW_SYNC_COLORS(win)\
    if (TW_CUR_FOREGROUND((win)) != TCUR_FOREGROUND((win)->terminal))\
	tset_foreground(((win))->terminal,TW_CUR_FOREGROUND((win)));\
    if (TW_CUR_BACKGROUND((win)) != TCUR_BACKGROUND((win)->terminal))\
	tset_background((win)->terminal,TW_CUR_BACKGROUND((win)))
    
#define TW_SET_BORDER(win,b)  ((win)->image.border = (b))
#define TW_BORDER(win)        ((win)->image.border)

#define TW_CLEAR_IMAGE(win)   iclr_image(&(win)->image)

/* Replace tset_modes with individual macros */
#define TW_CURSOR_INVISIBLE(w)    TCURSOR_INVISIBLE((w)->terminal)
		
#define TW_CURSOR_NORMAL(w)\
	if ( (w)->terminal->cursor_normal != NULL )\
	    raw_print((w)->terminal,(w)->terminal->cursor_normal);\
	else\
	    TW_SYNC_CURSOR(w)

/* Quick scroll bars for windows with an inner border */
#define TW_NEW_RIGHT_SCROLL_BAR(win)\
	    ((win)->rscroll_bar = tw_sub_win(win, TW_LINES(win) - 2, 1, 1, TW_COLS(win)-1, NO_AUTO_SCROLL))
#define TW_NEW_BOTTOM_SCROLL_BAR(win)\
	    ((win)->bscroll_bar = tw_sub_win(win, 1, TW_COLS(win)-2, TW_LINES(win)-1, 1, NO_AUTO_SCROLL))

#define TW_NEW_LEFT_SCROLL_BAR(win)\
	    ((win)->lscroll_bar = tw_sub_win(win, TW_LINES(win) - 2, 1, 1, 0, NO_AUTO_SCROLL))
#define TW_NEW_TOP_SCROLL_BAR(win)\
	    ((win)->tscroll_bar = tw_sub_win(win, 1, TW_COLS(win)-2, 0, 1, NO_AUTO_SCROLL))

/* del_win.c */
void tw_del_win(win_t **win);
/* init_double.c */
void tw_init_double(tw_panel_t *panel, int line, int col, double min, double max, char *prompt, char *help, double *double_ptr);
/* init_enum.c */
void tw_init_enum(tw_panel_t *panel, int line, int col, int len, char *list[], char *prompt, char *help, char *string_ptr);
/* init_int.c */
void tw_init_int(tw_panel_t *panel, int line, int col, int min, int max, char *prompt, char *help, int *int_ptr);
void tw_init_uint(tw_panel_t *panel, int line, int col, unsigned int min, unsigned int max, char *prompt, char *help, unsigned int *uint_ptr);
/* init_ldouble.c */
void tw_init_ldouble(tw_panel_t *panel, int line, int col, long double min, long double max, char *prompt, char *help, long double *ldouble_ptr);
/* init_llong.c */
void tw_init_llong(tw_panel_t *panel, int line, int col, long long min, long long max, char *prompt, char *help, long long *llong_ptr);
/* init_long.c */
void tw_init_long(tw_panel_t *panel, int line, int col, long min, long max, char *prompt, char *help, long *long_ptr);
/* init_menu.c */
void tw_init_menu(tw_panel_t *panel, win_t *win, int string_len, char *list[], char *help, char *string_ptr);
/* init_string.c */
void tw_init_string(tw_panel_t *panel, int line, int col, int string_len, int field_len, tw_str_t tw_str_type, char *prompt, char *help, char *string_ptr);
/* new_win.c */
win_t *tw_new_win(term_t *terminal, int lines, int cols, int screen_line, int screen_col, int traits);
win_t *simple_win(term_t *terminal, int lines, int cols, int screen_line, int screen_col, int traits);
void check_position(int *line_ptr, int *col_ptr, int lines, int cols, int parent_lines, int parent_cols);
void default_win_attr(win_t *win);
void init_win(win_t *win, int lines, int cols);
/* panel_check.c */
void tw_expand_panel_if_needed(tw_panel_t *panel);
/* print_cur.c */
void tw_print_current(win_t *win, tw_field_t *fields, int nfields);
/* sub_win.c */
win_t *tw_sub_win(win_t *parent_win, int wlines, int wcols, int parent_line, int parent_col, int traits);
/* waddstr.c */
void tw_add_string(win_t *win, char *string, int maxlen);
/* wblank.c */
void tw_blank_field(win_t *win, int len);
/* wbut_resp.c */
void tw_draw_button(win_t *win, int line, int col, char *button);
int tw_button_response(win_t *win, int button_line, char *buttons[]);
int tw_button_action_key(char *str);
/* wclear.c */
void tw_clear_win(win_t *win);
/* wclear_eol.c */
void tw_clear_to_eol(win_t *win);
/* wclear_eow.c */
void tw_clear_to_eow(win_t *win);
/* wdelete_ch.c */
void tw_del_ch(win_t *win);
/* wdelete_line.c */
void tw_del_line(win_t *win);
/* wdump.c */
int tw_dump_win(win_t *win, char *filename);
/* wget_item.c */
int tw_get_item(win_t *win, char *menu_text[], event_t *event, int *start_line, int reverse_menu, char selected_text[]);
int mouse_action(win_t *win, char *menu_text[], event_t *event);
/* wget_path.c */
int tw_get_pathname(win_t *win, char *file_name, char *file_spec);
int list_files(char *names[], char *pattern, int file_type, size_t max_names);
int strptrcmp(char **p1, char **p2);
/* wget_resp.c */
int tw_get_response(win_t *win, char valid[]);
/* wgetc.c */
int tw_getc(win_t *win);
/* wgetenum.c */
int tw_get_enum(win_t *win, char *string, char *list[], int str_max, event_t *event);
/* wgetevent.c */
int tw_get_event(win_t *win, event_t *event);
/* wgetfloat.c */
float tw_get_float(win_t *win, double incoming, double min, double max, event_t *event);
double tw_get_double(win_t *win, double incoming, double min, double max, event_t *event);
int tw_insert_float_digit(win_t *win, char *temp, int ch, int *pos, int maxlen, int line, int col, int *len, int touched);
/* wgetint.c */
short tw_get_short(win_t *win, int incoming, int min, int max, int base, event_t *event);
int tw_get_int(win_t *win, int incoming, int min, int max, int base, event_t *event);
unsigned int tw_get_uint(win_t *win, unsigned int incoming, unsigned int min, unsigned int max, int base, event_t *event);
long tw_get_long(win_t *win, long incoming, long min, long max, int base, event_t *event);
unsigned long tw_get_ulong(win_t *win, unsigned long incoming, unsigned long min, unsigned long max, int base, event_t *event);
void tw_get_int_string(win_t *win, int maxlen, int base, event_t *event, char *temp);
int digit(int ch, int base);
/* wgetmenu.c */
int tw_get_menu(win_t *win, char *string, char *list[], int str_max, event_t *event);
void display_strings(win_t *win, char *list[], int first_str);
/* wgetseq.c */
int tw_get_seq(win_t *win, char *sequence, size_t max_len);
/* wgetstring.c */
char *tw_get_string(win_t *win, char *string, int maxlen, int field_width, tw_str_t string_type, event_t *event);
char *tw_get_string_hist(win_t *win, char *string, int maxlen, int field_width, tw_str_t string_type, event_t *event, char *history[]);
/* whigh_print.c */
void tw_high_print(win_t *win, int fg, int bg, int hlfg, int hlbg, char *text);
/* whl_menu.c */
void tw_highlight_menu_item(win_t *win, char *text, int row, int no_color, int reverse_menu, int fg, int bg, int hl_fg, int hl_bg, int main_mode, int highlight_mode, int lc, int rc);
/* whome.c */
void tw_home_win(win_t *win);
/* winn_border.c */
void tw_draw_border(win_t *win);
/* winput_pan.c */
int tw_input_panel(win_t *win, tw_panel_t *panel, int help_line);
void tw_init_fields(tw_field_t fields[], int menu_str_link[], int nfields);
int tw_mouse_event(win_t *win, tw_field_t fields[], event_t *event, char *help, int help_line, int nfields, int ch, int *c);
void tw_copy_data(tw_field_t fields[], int nfields, int menu_str_link[]);
void tw_free_temp_fields(tw_field_t fields[], int nfields);
/* winsert_ch.c */
void tw_insertc(win_t *win, int ch);
/* winsert_line.c */
void tw_insert_line(win_t *win);
/* wlist_win.c */
void tw_list_win(win_t *win, int parent_line, int parent_col);
/* wmenu.c */
win_t *tw_menu(term_t *terminal, int row, int col, char *text[], border_t *border, int no_acs, int no_color, int foreground, int background, int hl_foreground, int hl_background);
/* wmouseinwin.c */
int tw_mouse_in_win(win_t *win, win_t *event_win, event_t *event);
/* wmove_down.c */
int tw_move_down(win_t *win);
/* wmove_left.c */
int tw_move_left(win_t *win);
/* wmove_right.c */
int tw_move_right(win_t *win);
/* wmove_to.c */
int tw_move_to(win_t *win, int line, int col);
/* wmove_up.c */
int tw_move_up(win_t *win);
/* wmsg_win.c */
win_t *tw_message_win(term_t *term, char *message[], int fg, int bg, int attr);
/* wout_border.c */
void tw_outer_border(win_t *win);
/* wprint_cen.c */
void tw_print_center(win_t *win, int line, char *format, ...);
/* wprintat.c */
int tw_print_at(win_t *win, int line, int col, char *format, ...);
/* wprintf.c */
int tw_printf(win_t *win, const char *format, ...);
/* wputc.c */
void tw_putc(win_t *win, int ch);
/* wputs.c */
void tw_puts(win_t *win, char *string);
void tw_nputs(win_t *win, char *string, int maxlen);
/* wredraw.c */
void tw_redraw(win_t *win, int topline, int leftcol, int bottomline, int rightcol);
/* wredraw_all.c */
void tw_redraw_all(term_t *terminal);
/* wredraw_line.c */
void tw_redraw_line(win_t *win, int line, int leftcol, int rightcol);
void tw_redraw_line_leave_cursor(win_t *win, int line, int leftcol, int rightcol);
/* wresp_win.c */
int tw_response_win(term_t *term, int line, int col, char *message[], char *buttons[], int extra_flags, border_t *border);
/* wrestore.c */
void tw_restore(win_t *win, int topline, int leftcol, int bottomline, int rightcol);
/* wrestore_lin.c */
void tw_restore_line(win_t *win, int line, int leftcol, int rightcol);
void tw_restore_line_leave_cursor(win_t *win, int line, int leftcol, int rightcol);
/* wscroll_bar.c */
void tw_draw_scroll_bar(win_t *bar_win);
int tw_move_scroll_bar(win_t *bar_win, int numerator, int denominator);
void tw_draw_block(win_t *win);
/* wscroll_for.c */
void tw_scroll_forward(win_t *win);
void ptw_scroll_forward(win_t *win);
/* wscroll_rev.c */
void tw_scroll_reverse(win_t *win);
void ptw_scroll_reverse(win_t *win);
/* wset_bg.c */
int tw_set_background(win_t *win, int color);
/* wset_fg.c */
int tw_set_foreground(win_t *win, int color);
/* wset_wattr.c */
void tw_set_win_attr(win_t *win, int normal_mode, int normal_fg, int normal_bg, int highlight_mode, int highlight_fg, int highlight_bg);
/* wshadow.c */
void tw_shadow(win_t *win);
void tw_checker_shadow(win_t *win);
void tw_blank_shadow(win_t *win);
void tw_dim_shadow(win_t *win);
void tw_match_term_ch(win_t *win, int line, int col);
/* wsync_modes.c */
void tw_sync_modes(win_t *win);
/* wunlist_win.c */
void tw_unlist_win(win_t *win);
int tw_show_list(term_t *terminal);

