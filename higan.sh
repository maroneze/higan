#!/usr/bin/env sh

# Runs higan (with ananke installed in DESTDIR).

DESTDIR=$(grep "^DESTDIR\s*:=" Makefile | sed s'/DESTDIR\s*:=\s*//')

LD_LIBRARY_PATH=$DESTDIR/usr/local/lib out/higan
