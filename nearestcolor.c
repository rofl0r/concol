#include "console.h"
#include "colors.h"
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(X) (sizeof(X) / sizeof((X)[0]))
#endif

#ifndef TB_BLACK

#define TB_BLACK        0x00
#define TB_RED          0x01
#define TB_GREEN        0x02
#define TB_YELLOW       0x03
#define TB_BLUE         0x04
#define TB_MAGENTA      0x05
#define TB_CYAN         0x06
#define TB_WHITE        0x07

#endif

static const rgb_t defcolors[] = {
	[TB_BLACK] = RGB3(BLACK),
	[TB_RED] = RGB3(RED),
	[TB_GREEN] = RGB3(GREEN),
	[TB_YELLOW] = RGB3(YELLOW),
	[TB_BLUE] = RGB3(BLUE),
	[TB_MAGENTA] = RGB3(MAGENTA),
	[TB_CYAN] = RGB3(CYAN),
	[TB_WHITE] = RGB3(WHITE),
};

#define ABS(X) ((X) < 0 ? (X) * -1 : (X))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
static int getNearestColor(rgb_t col) {
	unsigned dist[ARRAY_SIZE(defcolors)];
	unsigned i;
	unsigned nearest = 0xFFFFFFFF;
	unsigned brightestcol = 0;
	if(col.r == col.g && col.r == col.b) {
		if(col.r < 128) return TB_BLACK;
		else return TB_WHITE;
	}
	brightestcol = MAX(brightestcol, col.r);
	brightestcol = MAX(brightestcol, col.g);
	brightestcol = MAX(brightestcol, col.b);
	brightestcol = MAX(brightestcol, 1);
	int scale = (255*256) / brightestcol;
	for(i = 0; i < ARRAY_SIZE(defcolors); i++) {
		dist[i] = ABS(defcolors[i].r * 256 - col.r * scale) + 
			  ABS(defcolors[i].g * 256 - col.g * scale) + 
			  ABS(defcolors[i].b * 256 - col.b * scale);
		if(dist[i] == 0) return i;
		if(dist[i] < nearest) nearest = dist[i];
	}
	for(i = 0; i < ARRAY_SIZE(defcolors); i++) {
		if(dist[i] == nearest) return i;
	}
	return 0;
}


#if 0
int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor) {
	struct TbConsole *c = &self->backend.tb;
	int *dest = is_fg ? &c->fgcolor : &c->bgcolor;
	*dest = getNearestColor(mycolor);
	return 1;
}
#endif
