#ifndef TBCONSOLE_H
#define TBCONSOLE_H

#include "console_sel.h"

#if CONSOLE_BACKEND == TERMBOX_CONSOLE
#pragma RcB2 DEP "tbconsole.c"
#pragma RcB2 LINK "-L ../termbox -ltermbox"
#endif

typedef struct TbConsole {
	int fgcolor;
	int bgcolor;
} TbConsole;

#endif
