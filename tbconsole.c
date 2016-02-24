#include "console.h"
#include "console_keys.h"
#include "colors.h"
#include "../termbox/src/termbox.h"
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>

//RcB: DEP "tbconsole_chartab.c"

#define ARRAY_SIZE(X) (sizeof(X) / sizeof((X)[0]))

static const rgb_t tbcolors[] = {
	[TB_BLACK] = RGB3(BLACK),
	[TB_RED] = RGB3(RED),
	[TB_GREEN] = RGB3(GREEN),
	[TB_YELLOW] = RGB3(YELLOW),
	[TB_BLUE] = RGB3(BLUE),
	[TB_MAGENTA] = RGB3(MAGENTA),
	[TB_CYAN] = RGB3(CYAN),
	[TB_WHITE] = RGB3(WHITE),
};

/* initialize a Console struct */
void console_init(struct Console* self) {
	memset(self, 0, sizeof(struct Console));
	self->backendtype = cb_termbox;
}

/* cleanup restores the original term behaviour and releases acquired resources. */
void console_cleanup(struct Console* self) {
	(void) self;
	tb_shutdown();
}

#define ABS(X) ((X) < 0 ? (X) * -1 : (X))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
static int getNearestColor(rgb_t col) {
	unsigned dist[ARRAY_SIZE(tbcolors)];
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
	for(i = 0; i < ARRAY_SIZE(tbcolors); i++) {
		dist[i] = ABS(tbcolors[i].r * 256 - col.r * scale) + 
			  ABS(tbcolors[i].g * 256 - col.g * scale) + 
			  ABS(tbcolors[i].b * 256 - col.b * scale);
		if(dist[i] == 0) return i;
		if(dist[i] < nearest) nearest = dist[i];
	}
	for(i = 0; i < ARRAY_SIZE(tbcolors); i++) {
		if(dist[i] == nearest) return i;
	}
	return 0;
}

int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor) {
	struct TbConsole *c = &self->backend.tb;
	int *dest = is_fg ? &c->fgcolor : &c->bgcolor;
	*dest = getNearestColor(mycolor);
	return 1;
}

int console_getcolorcount(Console *self) { (void) self; return 8; }

void console_initoutput(struct Console* self) { (void) self; }

/* get width and height of the console display (in characters) */
void console_getbounds(struct Console* self, int* width, int* height) {
	self->dim.x = *width = tb_width();
	self->dim.y = *height = tb_height();
}

void console_goto(struct Console* self, int x, int y) {
	self->cursor.x = x;
	self->cursor.y = y;
}

/* prints a char and NOT advances cursor */
void console_addchar(struct Console* self, int c, unsigned int attributes) {
	(void) attributes;
	struct TbConsole *con = &self->backend.tb;
	tb_change_cell(self->cursor.x, self->cursor.y, c, con->fgcolor, con->bgcolor);
}
/* prints a char and advances cursor */
void console_printchar(struct Console* self, int c, unsigned int attributes) {
	int newx = self->cursor.x == (int) tb_width() ? 1 : self->cursor.x + 1;
	int newy = self->cursor.x == (int) tb_width() ? self->cursor.y + 1 : self->cursor.y;
	console_addchar(self, c, attributes);
	console_goto(self, newx, newy);
}

void console_putchar(Console* self, int ch, int doupdate) {
	console_addchar(self, ch, 0);
	if(self->automove) console_advance_cursor(self, 1);
	if(doupdate) console_refresh(self);
}


