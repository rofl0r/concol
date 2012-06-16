#ifndef BITFONT_H
#define BITFONT_H

#include "font.h"

typedef struct {
	point dim;
	char characters[];
} bitfont;

font* bitfont_to_font(bitfont* f);

//RcB: DEP "bitfont.c"

#endif
