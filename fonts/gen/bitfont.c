#include "bitfont.h"
#include "../font.h"
#include "../../../lib/include/bitarray.h"
#include <stdlib.h>
#include <limits.h>

#define get_bit_swapped(X, Y) BA_GET( (X) , (((Y) / CHAR_BIT) * CHAR_BIT) + ((CHAR_BIT - 1) - ((Y) % CHAR_BIT)) )


static inline char* font_get_char(font* f, unsigned long index) {
	return f->characters + (f->pointsperchar * (index & 0xff)); 
}

font* bitfont_to_font(bitfont* f) {
	font* res = malloc(sizeof(font) + (256 * (f->dim.x * f->dim.y)));
	char* curr;
	int i, x, y;
	if(res) {
		res->dim = f->dim;
		res->pointsperchar = f->dim.x * f->dim.y;
		res->characters = ((char*) res) + sizeof(font);
		curr = font_get_char(res, 0);
		for(i = 0; i < 256; i++) {
			//res->characters[i] = ((char*) res) + sizeof(font) + (res->dim.x * res->dim.y * i);
			for(y = 0; y < res->dim.y; y++) {
				for(x = 0; x < res->dim.x; x++) {
					*(curr++) = get_bit_swapped((unsigned char*)f->characters, (i * (res->dim.x * res->dim.y)) + (y * res->dim.x) + x);
				}
			}
		}
	}
	return res;
}

bitfont* font_to_bitfont(font *f) {
	bitfont *res = malloc(sizeof(bitfont) + BA_SIZE_REQUIRED(f->pointsperchar * 256));
	res->pointsperchar=f->pointsperchar;
	res->dim.x = f->dim.x;
	res->dim.y = f->dim.y;
	res->characters = ((char*) res) + sizeof(bitfont);
	size_t c;
	char* src = f->characters;
	for(c = 0; c < 256 * f->pointsperchar; c++) {
		BA_SET(res->characters, c, *(src++));
	}
	return res;
}
