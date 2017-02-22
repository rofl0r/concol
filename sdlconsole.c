#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "console.h"
#include "rect.h"
#include <stdarg.h>

//RcB: SKIPON CONSOLE_FONT
#ifndef CONSOLE_FONT
#include "fonts/testfont.h"
#endif
//RcB: SKIPOFF CONSOLE_FONT

#define SDL_mutex_lock(X) SDL_mutexP(X)
#define SDL_mutex_unlock(X) SDL_mutexV(X)

//RcB: DEP "sdlconsole_chartab.c"

void console_resize(Console *c, int w, int h);

static SDL_mutex *screens_lock;

void console_init(struct Console *self) {
	memset(self, 0, sizeof(struct Console));
	self->backendtype = cb_sdl;
}
void console_settitle(Console *self, const char *title) {
	(void) self;
	SDL_WM_SetCaption(title, NULL);
}

void console_init_graphics(Console* self, point resolution, font* fnt) {
	struct SDLConsole *c = &self->backend.sdl;
	c->paintmode = 0;
	c->cursorblink = 1;
	c->fullscreen = 0;
	self->automove = 0;
	self->isblinking = 0;
	c->surface = NULL;

#ifndef CONSOLE_FONT
	if(!fnt) fnt = &testfont;
#endif
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	screens_lock = SDL_CreateMutex();

	c->fnt = fnt;
	console_resize(self, resolution.x, resolution.y);
	//c->fnt = bitfont_to_font(&int10_font_16);
	console_settitle(self, "sdl-console");
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void console_cleanup(Console* self) {
	struct SDLConsole *c = &self->backend.sdl;
	if(c->fullscreen) console_toggle_fullscreen(self);
	SDL_DestroyMutex(screens_lock);
	SDL_Quit();
}

int console_getcolorcount(Console *self) { (void) self; return 256; }

void console_initoutput(Console* self) {(void) self;}

void console_clear(Console* self) {
	rect fs = rect_zero;
	fs.bottomright.x = self->dim.x -1;
	fs.bottomright.y = self->dim.y -1;
	console_fill(self, &fs, ' ');
}
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <time.h>
#include <errno.h>

int msleep(long millisecs) {
        struct timespec req, rem;
        req.tv_sec = millisecs / 1000;
        req.tv_nsec = (millisecs % 1000) * 1000 * 1000;
        int ret;
        while((ret = nanosleep(&req, &rem)) == -1 && errno == EINTR) req = rem;
        return ret;
}

void console_sleep(struct Console* self, int ms) {
	(void) self;
	msleep(ms);
}

#define INCLUDED_FROM_SDLCONSOLE
#include "sdlconsole_keyboard.c"
int console_getkey_nb(Console* c) {
	SDL_Event event;
	/* Loop through waiting messages and process them */
	while (SDL_PollEvent(&event)) {
		return sdlconsole_translate_event(c, &event);
	}
	return CK_UNDEF;
}

int console_getkey(Console* c) {
	SDL_Event event;
	/* Loop through waiting messages and process them */
	int ret;
	while(1) {
		while (SDL_PollEvent(&event)) {
			ret = sdlconsole_translate_event(c, &event);
			if(ret != CK_UNDEF) return ret;
		}
		console_sleep(c, 1);
	}
	return CK_UNDEF;
}


/* prints a char and NOT advances cursor */
void console_addchar(struct Console* self, int c, unsigned int attributes) {
	(void) attributes;
	int save = self->automove;
	self->automove = 0;
	console_putchar(self, c, 0);
	self->automove = save;
}
/* prints a char and advances cursor */
void console_printchar(struct Console* self, int c, unsigned int attributes) {
	(void) attributes;
	int save = self->automove;
	self->automove = 1;
	console_putchar(self, c, 0);
	self->automove = save;
}

void console_goto(Console* c, int x, int y) {
	c->cursor.x = x;
	c->cursor.y = y;
}


void console_lock(void) {
	while(SDL_mutex_lock(screens_lock) == -1) {
		printf("warning: couldn't aquire screens_lock\n");
		SDL_Delay(1);
	}
}

void console_unlock(void) {
	SDL_mutex_unlock(screens_lock);
}

void console_resize(Console *self, int w, int h) {
	SDLConsole *c = &self->backend.sdl;
	console_lock();
	c->res.x = w;
	c->res.y = h;
	self->dim.x = w / c->fnt->dim.x;
	self->dim.y = h / c->fnt->dim.y;
	if(c->surface)
		SDL_FreeSurface(c->surface);

	c->surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_HWPALETTE);
	if(!c->surface) {
		printf("Couldn't set screen mode to %d x %d : %s\n", w, h, SDL_GetError());
		exit(1);
	}
	console_unlock();
}

void console_toggle_fullscreen(Console* self) {
	SDLConsole *c = &self->backend.sdl;
	console_lock();
	SDL_WM_ToggleFullScreen(c->surface);
	c->fullscreen = ~ c->fullscreen;
	console_unlock();
}

