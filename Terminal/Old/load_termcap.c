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


 
#include <stdlib.h>
#include "direct.h"

/*********************************************************/
/* Load terminal capabilities directly from /etc/termcap */

int     load_termcap(term_name, terminal)
char   *term_name;
term_t *terminal;

{
    term_t      *terminal;
    char        bp[1024]="", *area;
    static char buff[1024]="";

    switch (tgetent(bp, term_name)) /* Read terminal entry from /etc/termcap */
    {
        case 0:
            fprintf(stderr, "No entry for %s in /etc/termcap!\n", termtype);
            exit(1);
        case 1:
            break;
        case -1:
            fprintf(stderr, "Cannot open /etc/termcap!\n");
            exit(1);
        default:
            fprintf(stderr, "Weird return value from tgetent()!\n");
            exit(1);
    }

    /* Extract relevant capabilities from termcap entry */
    area = buff;

    /* Basic terminal characteristics */
    terminal->lines = tgetnum("li");
    terminal->columns = tgetnum("co");
    terminal->bs_delay = tgetnum("dB");
    terminal->cr_delay = tgetnum("dC");
    terminal->ff_delay = tgetnum("dF");
    terminal->nl_delay = tgetnum("dN");
    terminal->tab_delay = tgetnum("dT");
    terminal->num_other_keys = tgetnum("kn");

    /* Misc numeric caps */
    terminal->blanks_left_by_sose = tgetnum("sg");
    terminal->blanks_left_by_usue = tgetnum("ug");
    terminal->virtual_terminal = tgetnum("vt");

    /* Control keys */
    terminal->up_arrow_key = tgetstr("ku", &area);
    terminal->down_arrow_key = tgetstr("kd", &area);
    terminal->left_arrow_key = tgetstr("kl", &area);
    terminal->right_arrow_key = tgetstr("kr", &area);
    terminal->home_key = tgetstr("kh", &area);

    /* Boolean capabilities (flags) */
    terminal->has_auto_margins = tgetflag("am");
    terminal->uses_ctrl_h = tgetflag("bs");
    terminal->bs_wraps = tgetflag("bw");
    terminal->cursor_addressable = tgetflag("ca");
    terminal->retained_above = tgetflag("da");
    terminal->retained_below = tgetflag("db");
    terminal->standout_on_status = tgetflag("es");
    terminal->gtty_indicates_tabs = tgetflag("gt");
    terminal->hard_copy_terminal = tgetflag("hc");
    terminal->writable_status_line = tgetflag("hs");
    terminal->insert_shows_nulls = tgetflag("in");
    terminal->moves_during_insert = tgetflag("mi");
    terminal->moves_during_standout = tgetflag("ms");
    terminal->no_correct_cr = tgetflag("nc");
    terminal->cannot_scroll = tgetflag("ns");
    terminal->overstrikes = tgetflag("os");
    terminal->has_hardware_tabs = tgetflag("pt");
    terminal->can_underline = tgetflag("ul");
    terminal->beehive = tgetflag("xb");
    terminal->ignores_nl_after_wrap = tgetflag("xn");
    terminal->return_like_ce = tgetflag("xr");
    terminal->no_standout_overwrite = tgetflag("xs");
    terminal->destructive_tabs = tgetflag("xt");

    terminal->backspace = tgetstr("bc",&area);
    terminal->bell = tgetstr("bl",&area);
    terminal->back_tab = tgetstr("bt",&area);
    terminal->command_char = tgetstr("CC",&area);

    terminal->reverse_mode = tgetstr("mr",&area);
    terminal->standout_mode = tgetstr("so",&area);
    terminal->underline_mode = tgetstr("uc",&area);
    terminal->end_open_vis_mode = tgetstr("ve",&area);
    terminal->open_vis_mode = tgetstr("vs",&area);
    terminal->end_alt_chars_mode = tgetstr("ae",&area);
    terminal->alt_chars_mode = tgetstr("as",&area);
    terminal->delete_mode = tgetstr("dm",&area);
    terminal->end_delete_mode = tgetstr("ed",&area);
    terminal->end_insert_mode = tgetstr("ei",&area);
    terminal->insert_mode = tgetstr("im",&area);
    terminal->end_kp_transmit_mode = tgetstr("ke",&area);
    terminal->kp_transmit_mode = tgetstr("ks",&area);
    terminal->blink_mode = tgetstr("mb",&area);
    terminal->bold_mode = tgetstr("md",&area);
    terminal->normal_mode = tgetstr("me",&area);
    terminal->dim_mode = tgetstr("mh",&area);
    terminal->end_underline_mode = tgetstr("ue",&area);
    terminal->end_standout = tgetstr("es",&area);

    /* Terminal control sequences */
    terminal->set_scroll_region = tgetstr("cs",&area);
    terminal->scroll_forward = tgetstr("sf", &area);
    terminal->scroll_reverse = tgetstr("sr", &area);

    terminal->add_blank_line = tgetstr("al",&area);
    terminal->clear_to_bottom = tgetstr("cd",&area);
    terminal->clear_to_eoln = tgetstr("ce",&area);
    terminal->horizontal_motion = tgetstr("ch",&area);
    terminal->clear_screen = tgetstr("cl",&area);
    terminal->cursor_motion = tgetstr("cm",&area);
    terminal->carriage_return = tgetstr("cr",&area);
    terminal->clear_tabs = tgetstr("ct",&area);
    terminal->vertical_motion = tgetstr("cv",&area);
    terminal->delete_char = tgetstr("dc",&area);
    terminal->delete_line = tgetstr("dl",&area);
    terminal->move_down = tgetstr("do",&area);
    terminal->clear_status_line = tgetstr("ds",&area);
    terminal->form_feed = tgetstr("ff",&area);
    terminal->close_status_line = tgetstr("fs",&area);
    terminal->half_line_down = tgetstr("hd",&area);
    terminal->home = tgetstr("ho",&area);
    terminal->half_line_up = tgetstr("hu",&area);
    terminal->insert_char = tgetstr("ic",&area);
    terminal->insert_pad = tgetstr("ip",&area);
    terminal->init_str = tgetstr("is",&area);
    terminal->move_left = tgetstr("le",&area);
    terminal->goto_last_line = tgetstr("ll",&area);
    terminal->mem_lock_above_cursor = tgetstr("ml",&area);
    terminal->mem_unlock = tgetstr("mu",&area);
    terminal->move_right = tgetstr("nd",&area);
    terminal->newline = tgetstr("nl",&area);
    terminal->pad_char = tgetstr("pc",&area);
    terminal->recover_save_cursor = tgetstr("rc",&area);
    terminal->reset_str = tgetstr("rs",&area);
    terminal->save_cursor = tgetstr("sc",&area); /* sc, st */   
    terminal->tab = tgetstr("ta",&area);
    terminal->open_status_line = tgetstr("ts",&area);
    terminal->underline_and_move = tgetstr("uc",&area);
    terminal->move_up = tgetstr("up",&area);
    terminal->visible_bell = tgetstr("vb",&area);

    /* Key input codes */
    terminal->arrow_key_map = tgetstr("ma",&area);
    terminal->key[0] = tgetstr("k0",&area);
    terminal->key[1] = tgetstr("k1",&area);
    terminal->key[2] = tgetstr("k2",&area);
    terminal->key[3] = tgetstr("k3",&area);
    terminal->key[4] = tgetstr("k4",&area);
    terminal->key[5] = tgetstr("k5",&area);
    terminal->key[6] = tgetstr("k6",&area);
    terminal->key[7] = tgetstr("k7",&area);
    terminal->key[8] = tgetstr("k8",&area);
    terminal->key[9] = tgetstr("k9",&area);
    terminal->bs_key = tgetstr("kb",&area);
    terminal->non_function_keys = tgetstr("ko",&area); /* ?? */

    terminal->key_labels[0] = tgetstr("l0",&area);
    terminal->key_labels[1] = tgetstr("l1",&area);
    terminal->key_labels[2] = tgetstr("l2",&area);
    terminal->key_labels[3] = tgetstr("l3",&area);
    terminal->key_labels[4] = tgetstr("l4",&area);
    terminal->key_labels[5] = tgetstr("l5",&area);
    terminal->key_labels[6] = tgetstr("l6",&area);
    terminal->key_labels[7] = tgetstr("l7",&area);
    terminal->key_labels[8] = tgetstr("l8",&area);
    terminal->key_labels[9] = tgetstr("l9",&area);

    /* Misc */
    terminal->hazeltine = tgetstr("hz",&area);
    terminal->similar_term = tgetstr("tc",&area);
    terminal->end_cm_program = tgetstr("te",&area);
    terminal->start_cm_program = tgetstr("ti",&area);
    terminal->erase_overstrikes = tgetstr("eo",&area);

    /* Filenames */
    terminal->file_with_init_str = tgetstr("if",&area);
    terminal->file_with_reset_str = tgetstr("rf",&area);

    return (terminal);
}
