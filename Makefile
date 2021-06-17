
LIB     = twintk
SLIB    = lib${LIB}.a
DLIB    = lib${LIB}.so.1

OBJ     =   Image/del_image.o Image/iaddc.o Image/ichange_sreg.o \
	    Image/iclear_eol.o Image/iclear_eos.o Image/iclr_image.o \
	    Image/idelete_ch.o Image/idump.o Image/iinsert_ch.o \
	    Image/imove_down.o Image/imove_left.o Image/imove_right.o \
	    Image/imove_to.o Image/imove_up.o Image/inext_line.o \
	    Image/init_image.o Image/iputc.o Image/iputs.o \
	    Image/iscroll_for.o Image/iscroll_rev.o Image/new_image.o \
	    Image/sub_image.o \
	    Terminal/addseq.o Terminal/del_term.o Terminal/getseq.o \
	    Terminal/init_keymap.o Terminal/init_names.o Terminal/init_term.o \
	    Terminal/load_tinfo.o Terminal/raw_print.o Terminal/tadd_kmous.o \
	    Terminal/taddc.o Terminal/taddseq.o Terminal/tcbreak_mode.o \
	    Terminal/tchange_sreg.o Terminal/tclear_eol.o Terminal/tclear_eos.o \
	    Terminal/tclr_screen.o Terminal/tdelete_ch.o Terminal/tdelete_line.o \
	    Terminal/tdump.o Terminal/tget_resp.o Terminal/tgetc.o \
	    Terminal/tgetevent.o Terminal/tgetmouse.o Terminal/tgets.o \
	    Terminal/tgetseq.o Terminal/thome.o Terminal/tinsert_ch.o \
	    Terminal/tinsert_line.o Terminal/tinsert_mode.o Terminal/tmousesrvr.o \
	    Terminal/tmove_down.o Terminal/tmove_left.o Terminal/tmove_right.o \
	    Terminal/tmove_to.o Terminal/tmove_up.o Terminal/tparm.o \
	    Terminal/tpop_tty.o Terminal/tprintf.o Terminal/tpush_event.o \
	    Terminal/tpush_tty.o Terminal/tputc.o Terminal/tputs.o \
	    Terminal/traw_mode.o Terminal/tredraw_line.o Terminal/tredraw.o \
	    Terminal/tscroll_for.o Terminal/tscroll_rev.o Terminal/tset_bg.o \
	    Terminal/tset_dim.o Terminal/tset_fg.o Terminal/tset_modes.o \
	    Terminal/tset_start.o Terminal/tset_tty.o Terminal/tunset_tty.o \
	    Win/del_win.o Win/init_double.o Win/init_enum.o Win/init_int.o \
	    Win/init_ldouble.o Win/init_llong.o Win/init_long.o \
	    Win/init_menu.o Win/init_string.o Win/new_win.o Win/panel_check.o \
	    Win/print_cur.o Win/sub_win.o Win/waddstr.o Win/wblank.o \
	    Win/wbut_resp.o Win/wclear_eol.o Win/wclear_eow.o Win/wclear.o \
	    Win/wdelete_ch.o Win/wdelete_line.o Win/wdump.o Win/wget_item.o \
	    Win/wget_path.o Win/wget_resp.o Win/wgetc.o Win/wgetenum.o \
	    Win/wgetevent.o Win/wgetfloat.o Win/wgetint.o Win/wgetmenu.o \
	    Win/wgetseq.o Win/wgetstring.o Win/whigh_print.o Win/whl_menu.o \
	    Win/whome.o Win/winn_border.o Win/winput_pan.o Win/winsert_ch.o \
	    Win/winsert_line.o Win/wlist_win.o Win/wmenu.o Win/wmouseinwin.o \
	    Win/wmove_down.o Win/wmove_left.o Win/wmove_right.o Win/wmove_to.o \
	    Win/wmove_up.o Win/wmsg_win.o Win/wout_border.o Win/wprint_cen.o \
	    Win/wprintat.o Win/wprintf.o Win/wputc.o Win/wputs.o \
	    Win/wredraw_all.o Win/wredraw_line.o Win/wredraw.o \
	    Win/wresp_win.o Win/wrestore_lin.o Win/wrestore.o \
	    Win/wscroll_bar.o Win/wscroll_for.o Win/wscroll_rev.o \
	    Win/wset_bg.o Win/wset_fg.o Win/wset_wattr.o Win/wshadow.o \
	    Win/wsync_modes.o Win/wunlist_win.o

LOCALBASE   ?= ../local
PREFIX      ?= ${LOCALBASE}

# Allow caller to override either MANPREFIX or MANDIR
MANPREFIX   ?= ${PREFIX}
MANDIR      ?= ${MANPREFIX}/man

INSTALL     ?= install
RM          ?= /bin/rm
CP          ?= /bin/cp
MKDIR       ?= /bin/mkdir
PRINTF      ?= printf

CFLAGS      ?= -Wall -O -g
INCLUDES    += -IImage -ITerminal -IWin -IMouse -I${LOCALBASE}/include
CFLAGS      += ${INCLUDES} -DPREFIX="\"${PREFIX}\""

all:    ${SLIB}

${SLIB}: ${OBJ}
	ar r ${SLIB} ${OBJ}
	ranlib ${SLIB}

include Makefile.depend

depend:
	rm -f Makefile.depend
	for file in Image/*.c Terminal/*.c Win/*.c; do \
	    ${CPP} ${INCLUDES} -MM $${file} >> Makefile.depend; \
	    ${PRINTF} "\t\$${CC} -c \$${CFLAGS} $${file} -o $${file%.c}.o\n\n" >> Makefile.depend; \
	done

install:        all
	${MKDIR} -p ${DESTDIR}${PREFIX}/share/twintk
	${MKDIR} -p ${DESTDIR}${PREFIX}/lib
	${MKDIR} -p ${DESTDIR}${PREFIX}/include
	${INSTALL} ${SLIB} ${DESTDIR}${PREFIX}/lib
	${INSTALL} Win/twintk.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Terminal/twintk_term.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Image/twintk_image.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Mouse/twintk_mouse.h ${DESTDIR}${PREFIX}/include
	${RM} -rf ${DESTDIR}${PREFIX}/share/twintk/terminfo
	${CP} -R terminfo ${DESTDIR}${PREFIX}/share/twintk

clean:
	rm -f ${OBJ} ${SLIB} ${DLIB}

realclean: clean
	rm -f .*.bak
