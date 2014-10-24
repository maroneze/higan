Modified version of higan for personal usage
============================================

This code is based on higan v094 
(http://byuu.net/files/higan_v094-source.tar.xz).


Main changes from v094
======================

* Added keyboard-based timing controls (run at 25%/50%/100%/200%)
    via F1/F2/F3/F4.
    - Timing based on sound emulation (does not work if no sound).
    - Only tested with Linux, mainly using ALSA 
        (but should work with PulseAudio).
* Default profile: balanced (to test 200% speed).


Compiling and running
=====================

* (Optional) Change the 'profile' variable in the Makefile, choosing between:
    - accuracy
    - balanced
    - performance
* (Optional) Change the DESTDIR variable in the Makefile.
    - DESTDIR is used to define destination of the 'install' target, 
        which is suffixed by '/usr/local'.
        +   If not specified, DESTDIR equals '/' in Linux.
* (Optional) Run ./configure --sharedpath=<path> to change the sharedpath
    variable. It is used to set up some installation files.
        +   If not specified, sharedpath equals '/usr/share' in Linux.
* Run 'make' to compile all files.
* Run 'make install' (may require sudo).
* Run './higan.sh'.


Troubleshooting
===============

* If having ananke-related errors (i.e. cannot import games):
    - Ensure libananke.so is in a directory searched by ld,
        e.g. by running 'LD_LIBRARY_PATH=/path/to/ananke path/to/higan'.
        The higan.sh script already does this, but changing the value
        of DESTDIR may require re-running 'make install'.
* If file "ipl.rom" not found:
    - Probable cause: higan could not find the profile folders where expected.
    - Ensure that 'make install' succeeded in copying all files.
    - Check sharedpath variable and permissions.
* Weird timing issues at startup:
    - Check that settings.bml in higan's config directory
        (i.e. usually ~/.config/higan in Linux)
        includes a valid 'Multiplier' entry.
    - Note: play speed settings are saved between sessions.
        Reset it to 100% if necessary.
