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

#include <assert.h>
#include "../../lib/include/bitarray.h"
static char* bitfont_get_char(bitfont* f, unsigned int ch) {
	static unsigned int lastchr = (unsigned) -1;
	static unsigned char char_data[16*16];
	if(ch != lastchr) {
		unsigned char* p = char_data;
		size_t i, start = f->bitperchar * ch;
		assert(f->bitperchar <= sizeof(char_data));
		for(i = 0; i < f->bitperchar; i++)
			*(p++) = BA_GET(f->characters, start + i);
		lastchr = ch;
	}
	return (char*) char_data;
}


#endif
