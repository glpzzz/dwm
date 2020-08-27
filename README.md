dwm - dynamic window manager
============================

dwm is an extremely fast, small, and dynamic window manager for X.

**This is my personal build.** Every branch on the repo is dwm patched with an one of the patches I find useful.

Master branch is in sync with the original dwm project at
https://git.suckless.org/dwm

Single patched branches
-----------------------

- [awesomebar](https://dwm.suckless.org/patches/awesomebar)
- [focusonclick](https://dwm.suckless.org/patches/focusonclick/)
- [holdbar-modkey](https://dwm.suckless.org/patches/holdbar/)
- [killunsel](https://dwm.suckless.org/patches/killunsel/)
- [noborder](https://dwm.suckless.org/patches/noborder/)
- [pertag](https://dwm.suckless.org/patches/pertag/)

My custom build
---------------

The branch **patched2** is the result of merging the original dwm with the
patched versions. 

Requirements
------------
In order to build dwm you need the Xlib header files.


Build
-----

Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm


Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.
