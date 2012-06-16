#ifndef CONSOLE_BACKEND_H
#define CONSOLE_BACKEND_H

#define NCURSES_CONSOLE 1
#define TERMBOX_CONSOLE 2

#ifdef IN_KDEVELOP_PARSER
//make KDevelop not fuck up
#define CONSOLE_BACKEND NCURSES_CONSOLE
#endif


#if (CONSOLE_BACKEND == NCURSES_CONSOLE)
//RcB: SKIPON "CONSOLE_BACKEND=TERMBOX_CONSOLE"
#  include "ncconsole.h"
#  include "ncconsole_chars.h"
//RcB: SKIPOFF "CONSOLE_BACKEND=TERMBOX_CONSOLE"
#  define CONSOLE NcConsole
#elif (CONSOLE_BACKEND == TERMBOX_CONSOLE)
//RcB: SKIPON "CONSOLE_BACKEND=NCURSES_CONSOLE"
#  include "tbconsole.h"
#  include "tbconsole_chars.h"
//RcB: SKIPOFF "CONSOLE_BACKEND=NCURSES_CONSOLE"
#  define CONSOLE TbConsole
#else 
#  error no backend specified
#endif

#endif

