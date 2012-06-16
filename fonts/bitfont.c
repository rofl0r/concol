#include "bitfont.h"
#include "font.h"
#include "../../lib/include/bitarray.h"
#include <stdlib.h>
#include <limits.h>

#define get_bit_swapped(X, Y) BA_GET( (X) , (((Y) / CHAR_BIT) * CHAR_BIT) + ((CHAR_BIT - 1) - ((Y) % CHAR_BIT)) )

font* bitfont_to_font(bitfont* f) {
	font* res = malloc(sizeof(font) + (256 * (f->dim.x * f->dim.y)));
	int i, x, y;
	if(res) {
		res->dim = f->dim;
		for(i = 0; i < 256; i++) {
			res->characters[i] = ((char*) res) + sizeof(font) + (res->dim.x * res->dim.y * i);
			for(y = 0; y < res->dim.y; y++) {
				for(x = 0; x < res->dim.x; x++) {
					res->characters[i][y * res->dim.x + x] = get_bit_swapped((unsigned char*)f->characters, (i * (res->dim.x * res->dim.y)) + (y * res->dim.x) + x);
				}
			}
		}
	}
	return res;
}
