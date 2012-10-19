#ifndef FONT_H
#define FONT_H

#include "../rect.h"

typedef struct {
	point dim;
	unsigned long bytesperchar;
	char *characters;
} font;

static inline char* font_get_char(font* f, unsigned long index) {
	return f->characters + (f->bytesperchar * (index & 0xff)); 
}

#endif
