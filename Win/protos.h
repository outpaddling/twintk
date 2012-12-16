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
int tw_printf(win_t *win, char *format, ...);
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
