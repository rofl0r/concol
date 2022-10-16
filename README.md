concol - a library for colourful terminal apps
==============================================

most modern terminal emulators feature plenty of colors, between 64 - 256.
the major hindrance to its usage is that the (n)curses library makes it very
difficult to use them. it sees colors as colorpairs which have to be set up
one after the other, resulting in a ton of code and work to do so.

concol offers a simple API that allows the programmer to just throw an RGB
value at it (either for background or foreground color) and use that at once.
the tedious colorpair manipulation is all done automatically by the library.

as a bonus, there are 3 different backends:

- (n)curses backend (works as well with netbsd curses)
- SDL backend
- termbox backend (8 colors only)

the SDL backend is perfect for developing the app as it allows to debug the
application without interference between the terminal modes used by GDB and
the app itself, which is usually a major pain when developing ncurses apps.

the termbox backend otoh is good to make tiny static linked programs when
8 colors are sufficient, since it is much smaller than (n)curses.
(note that termbox development diverged a lot from its original goals after
commit 66c3f91b. thus it is advised to use the parent of this commit, or use
the fork cursebox ( https://github.com/strake/cursebox.c ) (untested).
later termbox version weren't tested as well, so they may not work due to
changed API.

see the examples directory for usage of this library.

Compilation
-----------

    make BACKEND=SDL

or

    make BACKEND=NCURSES

or

    make BACKEND=TERMBOX

if you want to build the example programs, do the same, but use the examples
target, e.g.

    make BACKEND=NCURSES examples

TODO
----
it might make sense to start using the colors internally from 256 downwards,
as this has the advantage that the original 16 terminal colors never get changed
when using less than 240 colors (ncurses backend).
