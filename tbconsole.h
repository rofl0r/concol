#ifndef TBCONSOLE_H
#define TBCONSOLE_H

#include "console.h"
//RcB: DEP "tbconsole.c"
//RcB: LINK "-L ../termbox -ltermbox"


typedef struct TbConsole {
	struct Console super;
	int fgcolor;
	int bgcolor;
} TbConsole;

#endif
