
SUBLIBS = Image/libimage.a Terminal/libdirect.a Win/libwin.a

INSTALL         ?= install
DESTDIR         ?= .
PREFIX          ?= /usr/local
LOCALBASE       ?= /usr/local
MAKE            ?= make
CP              ?= /bin/cp
MKDIR           ?= /bin/mkdir
LIB             = libtwintk.a

all:    ${LIB}

${LIB}: ${SUBLIBS}
	ar r ${LIB} Image/*.o Terminal/*.o Win/*.o
	ranlib ${LIB}

Image/libimage.a:
	(cd Image && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX} depend && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX})

Terminal/libdirect.a:
	(cd Terminal && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX} depend && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX})

Win/libwin.a:
	(cd Win && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX} depend && ${MAKE} LOCALBASE=${LOCALBASE} PREFIX=${PREFIX})

depend:

install:        all
	${MKDIR} -p ${DESTDIR}${PREFIX}/share/twintk
	${MKDIR} -p ${DESTDIR}${PREFIX}/lib
	${MKDIR} -p ${DESTDIR}${PREFIX}/include
	${INSTALL} ${LIB} ${DESTDIR}${PREFIX}/lib
	${INSTALL} Win/twintk.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Terminal/twintk_term.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Image/twintk_image.h ${DESTDIR}${PREFIX}/include
	${INSTALL} Mouse/twintk_mouse.h ${DESTDIR}${PREFIX}/include
	${CP} -R terminfo ${DESTDIR}${PREFIX}/share/twintk

clean:
	(cd Image; ${MAKE} clean)
	(cd Terminal; ${MAKE} clean)
	(cd Win; ${MAKE} clean)
	rm -f ${LIB}

realclean:
	(cd Image; ${MAKE} realclean)
	(cd Terminal; ${MAKE} realclean)
	(cd Win; ${MAKE} realclean)
	rm -f ${LIB}

