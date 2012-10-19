#ifndef TBCONSOLE_H
#define TBCONSOLE_H

//RcB: SKIPON "CONSOLE_BACKEND=SDL_CONSOLE"
//RcB: SKIPON "CONSOLE_BACKEND=NCURSES_CONSOLE"
//RcB: DEP "tbconsole.c"
//RcB: LINK "-L ../termbox -ltermbox"
//RcB: SKIPOFF "CONSOLE_BACKEND=SDL_CONSOLE"
//RcB: SKIPOFF "CONSOLE_BACKEND=NCURSES_CONSOLE"

typedef struct TbConsole {
	int fgcolor;
	int bgcolor;
} TbConsole;

#endif
