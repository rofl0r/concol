#include "console.h"
#include "console_keys.h"
#include "colors.h"
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>


/* initialize a Console struct */
void console_init(struct Console* self) {
}

/* cleanup restores the original term behaviour and releases acquired resources. */
void console_cleanup(struct Console* self) {
}

int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor) {
	return 1;
}

int console_getcolorcount(Console *self) { (void) self; return 8; }

void console_initoutput(struct Console* self) { (void) self; }

/* get width and height of the console display (in characters) */
void console_getbounds(struct Console* self, int* width, int* height) {
	self->dim.x = *width = 80;
	self->dim.y = *height = 25;
}

void console_goto(struct Console* self, int x, int y) {
	self->cursor.x = x;
	self->cursor.y = y;
}

/* prints a char and NOT advances cursor */
void console_addchar(struct Console* self, int c, unsigned int attributes) {
}
/* prints a char and advances cursor */
void console_printchar(struct Console* self, int c, unsigned int attributes) {
}

void console_putchar(Console* self, int ch, int doupdate) {
}


/*
void console_printf (struct Console* con, const char* fmt, ...) {
}

void console_printfxy (struct Console* con, int x, int y, const char* fmt, ...) {
}
*/

int console_getkey(struct Console* self) {
	return '\n';
}

/* non blocking getkey. returns -1 if no data is available */
int console_getkey_nb(struct Console* self) {
	return -1;
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
}

void console_clear(struct Console* self) {
}

void console_blink_cursor(struct Console* self) { (void) self; }
void console_lock(void) {}
void console_unlock(void) {}
void console_toggle_fullscreen(struct Console* self) { (void) self; }

void console_init_graphics(Console* self, point resolution, font* fnt) {
}

void console_settitle(Console *self, const char *title) {
}

