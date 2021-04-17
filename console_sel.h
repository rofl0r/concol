#ifndef CONSOLE_SEL_H
#define CONSOLE_SEL_H

#define NCURSES_CONSOLE 1
#define TERMBOX_CONSOLE 2
#define SDL_CONSOLE 3
#define NOP_CONSOLE 4

#if !defined(CONSOLE_BACKEND) || CONSOLE_BACKEND < NCURSES_CONSOLE || CONSOLE_BACKEND > NOP_CONSOLE
#error "CONSOLE_BACKEND macro isn't set to one of the possible values!"
#endif

#endif
