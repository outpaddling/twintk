
SUBLIBS = Image/libimage.a Terminal/libdirect.a Win/libwin.a

INSTALL         ?= install
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
	${MKDIR} -p ${PREFIX}/share/twintk
	${MKDIR} -p ${PREFIX}/lib
	${MKDIR} -p ${PREFIX}/include
	${INSTALL} ${LIB} ${PREFIX}/lib
	${INSTALL} Win/twintk.h ${PREFIX}/include
	${INSTALL} Terminal/twintk_term.h ${PREFIX}/include
	${INSTALL} Image/twintk_image.h ${PREFIX}/include
	${INSTALL} Mouse/twintk_mouse.h ${PREFIX}/include
	${CP} -R terminfo ${PREFIX}/share/twintk

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

