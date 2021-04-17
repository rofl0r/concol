#ifndef NOPCONSOLE_H
#define NOPCONSOLE_H

#include "console_sel.h"

#if CONSOLE_BACKEND == NOP_CONSOLE
#pragma RcB2 DEP "nopconsole.c"
#endif

typedef struct NopConsole {
	int dummy;
} NopConsole;

#endif
