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

* Added record/replay buttons (currently only tested with SFC):
    - R toggles recording of the entire input sequence. This erases
      any previous recording.
    - Y starts replaying the recorded input sequence, at the same absolute
      time, that is, after resetting the game and pressing Y (or choosing the
      menu option "Reset and replay"), the game will replay according to the
      recorded sequence. No other input can be entered during replay.
    - M toggles recording of a "macro", a sequence of inputs with relative
      timing (instead of absolute).
    - K replays a recorded macro.
    - Note: the record/replay feature is still unstable and not very 
            user-friendly. In particular, any new recording of an input 
            sequence erases previous recordings. However, it is compatible
            with save/load states.
            Its intended usage is the creation of tool-assisted runs, with
            all input being recorded from the start of the game. Saving a
            game during recording should maintain all input history, and
            loading will erase all input history from after the saved state.
            This way, one can slow/accelerate time and use save states when
            recording a long game sequence, and then once it is done, 
            the game can be reset and auto-replayed in real-time for 
            video recording.
            Note that the modifications performed to enable input recording
            may have a noticeable impact on performance (lower a few FPS), 
            especially in accuracy mode.

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
    - Check the PWD when running the program (e.g. avoid running it directly
      from the 'out' directory).
* Weird timing issues at startup:
    - Check that settings.bml in higan's config directory
        (i.e. usually ~/.config/higan in Linux)
        includes a valid 'Multiplier' entry.
    - Note: play speed settings are saved between sessions.
        Reset it to 100% if necessary.