void console_getbounds(Console* c, int* x, int* y) {
	*x = c->dim.x;
	*y = c->dim.y;
}

static inline sdl_rgb_t rgb_to_srgb(rgb_t col) {
	sdl_rgb_t ret;
	ret.colors.a = col.a;
	ret.colors.r = col.r;
	ret.colors.g = col.g;
	ret.colors.b = col.b;
	return ret;
}

static inline rgb_t srgb_to_rgb(sdl_rgb_t col) {
	rgb_t ret;
	ret.a = col.colors.a;
	ret.r = col.colors.r;
	ret.g = col.colors.g;
	ret.b = col.colors.b;
	return ret;
}

rgb_tuple console_getcolors(Console* self) {
	struct SDLConsole *c = &self->backend.sdl;
	rgb_tuple ret;
	ret.fgcolor = srgb_to_rgb(c->color.fgcolor);
	ret.bgcolor = srgb_to_rgb(c->color.bgcolor);
	return ret;
}

int console_setcolor(struct Console* self, int is_fg, rgb_t color) {
	struct SDLConsole *c = &self->backend.sdl;
	sdl_rgb_t* dest = is_fg ? &c->color.fgcolor : &c->color.bgcolor;
	*dest = rgb_to_srgb(color);
	return 1;
}

void console_refresh(Console* self) {
	struct SDLConsole *c = &self->backend.sdl;
	console_lock();
	SDL_UpdateRect(c->surface, 0, 0, 0, 0);
	console_unlock();
}

void console_blink_cursor(Console* self) {
	struct SDLConsole *c = &self->backend.sdl;
	sdl_rgb_t *ptr = (sdl_rgb_t *) ((SDL_Surface*) c->surface)->pixels;
	int x, y, rx, ry, lineoffset;
	console_lock();
	if(c->cursorblink || self->isblinking) {
		for (y = 0, ry = self->cursor.y * c->fnt->dim.y; y < c->fnt->dim.y; y++, ry += 1) {
			for (x = 0, rx = self->cursor.x * c->fnt->dim.x; x < c->fnt->dim.x; x++, rx++) {
				lineoffset = ry * (((SDL_Surface*) c->surface)->pitch / 4);
				ptr[lineoffset + rx].val = ~ptr[lineoffset + rx].val;
			}
		}
		SDL_UpdateRect(c->surface, self->cursor.x * c->fnt->dim.x ,self->cursor.y * c->fnt->dim.y, c->fnt->dim.x, c->fnt->dim.y);
		self->isblinking = ~self->isblinking;
	}
	console_unlock();
}

#include <assert.h>
#include <limits.h>
#define BA_TARGET_BYTE(X, Y) ((X) + ((Y) / CHAR_BIT))
#define BA_BIT_DISTANCE(X, Y) ((Y) % CHAR_BIT)
#define BA_GET(X, Y) (!!( *BA_TARGET_BYTE(X, Y) & (1 << BA_BIT_DISTANCE(X, Y)) ))
static char* bitfont_get_char(font* f, unsigned int ch) {
	static unsigned int lastchr = (unsigned) -1;
	static unsigned char char_data[16*16];
	if(ch != lastchr) {
		unsigned char* p = char_data;
		size_t i, start = f->pointsperchar * ch;
		assert(f->pointsperchar <= sizeof(char_data));
		for(i = 0; i < f->pointsperchar; i++)
			*(p++) = BA_GET(f->characters, start + i);
		lastchr = ch;
	}
	return (char*) char_data;
}

void console_putchar(Console* self, int ch, int doupdate) {
	struct SDLConsole *c = &self->backend.sdl;
	console_unblink(self);
	console_lock();
	unsigned char* font = (void*)bitfont_get_char(c->fnt, ch & 0xff);
	int pitch_div_4 = (((SDL_Surface*) c->surface)->pitch / 4);
	sdl_rgb_t *ptr = (sdl_rgb_t *) ((SDL_Surface*) c->surface)->pixels;
	ptr += self->cursor.y * c->fnt->dim.y * pitch_div_4;
	ptr += self->cursor.x * c->fnt->dim.x;
	size_t advance = (pitch_div_4 - c->fnt->dim.x);
	sdl_rgb_t color[2] = {[0] = c->color.bgcolor, [1] = c->color.fgcolor};
	int x, y;
	for (y = 0; y < c->fnt->dim.y; y++, ptr += advance) {
		for (x = 0; x < c->fnt->dim.x; x++, font++, ptr++) {
			*ptr = color[*font];
		}
	}
	if(doupdate) SDL_UpdateRect(c->surface, self->cursor.x * c->fnt->dim.x ,self->cursor.y * c->fnt->dim.y, c->fnt->dim.x, c->fnt->dim.y);
	console_unlock();
	if(self->automove) console_advance_cursor(self, 1);
}




