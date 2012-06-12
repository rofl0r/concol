#ifndef CONSOLE_H
#define CONSOLE_H

typedef struct Cursor {
	int x;
	int y;
} Cursor;

typedef struct Console {
	Cursor cursor;
} Console;

/* initialize a Console struct */
void console_init(struct Console* self);
/* cleanup restores the original term behaviour and releases acquired resources. */
void console_cleanup(struct Console* self);

int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor);
int console_setcolors(struct Console* self, rgb_t bgcolor, rgb_t fgcolor);
void console_initoutput(struct Console* self);
void console_getsize(struct Console* self, int* x, int* y);
void console_getcursor(struct Console* self, int* x, int* y);
void console_goto(struct Console* self, int x, int y);
void console_addchar(struct Console* self, int c, unsigned int attributes);
void console_printchar(struct Console* self, int c, unsigned int attributes);
void console_printxy (struct Console* self, int x, int y, char* text);
int console_getkey(struct Console* self);
void console_sleep(struct Console* self, int ms);
void console_refresh(struct Console* self);
void console_clear(struct Console* self);

/*
TODO :

void sdlconsole_init(sdlconsole* c, point resolution, font* fnt);
void sdlconsole_getbounds(sdlconsole* c, int* x, int* y);
rgb_tuple sdlconsole_getcolors(sdlconsole* c);
void sdlconsole_putchar(sdlconsole* c, int ch, int doupdate);
int sdlconsole_getchar(sdlconsole* c);
void sdlconsole_printf(sdlconsole* c, const char* fmt, ...);
void sdlconsole_cursor_up(sdlconsole* c);
void sdlconsole_cursor_down(sdlconsole* c);
void sdlconsole_cursor_left(sdlconsole* c);
void sdlconsole_cursor_right(sdlconsole* c);

OK void sdlconsole_cleanup(sdlconsole* c);
OK void sdlconsole_goto(sdlconsole* c, int x, int y);
OK void sdlconsole_setcolor(sdlconsole* c, int is_fg, sdl_rgb_t color);
OK void sdlconsole_setcolors(sdlconsole* c, sdl_rgb_t bgcolor, sdl_rgb_t fgcolor);
OK void sdlconsole_getcursor(sdlconsole* c, int* x, int* y);

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
