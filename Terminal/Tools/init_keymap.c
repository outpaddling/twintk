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

#include "direct.h"

void    init_keymap(terminal)
term_t   *terminal;

{
    if ( terminal->key_backspace != NULL )
        add_seq(terminal->keymap,terminal->key_backspace,KEY_BACKSPACE,MAX_KEYS);
    if ( terminal->key_catab != NULL )
        add_seq(terminal->keymap,terminal->key_catab,KEY_CATAB,MAX_KEYS);
    if ( terminal->key_clear != NULL )
        add_seq(terminal->keymap,terminal->key_clear,KEY_CLEAR,MAX_KEYS);
    if ( terminal->key_ctab != NULL )
        add_seq(terminal->keymap,terminal->key_ctab,KEY_CTAB,MAX_KEYS);
    if ( terminal->key_dc != NULL )
        add_seq(terminal->keymap,terminal->key_dc,KEY_DC,MAX_KEYS);
    if ( terminal->key_dl != NULL )
        add_seq(terminal->keymap,terminal->key_dl,KEY_DL,MAX_KEYS);
    if ( terminal->key_down != NULL )
        add_seq(terminal->keymap,terminal->key_down,KEY_DOWN,MAX_KEYS);
    if ( terminal->key_eic != NULL )
        add_seq(terminal->keymap,terminal->key_eic,KEY_EIC,MAX_KEYS);
    if ( terminal->key_eol != NULL )
        add_seq(terminal->keymap,terminal->key_eol,KEY_EOL,MAX_KEYS);
    if ( terminal->key_eos != NULL )
        add_seq(terminal->keymap,terminal->key_eos,KEY_EOS,MAX_KEYS);
    if ( terminal->key_f0 != NULL )
        add_seq(terminal->keymap,terminal->key_f0,KEY_F0,MAX_KEYS);
    if ( terminal->key_f1 != NULL )
        add_seq(terminal->keymap,terminal->key_f1,KEY_F1,MAX_KEYS);
    if ( terminal->key_f10 != NULL )
        add_seq(terminal->keymap,terminal->key_f10,KEY_F10,MAX_KEYS);
    if ( terminal->key_f2 != NULL )
        add_seq(terminal->keymap,terminal->key_f2,KEY_F2,MAX_KEYS);
    if ( terminal->key_f3 != NULL )
        add_seq(terminal->keymap,terminal->key_f3,KEY_F3,MAX_KEYS);
    if ( terminal->key_f4 != NULL )
        add_seq(terminal->keymap,terminal->key_f4,KEY_F4,MAX_KEYS);
    if ( terminal->key_f5 != NULL )
        add_seq(terminal->keymap,terminal->key_f5,KEY_F5,MAX_KEYS);
    if ( terminal->key_f6 != NULL )
        add_seq(terminal->keymap,terminal->key_f6,KEY_F6,MAX_KEYS);
    if ( terminal->key_f7 != NULL )
        add_seq(terminal->keymap,terminal->key_f7,KEY_F7,MAX_KEYS);
    if ( terminal->key_f8 != NULL )
        add_seq(terminal->keymap,terminal->key_f8,KEY_F8,MAX_KEYS);
    if ( terminal->key_f9 != NULL )
        add_seq(terminal->keymap,terminal->key_f9,KEY_F9,MAX_KEYS);
    if ( terminal->key_home != NULL )
        add_seq(terminal->keymap,terminal->key_home,KEY_HOME,MAX_KEYS);
    if ( terminal->key_ic != NULL )
        add_seq(terminal->keymap,terminal->key_ic,KEY_IC,MAX_KEYS);
    if ( terminal->key_il != NULL )
        add_seq(terminal->keymap,terminal->key_il,KEY_IL,MAX_KEYS);
    if ( terminal->key_left != NULL )
        add_seq(terminal->keymap,terminal->key_left,KEY_LEFT,MAX_KEYS);
    if ( terminal->key_ll != NULL )
        add_seq(terminal->keymap,terminal->key_ll,KEY_LL,MAX_KEYS);
    if ( terminal->key_npage != NULL )
        add_seq(terminal->keymap,terminal->key_npage,KEY_NPAGE,MAX_KEYS);
    if ( terminal->key_ppage != NULL )
        add_seq(terminal->keymap,terminal->key_ppage,KEY_PPAGE,MAX_KEYS);
    if ( terminal->key_right != NULL )
        add_seq(terminal->keymap,terminal->key_right,KEY_RIGHT,MAX_KEYS);
    if ( terminal->key_sf != NULL )
        add_seq(terminal->keymap,terminal->key_sf,KEY_SF,MAX_KEYS);
    if ( terminal->key_sr != NULL )
        add_seq(terminal->keymap,terminal->key_sr,KEY_SR,MAX_KEYS);
    if ( terminal->key_stab != NULL )
        add_seq(terminal->keymap,terminal->key_stab,KEY_STAB,MAX_KEYS);
    if ( terminal->key_up != NULL )
        add_seq(terminal->keymap,terminal->key_up,KEY_UP,MAX_KEYS);
    if ( terminal->pkey_key != NULL )
        add_seq(terminal->keymap,terminal->pkey_key,PKEY_KEY,MAX_KEYS);
    if ( terminal->pkey_local != NULL )
        add_seq(terminal->keymap,terminal->pkey_local,PKEY_LOCAL,MAX_KEYS);
    if ( terminal->pkey_xmit != NULL )
        add_seq(terminal->keymap,terminal->pkey_xmit,PKEY_XMIT,MAX_KEYS);
    if ( terminal->key_a1 != NULL )
        add_seq(terminal->keymap,terminal->key_a1,KEY_A1,MAX_KEYS);
    if ( terminal->key_a3 != NULL )
        add_seq(terminal->keymap,terminal->key_a3,KEY_A3,MAX_KEYS);
    if ( terminal->key_b2 != NULL )
        add_seq(terminal->keymap,terminal->key_b2,KEY_B2,MAX_KEYS);
    if ( terminal->key_c1 != NULL )
        add_seq(terminal->keymap,terminal->key_c1,KEY_C1,MAX_KEYS);
    if ( terminal->key_c3 != NULL )
        add_seq(terminal->keymap,terminal->key_c3,KEY_C3,MAX_KEYS);
    if ( terminal->key_btab != NULL )
        add_seq(terminal->keymap,terminal->key_btab,KEY_BTAB,MAX_KEYS);
    if ( terminal->key_beg != NULL )
        add_seq(terminal->keymap,terminal->key_beg,KEY_BEG,MAX_KEYS);
    if ( terminal->key_cancel != NULL )
        add_seq(terminal->keymap,terminal->key_cancel,KEY_CANCEL,MAX_KEYS);
    if ( terminal->key_close != NULL )
        add_seq(terminal->keymap,terminal->key_close,KEY_CLOSE,MAX_KEYS);
    if ( terminal->key_command != NULL )
        add_seq(terminal->keymap,terminal->key_command,KEY_COMMAND,MAX_KEYS);
    if ( terminal->key_copy != NULL )
        add_seq(terminal->keymap,terminal->key_copy,KEY_COPY,MAX_KEYS);
    if ( terminal->key_create != NULL )
        add_seq(terminal->keymap,terminal->key_create,KEY_CREATE,MAX_KEYS);
    if ( terminal->key_end != NULL )
        add_seq(terminal->keymap,terminal->key_end,KEY_END,MAX_KEYS);
    if ( terminal->key_enter != NULL )
        add_seq(terminal->keymap,terminal->key_enter,KEY_ENTER,MAX_KEYS);
    if ( terminal->key_exit != NULL )
        add_seq(terminal->keymap,terminal->key_exit,KEY_EXIT,MAX_KEYS);
    if ( terminal->key_find != NULL )
        add_seq(terminal->keymap,terminal->key_find,KEY_FIND,MAX_KEYS);
    if ( terminal->key_help != NULL )
        add_seq(terminal->keymap,terminal->key_help,KEY_HELP,MAX_KEYS);
    if ( terminal->key_mark != NULL )
        add_seq(terminal->keymap,terminal->key_mark,KEY_MARK,MAX_KEYS);
    if ( terminal->key_message != NULL )
        add_seq(terminal->keymap,terminal->key_message,KEY_MESSAGE,MAX_KEYS);
    if ( terminal->key_move != NULL )
        add_seq(terminal->keymap,terminal->key_move,KEY_MOVE,MAX_KEYS);
    if ( terminal->key_next != NULL )
        add_seq(terminal->keymap,terminal->key_next,KEY_NEXT,MAX_KEYS);
    if ( terminal->key_open != NULL )
        add_seq(terminal->keymap,terminal->key_open,KEY_OPEN,MAX_KEYS);
    if ( terminal->key_options != NULL )
        add_seq(terminal->keymap,terminal->key_options,KEY_OPTIONS,MAX_KEYS);
    if ( terminal->key_previous != NULL )
        add_seq(terminal->keymap,terminal->key_previous,KEY_PREVIOUS,MAX_KEYS);
    if ( terminal->key_print != NULL )
        add_seq(terminal->keymap,terminal->key_print,KEY_PRINT,MAX_KEYS);
    if ( terminal->key_redo != NULL )
        add_seq(terminal->keymap,terminal->key_redo,KEY_REDO,MAX_KEYS);
    if ( terminal->key_reference != NULL )
        add_seq(terminal->keymap,terminal->key_reference,KEY_REFERENCE,MAX_KEYS);
    if ( terminal->key_refresh != NULL )
        add_seq(terminal->keymap,terminal->key_refresh,KEY_REFRESH,MAX_KEYS);
    if ( terminal->key_replace != NULL )
        add_seq(terminal->keymap,terminal->key_replace,KEY_REPLACE,MAX_KEYS);
    if ( terminal->key_restart != NULL )
        add_seq(terminal->keymap,terminal->key_restart,KEY_RESTART,MAX_KEYS);
    if ( terminal->key_resume != NULL )
        add_seq(terminal->keymap,terminal->key_resume,KEY_RESUME,MAX_KEYS);
    if ( terminal->key_save != NULL )
        add_seq(terminal->keymap,terminal->key_save,KEY_SAVE,MAX_KEYS);
    if ( terminal->key_suspend != NULL )
        add_seq(terminal->keymap,terminal->key_suspend,KEY_SUSPEND,MAX_KEYS);
    if ( terminal->key_undo != NULL )
        add_seq(terminal->keymap,terminal->key_undo,KEY_UNDO,MAX_KEYS);
    if ( terminal->key_sbeg != NULL )
        add_seq(terminal->keymap,terminal->key_sbeg,KEY_SBEG,MAX_KEYS);
    if ( terminal->key_scancel != NULL )
        add_seq(terminal->keymap,terminal->key_scancel,KEY_SCANCEL,MAX_KEYS);
    if ( terminal->key_scommand != NULL )
        add_seq(terminal->keymap,terminal->key_scommand,KEY_SCOMMAND,MAX_KEYS);
    if ( terminal->key_scopy != NULL )
        add_seq(terminal->keymap,terminal->key_scopy,KEY_SCOPY,MAX_KEYS);
    if ( terminal->key_screate != NULL )
        add_seq(terminal->keymap,terminal->key_screate,KEY_SCREATE,MAX_KEYS);
    if ( terminal->key_sdc != NULL )
        add_seq(terminal->keymap,terminal->key_sdc,KEY_SDC,MAX_KEYS);
    if ( terminal->key_sdl != NULL )
        add_seq(terminal->keymap,terminal->key_sdl,KEY_SDL,MAX_KEYS);
    if ( terminal->key_select != NULL )
        add_seq(terminal->keymap,terminal->key_select,KEY_SELECT,MAX_KEYS);
    if ( terminal->key_send != NULL )
        add_seq(terminal->keymap,terminal->key_send,KEY_SEND,MAX_KEYS);
    if ( terminal->key_seol != NULL )
        add_seq(terminal->keymap,terminal->key_seol,KEY_SEOL,MAX_KEYS);
    if ( terminal->key_sexit != NULL )
        add_seq(terminal->keymap,terminal->key_sexit,KEY_SEXIT,MAX_KEYS);
    if ( terminal->key_sfind != NULL )
        add_seq(terminal->keymap,terminal->key_sfind,KEY_SFIND,MAX_KEYS);
    if ( terminal->key_shelp != NULL )
        add_seq(terminal->keymap,terminal->key_shelp,KEY_SHELP,MAX_KEYS);
    if ( terminal->key_shome != NULL )
        add_seq(terminal->keymap,terminal->key_shome,KEY_SHOME,MAX_KEYS);
    if ( terminal->key_sic != NULL )
        add_seq(terminal->keymap,terminal->key_sic,KEY_SIC,MAX_KEYS);
    if ( terminal->key_sleft != NULL )
        add_seq(terminal->keymap,terminal->key_sleft,KEY_SLEFT,MAX_KEYS);
    if ( terminal->key_smessage != NULL )
        add_seq(terminal->keymap,terminal->key_smessage,KEY_SMESSAGE,MAX_KEYS);
    if ( terminal->key_smove != NULL )
        add_seq(terminal->keymap,terminal->key_smove,KEY_SMOVE,MAX_KEYS);
    if ( terminal->key_snext != NULL )
        add_seq(terminal->keymap,terminal->key_snext,KEY_SNEXT,MAX_KEYS);
    if ( terminal->key_soptions != NULL )
        add_seq(terminal->keymap,terminal->key_soptions,KEY_SOPTIONS,MAX_KEYS);
    if ( terminal->key_sprevious != NULL )
        add_seq(terminal->keymap,terminal->key_sprevious,KEY_SPREVIOUS,MAX_KEYS);
    if ( terminal->key_sprint != NULL )
        add_seq(terminal->keymap,terminal->key_sprint,KEY_SPRINT,MAX_KEYS);
    if ( terminal->key_sredo != NULL )
        add_seq(terminal->keymap,terminal->key_sredo,KEY_SREDO,MAX_KEYS);
    if ( terminal->key_sreplace != NULL )
        add_seq(terminal->keymap,terminal->key_sreplace,KEY_SREPLACE,MAX_KEYS);
    if ( terminal->key_sright != NULL )
        add_seq(terminal->keymap,terminal->key_sright,KEY_SRIGHT,MAX_KEYS);
    if ( terminal->key_srsume != NULL )
        add_seq(terminal->keymap,terminal->key_srsume,KEY_SRSUME,MAX_KEYS);
    if ( terminal->key_ssave != NULL )
        add_seq(terminal->keymap,terminal->key_ssave,KEY_SSAVE,MAX_KEYS);
    if ( terminal->key_ssuspend != NULL )
        add_seq(terminal->keymap,terminal->key_ssuspend,KEY_SSUSPEND,MAX_KEYS);
    if ( terminal->key_sundo != NULL )
        add_seq(terminal->keymap,terminal->key_sundo,KEY_SUNDO,MAX_KEYS);
    if ( terminal->key_f11 != NULL )
        add_seq(terminal->keymap,terminal->key_f11,KEY_F11,MAX_KEYS);
    if ( terminal->key_f12 != NULL )
        add_seq(terminal->keymap,terminal->key_f12,KEY_F12,MAX_KEYS);
    if ( terminal->key_f13 != NULL )
        add_seq(terminal->keymap,terminal->key_f13,KEY_F13,MAX_KEYS);
    if ( terminal->key_f14 != NULL )
        add_seq(terminal->keymap,terminal->key_f14,KEY_F14,MAX_KEYS);
    if ( terminal->key_f15 != NULL )
        add_seq(terminal->keymap,terminal->key_f15,KEY_F15,MAX_KEYS);
    if ( terminal->key_f16 != NULL )
        add_seq(terminal->keymap,terminal->key_f16,KEY_F16,MAX_KEYS);
    if ( terminal->key_f17 != NULL )
        add_seq(terminal->keymap,terminal->key_f17,KEY_F17,MAX_KEYS);
    if ( terminal->key_f18 != NULL )
        add_seq(terminal->keymap,terminal->key_f18,KEY_F18,MAX_KEYS);
    if ( terminal->key_f19 != NULL )
        add_seq(terminal->keymap,terminal->key_f19,KEY_F19,MAX_KEYS);
    if ( terminal->key_f20 != NULL )
        add_seq(terminal->keymap,terminal->key_f20,KEY_F20,MAX_KEYS);
    if ( terminal->key_f21 != NULL )
        add_seq(terminal->keymap,terminal->key_f21,KEY_F21,MAX_KEYS);
    if ( terminal->key_f22 != NULL )
        add_seq(terminal->keymap,terminal->key_f22,KEY_F22,MAX_KEYS);
    if ( terminal->key_f23 != NULL )
        add_seq(terminal->keymap,terminal->key_f23,KEY_F23,MAX_KEYS);
    if ( terminal->key_f24 != NULL )
        add_seq(terminal->keymap,terminal->key_f24,KEY_F24,MAX_KEYS);
    if ( terminal->key_f25 != NULL )
        add_seq(terminal->keymap,terminal->key_f25,KEY_F25,MAX_KEYS);
    if ( terminal->key_f26 != NULL )
        add_seq(terminal->keymap,terminal->key_f26,KEY_F26,MAX_KEYS);
    if ( terminal->key_f27 != NULL )
        add_seq(terminal->keymap,terminal->key_f27,KEY_F27,MAX_KEYS);
    if ( terminal->key_f28 != NULL )
        add_seq(terminal->keymap,terminal->key_f28,KEY_F28,MAX_KEYS);
    if ( terminal->key_f29 != NULL )
        add_seq(terminal->keymap,terminal->key_f29,KEY_F29,MAX_KEYS);
    if ( terminal->key_f30 != NULL )
        add_seq(terminal->keymap,terminal->key_f30,KEY_F30,MAX_KEYS);
    if ( terminal->key_f31 != NULL )
        add_seq(terminal->keymap,terminal->key_f31,KEY_F31,MAX_KEYS);
    if ( terminal->key_f32 != NULL )
        add_seq(terminal->keymap,terminal->key_f32,KEY_F32,MAX_KEYS);
    if ( terminal->key_f33 != NULL )
        add_seq(terminal->keymap,terminal->key_f33,KEY_F33,MAX_KEYS);
    if ( terminal->key_f34 != NULL )
        add_seq(terminal->keymap,terminal->key_f34,KEY_F34,MAX_KEYS);
    if ( terminal->key_f35 != NULL )
        add_seq(terminal->keymap,terminal->key_f35,KEY_F35,MAX_KEYS);
    if ( terminal->key_f36 != NULL )
        add_seq(terminal->keymap,terminal->key_f36,KEY_F36,MAX_KEYS);
    if ( terminal->key_f37 != NULL )
        add_seq(terminal->keymap,terminal->key_f37,KEY_F37,MAX_KEYS);
    if ( terminal->key_f38 != NULL )
        add_seq(terminal->keymap,terminal->key_f38,KEY_F38,MAX_KEYS);
    if ( terminal->key_f39 != NULL )
        add_seq(terminal->keymap,terminal->key_f39,KEY_F39,MAX_KEYS);
    if ( terminal->key_f40 != NULL )
        add_seq(terminal->keymap,terminal->key_f40,KEY_F40,MAX_KEYS);
    if ( terminal->key_f41 != NULL )
        add_seq(terminal->keymap,terminal->key_f41,KEY_F41,MAX_KEYS);
    if ( terminal->key_f42 != NULL )
        add_seq(terminal->keymap,terminal->key_f42,KEY_F42,MAX_KEYS);
    if ( terminal->key_f43 != NULL )
        add_seq(terminal->keymap,terminal->key_f43,KEY_F43,MAX_KEYS);
    if ( terminal->key_f44 != NULL )
        add_seq(terminal->keymap,terminal->key_f44,KEY_F44,MAX_KEYS);
    if ( terminal->key_f45 != NULL )
        add_seq(terminal->keymap,terminal->key_f45,KEY_F45,MAX_KEYS);
    if ( terminal->key_f46 != NULL )
        add_seq(terminal->keymap,terminal->key_f46,KEY_F46,MAX_KEYS);
    if ( terminal->key_f47 != NULL )
        add_seq(terminal->keymap,terminal->key_f47,KEY_F47,MAX_KEYS);
    if ( terminal->key_f48 != NULL )
        add_seq(terminal->keymap,terminal->key_f48,KEY_F48,MAX_KEYS);
    if ( terminal->key_f49 != NULL )
        add_seq(terminal->keymap,terminal->key_f49,KEY_F49,MAX_KEYS);
    if ( terminal->key_f50 != NULL )
        add_seq(terminal->keymap,terminal->key_f50,KEY_F50,MAX_KEYS);
    if ( terminal->key_f51 != NULL )
        add_seq(terminal->keymap,terminal->key_f51,KEY_F51,MAX_KEYS);
    if ( terminal->key_f52 != NULL )
        add_seq(terminal->keymap,terminal->key_f52,KEY_F52,MAX_KEYS);
    if ( terminal->key_f53 != NULL )
        add_seq(terminal->keymap,terminal->key_f53,KEY_F53,MAX_KEYS);
    if ( terminal->key_f54 != NULL )
        add_seq(terminal->keymap,terminal->key_f54,KEY_F54,MAX_KEYS);
    if ( terminal->key_f55 != NULL )
        add_seq(terminal->keymap,terminal->key_f55,KEY_F55,MAX_KEYS);
    if ( terminal->key_f56 != NULL )
        add_seq(terminal->keymap,terminal->key_f56,KEY_F56,MAX_KEYS);
    if ( terminal->key_f57 != NULL )
        add_seq(terminal->keymap,terminal->key_f57,KEY_F57,MAX_KEYS);
    if ( terminal->key_f58 != NULL )
        add_seq(terminal->keymap,terminal->key_f58,KEY_F58,MAX_KEYS);
    if ( terminal->key_f59 != NULL )
        add_seq(terminal->keymap,terminal->key_f59,KEY_F59,MAX_KEYS);
    if ( terminal->key_f60 != NULL )
        add_seq(terminal->keymap,terminal->key_f60,KEY_F60,MAX_KEYS);
    if ( terminal->key_f61 != NULL )
        add_seq(terminal->keymap,terminal->key_f61,KEY_F61,MAX_KEYS);
    if ( terminal->key_f62 != NULL )
        add_seq(terminal->keymap,terminal->key_f62,KEY_F62,MAX_KEYS);
    if ( terminal->key_f63 != NULL )
        add_seq(terminal->keymap,terminal->key_f63,KEY_F63,MAX_KEYS);
}
