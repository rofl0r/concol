#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "sdlconsole.h"
#include "rect.h"
#include <stdarg.h>

void console_resize(Console *c, int w, int h);

static SDL_mutex *screens_lock;

void console_init(struct Console *self) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	c->paintmode = 0;
	c->cursorblink = 1;
	c->fullscreen = 0;
	c->super.automove = 0;
	c->super.isblinking = 0;
	c->surface = NULL;
}

void sdlconsole_init(SDLConsole* c, point resolution, font* fnt) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	screens_lock = SDL_CreateMutex();
	
	c->fnt = fnt;
	console_resize(&c->super, resolution.x, resolution.y);
	//c->fnt = bitfont_to_font(&int10_font_16);
	SDL_WM_SetCaption("sdl-console", NULL);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
}

void console_cleanup(Console* self) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	if(c->fullscreen) sdlconsole_toggle_fullscreen(c);
	SDL_DestroyMutex(screens_lock);
	SDL_Quit();
}

void console_initoutput(Console* self) {(void) self;}

void console_clear(Console* self) {
	rect fs = {{0}};
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
		return sdlconsole_translate_event((struct SDLConsole*) c, &event);
	}
	return 1;
}

int console_getkey(Console* c) {
	SDL_Event event;
	/* Loop through waiting messages and process them */
	int ret;
	while(1)
		while (SDL_PollEvent(&event)) {
			ret = sdlconsole_translate_event((struct SDLConsole*) c, &event);
			if(ret != CK_UNDEF) return ret;
			console_sleep(c, 1);
		}
	return 1;
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
	struct SDLConsole *c = (struct SDLConsole *) self;
	console_lock();
	c->res.x = w;
	c->res.y = h;
	c->super.dim.x = w / c->fnt->dim.x;
	c->super.dim.y = h / c->fnt->dim.y;
	if(c->surface) 
		SDL_FreeSurface(c->surface);
	
	c->surface = SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_HWPALETTE);
	if(!c->surface) {
		printf("Couldn't set screen mode to %d x %d : %s\n", w, h, SDL_GetError());
		exit(1);
	}
	console_unlock();
}

void sdlconsole_toggle_fullscreen(SDLConsole* c) {
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

static sdl_rgb_tuple sdlconsole_getcolors(SDLConsole* c) {
	return c->color;
}

rgb_tuple console_getcolors(Console* self) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	rgb_tuple ret;
	ret.fgcolor = srgb_to_rgb(c->color.fgcolor);
	ret.bgcolor = srgb_to_rgb(c->color.bgcolor);
	return ret;
}

int console_setcolor(struct Console* self, int is_fg, rgb_t color) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	sdl_rgb_t* dest = is_fg ? &c->color.fgcolor : &c->color.bgcolor;
	*dest = rgb_to_srgb(color);
	return 1;
}

void console_refresh(Console* self) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	console_lock();
	SDL_UpdateRect(c->surface, 0, 0, 0, 0);
	console_unlock();
}

void console_blink_cursor(Console* self) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	sdl_rgb_t *ptr = (sdl_rgb_t *) c->surface->pixels;
	int x, y, rx, ry, lineoffset;
	console_lock();
	if(c->cursorblink || c->isblinking) {
		for (y = 0, ry = c->super.cursor.y * c->fnt->dim.y; y < c->fnt->dim.y; y++, ry += 1) {
			for (x = 0, rx = c->super.cursor.x * c->fnt->dim.x; x < c->fnt->dim.x; x++, rx++) {
				lineoffset = ry * (c->surface->pitch / 4);
				ptr[lineoffset + rx].val = ~ptr[lineoffset + rx].val;
			}
		}
		SDL_UpdateRect(c->surface, c->super.cursor.x * c->fnt->dim.x ,c->super.cursor.y * c->fnt->dim.y, c->fnt->dim.x, c->fnt->dim.y);
		c->isblinking = ~c->isblinking;
	}
	console_unlock();
}

void console_putchar(Console* self, int ch, int doupdate) {
	struct SDLConsole *c = (struct SDLConsole *) self;
	console_unblink(self);
	console_lock();
	char* char_data = c->fnt->characters[ch & 0xff];
	sdl_rgb_t *ptr = (sdl_rgb_t *) c->surface->pixels;
	sdl_rgb_t *color;
	int lineoffset;
	int pitch_div_4 = (c->surface->pitch / 4);
	int x, y, rx, ry;
	for (y = 0, ry = c->super.cursor.y * c->fnt->dim.y; y < c->fnt->dim.y; y++, ry++) {
		lineoffset = ry * pitch_div_4;
		for (x = 0, rx = c->super.cursor.x * c->fnt->dim.x; x < c->fnt->dim.x; x++, rx++) {
			color = char_data[y * c->fnt->dim.x + x] ? &c->color.fgcolor : &c->color.bgcolor;
			ptr[lineoffset + rx] = *color;
		}
	}
	if(doupdate) SDL_UpdateRect(c->surface, c->super.cursor.x * c->fnt->dim.x ,c->super.cursor.y * c->fnt->dim.y, c->fnt->dim.x, c->fnt->dim.y);
	console_unlock();
	if(c->super.automove) console_advance_cursor(self, 1);
}




