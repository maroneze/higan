#!/usr/bin/env bash

SCR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCR_DIR

# Runs higan (with ananke installed in DESTDIR).

DESTDIR=$(grep "^DESTDIR\s*:=" Makefile | sed s'/DESTDIR\s*:=\s*//')

LD_LIBRARY_PATH=$DESTDIR/usr/local/lib out/higan
