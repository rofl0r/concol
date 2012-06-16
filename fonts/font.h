#ifndef FONT_H
#define FONT_H

#include "../rect.h"

typedef struct {
	point dim;
	char* characters[256];
} font;

#endif
