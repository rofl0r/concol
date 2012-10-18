#ifndef TBCONSOLE_H
#define TBCONSOLE_H

//RcB: DEP "tbconsole.c"
//RcB: LINK "-L ../termbox -ltermbox"

typedef struct TbConsole {
	int fgcolor;
	int bgcolor;
} TbConsole;

#endif
