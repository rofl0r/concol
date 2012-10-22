#ifndef SDLCONSOLE_H
#define SDLCONSOLE_H

#include <stdint.h>
#include "endianness.h"

#include "point.h"
#include "rect.h"
#include "fonts/font.h"
#include "console.h"
#include "console_events.h"

//RcB: SKIPON "CONSOLE_BACKEND=TERMBOX_CONSOLE"
//RcB: SKIPON "CONSOLE_BACKEND=NCURSES_CONSOLE"
//RcB: LINK "-lSDL"
//RcB: DEP "sdlconsole.c"
//RcB: SKIPOFF "CONSOLE_BACKEND=TERMBOX_CONSOLE"
//RcB: SKIPOFF "CONSOLE_BACKEND=NCURSES_CONSOLE"


typedef union {
	struct colors {
#ifdef IS_LITTLE_ENDIAN
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
#else
		unsigned char a;
		unsigned char r;
		unsigned char g;
		unsigned char b;
#endif
	} colors;
	uint32_t val;
} sdl_rgb_t;

#define SRGB(R,G,B) ((sdl_rgb_t) {.colors.r = R, .colors.g = G, .colors.b = B, .colors.a = 0})
#define SRGB_INIT(x,y,z) ({.colors.r = R, .colors.g = G, .colors.b = B, .colors.a = 0})
#define SRGB3(X) SRGB(X)

#define SRGB_BLACK SRGB3(BLACK)
#define SRGB_WHITE SRGB3(WHITE)
#define SRGB_RED SRGB3(RED)
#define SRGB_GREEN SRGB3(GREEN)
#define SRGB_GREEN2 SRGB3(GREEN2)
#define SRGB_BLUE SRGB3(BLUE)
#define SRGB_YELLOW SRGB3(YELLOW)

#define SRGB_BLUE_NICE SRGB3(BLUE_NICE)
#define SRGB_GRAY_LIGHT SRGB3(GRAY_LIGHT)
#define SRGB_GRAY_DARK SRGB3(GRAY_DARK)

#define SRGB_NORTON_UTILITIES_VIOLET SRGB3(NORTON_UTILITIES_VIOLET)
#define SRGB_ORANGE  SRGB3(ORANGE)


typedef struct {
	sdl_rgb_t bgcolor;
	sdl_rgb_t fgcolor;
} sdl_rgb_tuple;

typedef struct SDLConsole {
	void *surface;
	sdl_rgb_tuple color;
	point res;
	font* fnt;
	int paintmode:1;
	int cursorblink:1;
	int fullscreen:1;
} SDLConsole;

#endif
