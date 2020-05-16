twintk
======

Twintk, the Terminal Windows Toolkit, is a library for developing intuitive
terminal-based applications utilizing menus, color, dialogs, etc.

It utilizes 3 separate layers:

* Image: A buffer containing the current window or terminal content
* Terminal: A structure representing a terminal screen, including its image
* Window: A structire representing a window, including its image

A window or terminal can be updated immediately via functions like wputc()
or a curses-like strategy can be used by updating the image only and then
redrawing the window or terminal.  The developer can choose which approach
is best for each situation.

Twintk was developed in the early 1990s in support of APE (Another
Programmer's Editor) due to limitations encountered with curses at the time.
