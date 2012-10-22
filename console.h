#ifndef CONSOLE_H
#define CONSOLE_H

//RcB: DEP "console.c"

#include "point.h"
#include "rect.h"
#include "console_events.h"
#include "console_chars.h"

#include "sdlconsole.h"
#include "tbconsole.h"
#include "ncconsole.h"

#include "fonts/bitfont.h"

extern unsigned int console_chartab[CC_MAX];
#define CCT(charname) (console_chartab[charname])

#ifdef CONSOLE_FONT
#include "fonts/allfonts.h"
#endif

enum ConsoleBackend {
	cb_sdl = 0,
	cb_ncurses,
	cb_termbox,
};

typedef struct Console {
	enum ConsoleBackend backendtype;
	point cursor;
	point dim; //dimensions
	mouse_event mouse;
	int automove:1; // flag which affects putchar and printf (cursor will be advanced)
	int isblinking:1;
	union {
		TbConsole tb;
		SDLConsole sdl;
		NcConsole nc;
	} backend;
} Console;

#include "rgb.h"

/* initialize a Console struct */
void console_init(struct Console* self);
/* cleanup restores the original term behaviour and releases acquired resources. */
void console_cleanup(struct Console* self);

int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor);
int console_setcolors(struct Console* self, rgb_t bgcolor, rgb_t fgcolor);
void console_initoutput(struct Console* self);
/* get width and height of the console display (in characters) */
void console_getbounds(struct Console* self, int* width, int* height);
void console_getcursor(struct Console* self, int* x, int* y);
void console_goto(struct Console* self, int x, int y);
/* prints a char and NOT advances cursor */
void console_addchar(struct Console* self, int c, unsigned int attributes);
/* prints a char and advances cursor */
void console_printchar(struct Console* self, int c, unsigned int attributes);

/* prints a char and updates (redraws) the screen when doupdate is 1. advances cursor if automove is set. */
void console_putchar(Console* self, int ch, int doupdate);

//void console_printfxy (struct Console* con, int x, int y, const char* fmt, ...);

#define console_printfxy(con, x, y, ...) do { console_goto(con, x, y); console_printf(con, __VA_ARGS__); } while (0)

void console_printf (struct Console* con, const char* fmt, ...);
/* blocking */
int console_getkey(struct Console* self);
/* non-blocking */
int console_getkey_nb(struct Console* self);
void console_sleep(struct Console* self, int ms);

#define console_draw(C) console_refresh(C)
void console_refresh(struct Console* self);
void console_clear(struct Console* self);

void console_lock(void);
void console_unlock(void);
void console_blink_cursor(struct Console* self);

// generic
void console_fill(Console *c, rect* area, int ch);
mouse_event console_getmouse(Console* c);
void console_advance_cursor(Console* c, int steps);
void console_setautomove(Console* c, int automove);
void console_linebreak(Console* c);
void console_cursor_up(Console* c);
void console_cursor_down(Console* c);
void console_cursor_left(Console* c);
void console_cursor_right(Console* c);
void console_unblink(Console* c);
enum ConsoleBackend console_getbackendtype(Console *c);

/* sdl-specific, only implemented in SDL backend. */
void console_toggle_fullscreen(Console *c);
/* must be called after console_init() */
void console_init_graphics(Console* self, point resolution, bitfont* fnt);

/*
TODO :

rgb_tuple sdlconsole_getcolors(sdlconsole* c);
void sdlconsole_putchar(sdlconsole* c, int ch, int doupdate);
int sdlconsole_getchar(sdlconsole* c);
void sdlconsole_cursor_up(sdlconsole* c);
void sdlconsole_cursor_down(sdlconsole* c);
void sdlconsole_cursor_left(sdlconsole* c);
void sdlconsole_cursor_right(sdlconsole* c);

OK void sdlconsole_cleanup(sdlconsole* c);
OK void sdlconsole_goto(sdlconsole* c, int x, int y);
OK void sdlconsole_setcolor(sdlconsole* c, int is_fg, sdl_rgb_t color);
OK void sdlconsole_setcolors(sdlconsole* c, sdl_rgb_t bgcolor, sdl_rgb_t fgcolor);
OK void sdlconsole_getcursor(sdlconsole* c, int* x, int* y);
OK void sdlconsole_getbounds(sdlconsole* c, int* x, int* y);
OK void sdlconsole_printf(sdlconsole* c, const char* fmt, ...);

EVAL:

void sdlconsole_draw(sdlconsole* c);
void sdlconsole_blink_cursor(sdlconsole* c);
void sdlconsole_unblink(sdlconsole* c);
void sdlconsole_scrollup(sdlconsole* c);
void sdlconsole_linebreak(sdlconsole* c);
void sdlconsole_advance_cursor(sdlconsole* c, int steps);
void sdlconsole_toggle_fullscreen(sdlconsole* c);

void sdlconsole_resize(sdlconsole *c, int w, int h);
void sdlconsole_setautomove(sdlconsole* c, int automove);
mouse_event sdlconsole_getmouse(sdlconsole* c);
void sdlconsole_fill(sdlconsole *c, rect* area, int ch);
int sdlconsole_translate_event(sdlconsole* c, SDL_Event* ev);
void sdlconsole_lock(void);
void sdlconsole_unlock(void); */


#endif
