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

# Default to ../local if PREFIX is not set
: ${PREFIX:=../local}

PREFIX=$(realpath $PREFIX)
export PREFIX

# OS-dependent tricks
# Set rpath to avoid picking up libs installed by package managers in
# /usr/local/lib, etc.
case $(uname) in
*)
    if [ -z "$CFLAGS" ]; then
	export CFLAGS="-Wall -g -O"
    fi
    LIBDIR=$(realpath $PREFIX/lib)
    export LDFLAGS="-L. -L$LIBDIR -Wl,-rpath,$LIBDIR:/usr/lib:/lib"
    ;;

esac

# OpenBSD and NetBSD make won't install objs into subdirectory
if [ $(uname) = OpenBSD ] || [ $(uname) = NetBSD ] || [ $(uname) = DragonFly ]; then
    make=gmake
else
    make=make
fi

$make clean install
