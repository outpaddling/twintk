#!/bin/sh -e

##########################################################################
#
#   Perform a cave-man install for development and testing purposes.
#   For production use, this software should be installed via a package
#   manager such as Debian packages, FreeBSD ports, MacPorts, pkgsrc, etc.
#       
#   History:
#   Date        Name        Modification
#   2021-07-12  Jason Bacon Begin
##########################################################################

# Default to ../local if LOCALBASE is not set
: ${LOCALBASE:=../local}

# OS-dependent tricks
# Set rpath to avoid picking up libs installed by package managers in
# /usr/local/lib, etc.
case $(uname) in
*)
    if [ -z "$CFLAGS" ]; then
	export CFLAGS="-Wall -g -O"
    fi
    LIBDIR=$(realpath $LOCALBASE/lib)
    export LDFLAGS="-L. -L$LIBDIR -Wl,-rpath,$LIBDIR:/usr/lib:/lib"
    ;;

esac

# NetBSD make won't install objs into subdirectory
if [ $(uname) = NetBSD ]; then
    make=gmake
else
    make=make
fi

$make clean install