/*
static void print_tb(const char *str, unsigned int x, unsigned int y, uint16_t fg, uint16_t bg) {
	while (*str) {
		uint32_t uni;
		str += utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, fg, bg);
		x++;
	}
}

void console_printf (struct Console* con, const char* fmt, ...) {
	struct TbConsole *self = (struct TbConsole*) con;
	console_initoutput(con);
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = vsnprintf(buf, sizeof(buf), fmt, ap);
	(void) result;
	va_end(ap);
	print_tb (buf, con->cursor.x, con->cursor.y, self->fgcolor, self->bgcolor);
}

void console_printfxy (struct Console* con, int x, int y, const char* fmt, ...) {
	struct TbConsole *self = (struct TbConsole*) con;
	console_initoutput(con);
	char buf[256];
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = vsnprintf(buf, sizeof(buf), fmt, ap);
	(void) result;
	va_end(ap);
	print_tb (buf, x, y, self->fgcolor, self->bgcolor);
}*/
#define TB_KEY_MIN TB_KEY_ARROW_RIGHT
#define TB_KEY_MAX TB_KEY_F1
static const unsigned char key_table[] = {
	[TB_KEY_F1 - TB_KEY_MIN] = CK_F1 - 0x100,
	[TB_KEY_F2 - TB_KEY_MIN] = CK_F2 - 0x100,
	[TB_KEY_F3 - TB_KEY_MIN] = CK_F3 - 0x100,
	[TB_KEY_F4 - TB_KEY_MIN] = CK_F4 - 0x100,
	[TB_KEY_F5 - TB_KEY_MIN] = CK_F5 - 0x100,
	[TB_KEY_F6 - TB_KEY_MIN] = CK_F6 - 0x100,
	[TB_KEY_F7 - TB_KEY_MIN] = CK_F7 - 0x100,
	[TB_KEY_F8 - TB_KEY_MIN] = CK_F8 - 0x100,
	[TB_KEY_F9 - TB_KEY_MIN] = CK_F9 - 0x100,
	[TB_KEY_F10 - TB_KEY_MIN] = CK_F10 - 0x100,
	[TB_KEY_F11 - TB_KEY_MIN] = CK_F11 - 0x100,
	[TB_KEY_F12 - TB_KEY_MIN] = CK_F12 - 0x100,
	[TB_KEY_INSERT - TB_KEY_MIN] = CK_INS - 0x100,
	[TB_KEY_DELETE - TB_KEY_MIN] = CK_DEL - 0x100,
	[TB_KEY_HOME - TB_KEY_MIN] = CK_HOME - 0x100,
	[TB_KEY_END - TB_KEY_MIN] = CK_END - 0x100,
	[TB_KEY_PGUP - TB_KEY_MIN] = CK_PAGE_UP - 0x100,
	[TB_KEY_PGDN - TB_KEY_MIN] = CK_PAGE_DOWN - 0x100,
	[TB_KEY_ARROW_UP - TB_KEY_MIN] = CK_CURSOR_UP - 0x100,
	[TB_KEY_ARROW_DOWN - TB_KEY_MIN] = CK_CURSOR_DOWN - 0x100,
	[TB_KEY_ARROW_LEFT - TB_KEY_MIN] = CK_CURSOR_LEFT - 0x100,
	[TB_KEY_ARROW_RIGHT - TB_KEY_MIN] = CK_CURSOR_RIGHT - 0x100,
};
#define key_table_size (sizeof(key_table) / sizeof(key_table[0]))

static int event_to_key(int retval, struct tb_event *e) {
	if(retval == TB_EVENT_RESIZE) return CK_RESIZE_EVENT;
	int ret = 0;
	if (e->key >= TB_KEY_MIN) {
		// special key
		if(e->key - TB_KEY_MIN >= (ssize_t) key_table_size) {
			//BUG
			return CK_ERR;
		}
		ret = key_table[e->key - TB_KEY_MIN] + 0x100;

	} else if (e->ch < 128) {
		if (e->ch == 0 && e->key < 128)
			ret = e->key;
		else
			ret = e->ch;
	}
	if (e->mod & TB_MOD_ALT) ret |= CK_MOD_ALT;
	return ret;
}

/* blocking getkey */
int console_getkey(struct Console* self) {
	(void) self;
	struct tb_event e;
	int ret = tb_poll_event(&e);
	if(ret == 0) ret = -1;
	return event_to_key(ret, &e);
}

/* non blocking getkey. returns -1 if no data is available */
int console_getkey_nb(struct Console* self) {
	(void) self;
	struct tb_event e;
	int ret = tb_peek_event(&e, 0);
	if(ret == 0) ret = -1;
	return event_to_key(ret, &e);
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

void console_refresh(struct Console* self) {
	(void) self;
	tb_present();
}

void console_clear(struct Console* self) {
	(void) self;
	tb_clear();
}

void console_blink_cursor(struct Console* self) { (void) self; }
void console_lock(void) {}
void console_unlock(void) {}
void console_toggle_fullscreen(struct Console* self) { (void) self; }

void console_init_graphics(Console* self, point resolution, font* fnt) {
	(void) resolution; (void) fnt;
	tb_init();
	tb_select_input_mode(TB_INPUT_ESC);
	tb_clear();
	self->dim.x = tb_width();
	self->dim.y = tb_height();
}
