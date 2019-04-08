#ifndef BITFONT_OLD_H
#define BITFONT_OLD_H
#include "../font.h"

typedef font bitfont;

font* bitfont_to_font(bitfont* f);
bitfont* font_to_bitfont(font *f);

#pragma RcB2 DEP "bitfont.c"

#endif
