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
#include "twintk_term.h"

/*
 * Set up key map from terminfo database.
 * Place desired function first in case of collisions between sequences
 * so that the preferred function overrides the other(s).
 * e.g. down arrow key sends same code as enter on some terminals,
 * so put return earlier in code below.
 */
 
void    init_keymap(terminal)
term_t   *terminal;

{
    /* Add Terminfo-defined keys or common defaults for things
       like BS and DEL if not defined */
    
    if ( terminal->key_enter != NULL )
	taddseq(terminal,terminal->key_enter,KEY_ENTER);
    
    /* Return and Enter keys differ on some terminals (e.g. vt220), so
     * add \r in any case.  Also override \r as key_refresh for vtxxx
     * below (DEC Unix bug).  Check for backspace collision first (dg200) */
    if ( ((terminal->key_enter == NULL) || (strcmp(terminal->key_enter,"\r") != 0)) &&
	 ((terminal->key_backspace!=NULL) && (strcmp(terminal->key_backspace,"\r") != 0)) )
	taddseq(terminal,"\r",KEY_ENTER);
    
    /* Arrow keys - these come first so they override colliding keys */
    if ( terminal->key_left != NULL )
	taddseq(terminal,terminal->key_left,KEY_LEFT);
    if ( terminal->key_right != NULL )
	taddseq(terminal,terminal->key_right,KEY_RIGHT);
    if ( terminal->key_down != NULL )
	taddseq(terminal,terminal->key_down,KEY_DOWN);
    if ( terminal->key_up != NULL )
	taddseq(terminal,terminal->key_up,KEY_UP);
    
    /* Motion keys.  Put here before BS for dg200 HOME=\b */
    if ( terminal->key_home != NULL )
	taddseq(terminal,terminal->key_home,KEY_HOME);
    if ( terminal->key_npage != NULL )
	taddseq(terminal,terminal->key_npage,KEY_NPAGE);
    if ( terminal->key_ppage != NULL )
	taddseq(terminal,terminal->key_ppage,KEY_PPAGE);
    if ( terminal->key_end != NULL )
	taddseq(terminal,terminal->key_end,KEY_END);
    
    /* Should come after down arrow so arrows works on collision */
    /* e.g. tvi and adm series use \b for both backspace and down arrow */
    if ( terminal->key_backspace != NULL )
    {
	taddseq(terminal,terminal->key_backspace,KEY_BACKSPACE);
	if ( *terminal->key_backspace != '\b' )
	    taddseq(terminal,"\b",KEY_BACKSPACE);
    }
    else
	taddseq(terminal,"\b",KEY_BACKSPACE);

    /* Delete key */
    if ( terminal->key_dc != NULL )
    {
	/* fprintf(stderr,"Adding %d as key_dc...\n",*terminal->key_dc); */
	taddseq(terminal,terminal->key_dc,KEY_DC);
    }
    
    /* Mouse */
    if ( terminal->key_mouse != NULL )
	taddseq(terminal,terminal->key_mouse,KEY_MOUSE);

    /* Function keys */
    if ( terminal->key_f0 != NULL )
	taddseq(terminal,terminal->key_f0,KEY_F0);
    if ( terminal->key_f1 != NULL )
	taddseq(terminal,terminal->key_f1,KEY_F1);
    if ( terminal->key_f2 != NULL )
	taddseq(terminal,terminal->key_f2,KEY_F2);
    if ( terminal->key_f3 != NULL )
	taddseq(terminal,terminal->key_f3,KEY_F3);
    if ( terminal->key_f4 != NULL )
	taddseq(terminal,terminal->key_f4,KEY_F4);
    if ( terminal->key_f5 != NULL )
	taddseq(terminal,terminal->key_f5,KEY_F5);
    if ( terminal->key_f6 != NULL )
	taddseq(terminal,terminal->key_f6,KEY_F6);
    if ( terminal->key_f7 != NULL )
	taddseq(terminal,terminal->key_f7,KEY_F7);
    if ( terminal->key_f8 != NULL )
	taddseq(terminal,terminal->key_f8,KEY_F8);
    if ( terminal->key_f9 != NULL )
	taddseq(terminal,terminal->key_f9,KEY_F9);
    if ( terminal->key_f10 != NULL )
	taddseq(terminal,terminal->key_f10,KEY_F10);
    if ( terminal->key_f11 != NULL )
	taddseq(terminal,terminal->key_f11,KEY_F11);
    if ( terminal->key_f12 != NULL )
	taddseq(terminal,terminal->key_f12,KEY_F12);

    /* Insert character/insert mode */
    if ( terminal->key_ic != NULL )
	taddseq(terminal,terminal->key_ic,KEY_IC);

    /* Delete line */
    if ( terminal->key_dl != NULL )
	taddseq(terminal,terminal->key_dl,KEY_DL);

    /* Clear to end of line */
    if ( terminal->key_eol != NULL )
	taddseq(terminal,terminal->key_eol,KEY_EOL);
    
    /* Insert line */
    if ( terminal->key_il != NULL )
	taddseq(terminal,terminal->key_il,KEY_IL);

    if ( terminal->key_help != NULL )
	taddseq(terminal,terminal->key_help,KEY_HELP);
    if ( terminal->key_undo != NULL )
	taddseq(terminal,terminal->key_undo,KEY_UNDO);
    if ( terminal->key_btab != NULL )
	taddseq(terminal,terminal->key_btab,KEY_BTAB);
    if ( terminal->key_find != NULL )
	taddseq(terminal,terminal->key_find,KEY_FIND);
    if ( terminal->key_mark != NULL )
	taddseq(terminal,terminal->key_mark,KEY_MARK);
    if ( terminal->key_copy != NULL )
	taddseq(terminal,terminal->key_copy,KEY_COPY);

    /* Workaround DEC Unix bug with \r for refresh key */
    if (terminal->key_refresh != NULL)
	taddseq(terminal,terminal->key_refresh,KEY_REFRESH);
    
    if ( terminal->key_replace != NULL )
	taddseq(terminal,terminal->key_replace,KEY_REPLACE);
    if ( terminal->key_save != NULL )
	taddseq(terminal,terminal->key_save,KEY_SAVE);

#if 0
    /* Stuff that doesn't appear to be useful at the moment */
    /* Includes key_f13 and beyond */
    if ( terminal->key_catab != NULL )
	taddseq(terminal,terminal->key_catab,KEY_CATAB);
    if ( terminal->key_clear != NULL )
	taddseq(terminal,terminal->key_clear,KEY_CLEAR);
    if ( terminal->key_ctab != NULL )
	taddseq(terminal,terminal->key_ctab,KEY_CTAB);
    if ( terminal->key_eic != NULL )
	taddseq(terminal,terminal->key_eic,KEY_EIC);
    if ( terminal->key_eos != NULL )
	taddseq(terminal,terminal->key_eos,KEY_EOS);
    if ( terminal->key_ll != NULL )
	taddseq(terminal,terminal->key_ll,KEY_LL);
    if ( terminal->key_sf != NULL )
	taddseq(terminal,terminal->key_sf,KEY_SF);
    if ( terminal->key_sr != NULL )
	taddseq(terminal,terminal->key_sr,KEY_SR);
    if ( terminal->key_stab != NULL )
	taddseq(terminal,terminal->key_stab,KEY_STAB);
    if ( terminal->pkey_key != NULL )
	taddseq(terminal,terminal->pkey_key,PKEY_KEY);
    if ( terminal->pkey_local != NULL )
	taddseq(terminal,terminal->pkey_local,PKEY_LOCAL);
    if ( terminal->pkey_xmit != NULL )
	taddseq(terminal,terminal->pkey_xmit,PKEY_XMIT);
    if ( terminal->key_a1 != NULL )
	taddseq(terminal,terminal->key_a1,KEY_A1);
    if ( terminal->key_a3 != NULL )
	taddseq(terminal,terminal->key_a3,KEY_A3);
    if ( terminal->key_b2 != NULL )
	taddseq(terminal,terminal->key_b2,KEY_B2);
    if ( terminal->key_c1 != NULL )
	taddseq(terminal,terminal->key_c1,KEY_C1);
    if ( terminal->key_c3 != NULL )
	taddseq(terminal,terminal->key_c3,KEY_C3);
    if ( terminal->key_beg != NULL )
	taddseq(terminal,terminal->key_beg,KEY_BEG);
    if ( terminal->key_cancel != NULL )
	taddseq(terminal,terminal->key_cancel,KEY_CANCEL);
    if ( terminal->key_close != NULL )
	taddseq(terminal,terminal->key_close,KEY_CLOSE);
    if ( terminal->key_command != NULL )
	taddseq(terminal,terminal->key_command,KEY_COMMAND);
    if ( terminal->key_create != NULL )
	taddseq(terminal,terminal->key_create,KEY_CREATE);
    if ( terminal->key_exit != NULL )
	taddseq(terminal,terminal->key_exit,KEY_EXIT);
    if ( terminal->key_message != NULL )
	taddseq(terminal,terminal->key_message,KEY_MESSAGE);
    if ( terminal->key_move != NULL )
	taddseq(terminal,terminal->key_move,KEY_MOVE);
    if ( terminal->key_next != NULL )
	taddseq(terminal,terminal->key_next,KEY_NEXT);
    if ( terminal->key_open != NULL )
	taddseq(terminal,terminal->key_open,KEY_OPEN);
    if ( terminal->key_options != NULL )
	taddseq(terminal,terminal->key_options,KEY_OPTIONS);
    if ( terminal->key_previous != NULL )
	taddseq(terminal,terminal->key_previous,KEY_PREVIOUS);
    if ( terminal->key_print != NULL )
	taddseq(terminal,terminal->key_print,KEY_PRINT);
    if ( terminal->key_redo != NULL )
	taddseq(terminal,terminal->key_redo,KEY_REDO);
    if ( terminal->key_reference != NULL )
	taddseq(terminal,terminal->key_reference,KEY_REFERENCE);
    if ( terminal->key_restart != NULL )
	taddseq(terminal,terminal->key_restart,KEY_RESTART);
    if ( terminal->key_resume != NULL )
	taddseq(terminal,terminal->key_resume,KEY_RESUME);
    if ( terminal->key_suspend != NULL )
	taddseq(terminal,terminal->key_suspend,KEY_SUSPEND);
    if ( terminal->key_sbeg != NULL )
	taddseq(terminal,terminal->key_sbeg,KEY_SBEG);
    if ( terminal->key_scancel != NULL )
	taddseq(terminal,terminal->key_scancel,KEY_SCANCEL);
    if ( terminal->key_scommand != NULL )
	taddseq(terminal,terminal->key_scommand,KEY_SCOMMAND);
    if ( terminal->key_scopy != NULL )
	taddseq(terminal,terminal->key_scopy,KEY_SCOPY);
    if ( terminal->key_screate != NULL )
	taddseq(terminal,terminal->key_screate,KEY_SCREATE);
    if ( terminal->key_sdc != NULL )
	taddseq(terminal,terminal->key_sdc,KEY_SDC);
    if ( terminal->key_sdl != NULL )
	taddseq(terminal,terminal->key_sdl,KEY_SDL);
    if ( terminal->key_select != NULL )
	taddseq(terminal,terminal->key_select,KEY_SELECT);
    if ( terminal->key_send != NULL )
	taddseq(terminal,terminal->key_send,KEY_SEND);
    if ( terminal->key_seol != NULL )
	taddseq(terminal,terminal->key_seol,KEY_SEOL);
    if ( terminal->key_sexit != NULL )
	taddseq(terminal,terminal->key_sexit,KEY_SEXIT);
    if ( terminal->key_sfind != NULL )
	taddseq(terminal,terminal->key_sfind,KEY_SFIND);
    if ( terminal->key_shelp != NULL )
	taddseq(terminal,terminal->key_shelp,KEY_SHELP);
    if ( terminal->key_shome != NULL )
	taddseq(terminal,terminal->key_shome,KEY_SHOME);
    if ( terminal->key_sic != NULL )
	taddseq(terminal,terminal->key_sic,KEY_SIC);
    if ( terminal->key_sleft != NULL )
	taddseq(terminal,terminal->key_sleft,KEY_SLEFT);
    if ( terminal->key_smessage != NULL )
	taddseq(terminal,terminal->key_smessage,KEY_SMESSAGE);
    if ( terminal->key_smove != NULL )
	taddseq(terminal,terminal->key_smove,KEY_SMOVE);
    if ( terminal->key_snext != NULL )
	taddseq(terminal,terminal->key_snext,KEY_SNEXT);
    if ( terminal->key_soptions != NULL )
	taddseq(terminal,terminal->key_soptions,KEY_SOPTIONS);
    if ( terminal->key_sprevious != NULL )
	taddseq(terminal,terminal->key_sprevious,KEY_SPREVIOUS);
    if ( terminal->key_sprint != NULL )
	taddseq(terminal,terminal->key_sprint,KEY_SPRINT);
    if ( terminal->key_sredo != NULL )
	taddseq(terminal,terminal->key_sredo,KEY_SREDO);
    if ( terminal->key_sreplace != NULL )
	taddseq(terminal,terminal->key_sreplace,KEY_SREPLACE);
    if ( terminal->key_sright != NULL )
	taddseq(terminal,terminal->key_sright,KEY_SRIGHT);
    if ( terminal->key_srsume != NULL )
	taddseq(terminal,terminal->key_srsume,KEY_SRSUME);
    if ( terminal->key_ssave != NULL )
	taddseq(terminal,terminal->key_ssave,KEY_SSAVE);
    if ( terminal->key_ssuspend != NULL )
	taddseq(terminal,terminal->key_ssuspend,KEY_SSUSPEND);
    if ( terminal->key_sundo != NULL )
	taddseq(terminal,terminal->key_sundo,KEY_SUNDO);
#endif

    /* Fixes */
    if ( (memcmp(terminal->term_type,"vt",2) == 0) ||
	 (strstr(terminal->term_type,"xterm") != NULL) ||
	 (strstr(terminal->term_type,"rxvt") != NULL) ||
	 (memcmp(terminal->term_type,"gnome", 5) == 0) ||
	 (memcmp(terminal->term_type,"xfce",4) == 0) ||
	 (memcmp(terminal->term_type,"ansi",4) == 0) ||
	 /*
	  * xfce4 Terminal has wrong magic sequences for arrow keys
	  * Database says, e.g. \EOC, Terminal sends \E-[C
	  */
	 (strcmp(terminal->term_type,"Terminal") == 0) )
	vtxxx_keys(terminal);
}


