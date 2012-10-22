#ifndef FONT_H
#define FONT_H

#include "../rect.h"

typedef struct {
	point dim;
	unsigned long pointsperchar;
	char *characters;
} font;

#endif
