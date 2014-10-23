Modified version of higan for personal usage
============================================

This code is based on higan v094 
(http://byuu.net/files/higan_v094-source.tar.xz).


Compiling and running
=====================

* (Optional) Change the 'profile' variable in the Makefile, choosing between:
    - accuracy
    - balanced
    - performance
* (Optional) Change the DESTDIR and sharedpath variables in the Makefile.
    - DESTDIR is used to define destination of the 'install' target, 
        which is suffixed by '/usr/local'.
        +   If not specified, DESTDIR equals '/' in Linux.
    - sharedpath is used to set up some installation files.
        +   If not specified, sharedpath equals '/usr/share' in Linux.
* Run 'make' to compile all files.
* Run 'make install' (may require sudo).
* Run 'higan' binary.


Troubleshooting
===============

* If havingh ananke-related errors:
    - Cd to 'ananke' and run 'make'.
    - Ensure libananke.so is in a directory searched by ld,
        e.g. by running 'LD_LIBRARY_PATH=/path/to/ananke <DESTDIR>/higan'
* If file "ipl.rom" not found:
    - Ensure that 'make install' succeeded in copying all files.
    - Check sharedpath variable and permissions.
        Note: if the value of sharedpath has changed *after* compiling the
        sources, it is necessary to do 'make clean' and then 'make' again.