/***************************************************************************
 * Description:
 *  The DEC vtXXX based descriptions are hopelessly hosed. 
 *  There's a wide variety of xterm-* terminal emulators that
 *  send vastly different key sequences for Del, Backspace, F*, etc.
 *  Choosing the right entry from among xterm, xtermc, xterm-color, etc.
 *  is next to impossible, since it can't be known whether the
 *  user is actually using an xterm, rxvt, konsole, gnome-terminal,
 *  yada yada.  This function masks some of the differences by
 *  enabling common vtxxx/xterm* key sequences if they're not in
 *  the chosen entry.  E.g., if the terminfo entry says F1 is \033OP
 *  and the user's terminal emulator actually sends \033[11~, the
 *  twintk application will work since both are mapped to KEY_F1
 *  after running vtxxx_keys().
 ***************************************************************************/

void    vtxxx_keys(term)
term_t  *term;

{
    /* If DEL is not defined as the delete char, use it for backspace.
       (vtXXX standard) */
    /* The DEL key should be defined as an escape sequence. */
    if ( (term->key_dc == NULL) || (strcmp(term->key_dc,"\177") != 0) )
	taddseq(term,"\177",KEY_BACKSPACE);
    
    /* Alternative arrow key sequences */
    if ( (term->key_up == NULL) || strcmp(term->key_up,"\033[A"))
	taddseq(term, "\033[A", KEY_UP);
    if ( (term->key_down == NULL) || strcmp(term->key_down,"\033[B"))
	taddseq(term, "\033[B", KEY_DOWN);
    if ( (term->key_right == NULL) || strcmp(term->key_right,"\033[C"))
	taddseq(term, "\033[C", KEY_RIGHT);
    if ( (term->key_left == NULL) || strcmp(term->key_left,"\033[D"))
	taddseq(term, "\033[D", KEY_LEFT);
    
    if ( (term->key_up == NULL) || strcmp(term->key_up,"\033OA"))
	taddseq(term, "\033OA", KEY_UP);
    if ( (term->key_down == NULL) || strcmp(term->key_down,"\033OB"))
	taddseq(term, "\033OB", KEY_DOWN);
    if ( (term->key_right == NULL) || strcmp(term->key_right,"\033OC"))
	taddseq(term, "\033OC", KEY_RIGHT);
    if ( (term->key_left == NULL) || strcmp(term->key_left,"\033OD"))
	taddseq(term, "\033OD", KEY_LEFT);

    if ( (term->key_ppage == NULL) || strcmp(term->key_ppage,"\033[I"))
	taddseq(term, "\033[I",KEY_PPAGE);
    if ( (term->key_npage == NULL) || strcmp(term->key_npage,"\033[G"))
	taddseq(term, "\033[G",KEY_NPAGE);
    if ( (term->key_ppage == NULL) || strcmp(term->key_ppage,"\033[5~"))
	taddseq(term, "\033[5~",KEY_PPAGE);
    if ( (term->key_npage == NULL) || strcmp(term->key_npage,"\033[6~"))
	taddseq(term, "\033[6~",KEY_NPAGE);

    if ( (term->key_end == NULL) || strcmp(term->key_end,"\033[F"))
	taddseq(term, "\033[F",KEY_END);
    if ( (term->key_home == NULL) || strcmp(term->key_home,"\033[H"))
	taddseq(term, "\033[H",KEY_HOME);
    if ( (term->key_end == NULL) || strcmp(term->key_end,"\033[K"))
	taddseq(term, "\033[K",KEY_END);
    if ( (term->key_home == NULL) || strcmp(term->key_home,"\033[1~"))
	taddseq(term, "\033[1~",KEY_HOME);
    if ( (term->key_end == NULL) || strcmp(term->key_end,"\033[4~"))
	taddseq(term, "\033[4~",KEY_END);
    if ( (term->key_dc == NULL) || strcmp(term->key_dc,"\033[3~"))
	taddseq(term, "\033[3~",KEY_DC);

    if ( (term->key_f1 == NULL) || strcmp(term->key_f1,"\033OP"))
	taddseq(term, "\033OP",KEY_F1);
    if ( (term->key_f2 == NULL) || strcmp(term->key_f2,"\033OQ"))
	taddseq(term, "\033OQ",KEY_F2);
    if ( (term->key_f3 == NULL) || strcmp(term->key_f3,"\033OR"))
	taddseq(term, "\033OR",KEY_F3);
    if ( (term->key_f4 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OS",KEY_F4);
    if ( (term->key_f5 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OT",KEY_F5);
    if ( (term->key_f6 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OU",KEY_F6);
    if ( (term->key_f7 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OV",KEY_F7);
    if ( (term->key_f8 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OW",KEY_F8);
    if ( (term->key_f9 == NULL) || strcmp(term->key_f4,"\033OS"))
	taddseq(term, "\033OX",KEY_F9);

    /* FreeBSD xterm fix */
    if ( (term->key_home == NULL) || strcmp(term->key_home,"\033OH"))
	taddseq(term, "\033OH", KEY_HOME);
    if ( (term->key_end == NULL) || strcmp(term->key_end,"\033OF"))
	taddseq(term, "\033OF", KEY_END); /* ctwm */

    /* Coherent and MIPS xterm */
    if ( (term->key_end == NULL) || strcmp(term->key_end,"\033[\310"))
	taddseq(term, "\033[\310", KEY_END);
    if ( (term->key_ic == NULL) || strcmp(term->key_ic,"\033[2~"))
	taddseq(term, "\033[2~", KEY_IC);
    if ( (term->key_f1 == NULL) || strcmp(term->key_f1,"\033[11~"))
	taddseq(term, "\033[11~", KEY_F1);
    if ( (term->key_f2 == NULL) || strcmp(term->key_f2,"\033[12~"))
	taddseq(term, "\033[12~", KEY_F2);
    if ( (term->key_f3 == NULL) || strcmp(term->key_f3,"\033[13~"))
	taddseq(term, "\033[13~", KEY_F3);
    if ( (term->key_f4== NULL) || strcmp(term->key_f4,"\033[14~"))
	taddseq(term, "\033[14~", KEY_F4);
    if ( (term->key_f5 == NULL) || strcmp(term->key_f5,"\033[15~"))
	taddseq(term, "\033[15~", KEY_F5);
    if ( (term->key_f6 == NULL) || strcmp(term->key_f6,"\033[17~"))
	taddseq(term, "\033[17~", KEY_F6);
    if ( (term->key_f7 == NULL) || strcmp(term->key_f7,"\033[18~"))
	taddseq(term, "\033[18~", KEY_F7);
    if ( (term->key_f8 == NULL) || strcmp(term->key_f8,"\033[19~"))
	taddseq(term, "\033[19~", KEY_F8);
    if ( (term->key_f9 == NULL) || strcmp(term->key_f9,"\033[20~"))
	taddseq(term, "\033[20~", KEY_F9);
    if ( (term->key_f10 == NULL) || strcmp(term->key_f10,"\033[21~"))
	taddseq(term, "\033[21~", KEY_F10);
    if ( (term->key_f11 == NULL) || strcmp(term->key_f11,"\033[23~"))
	taddseq(term, "\033[23~", KEY_F11);
    if ( (term->key_f12 == NULL) || strcmp(term->key_f12,"\033[24~"))
	taddseq(term, "\033[24~", KEY_F12);
}

