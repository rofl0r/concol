#ifndef BITFONT_H
#define BITFONT_H

#include "font.h"

typedef struct {
	point dim;
	unsigned long bitperchar;
	char *characters;
} bitfont;

font* bitfont_to_font(bitfont* f);
bitfont* font_to_bitfont(font *f);

//RcB: DEP "bitfont.c"

#endif
