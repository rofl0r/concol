#include "console.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

//generic algos used by all 3 backends

int console_setcolors(struct Console* self, rgb_t bgcolor, rgb_t fgcolor) {
	return
		console_setcolor(self, 0, bgcolor) +
		console_setcolor(self, 1, fgcolor);
}

void console_setautomove(Console* c, int automove) {
	c->automove = !!automove;
}

void console_unblink(Console* c) {
	if(c->isblinking)
		console_blink_cursor(c);
}

mouse_event console_getmouse(Console* c) {
	return c->mouse;
}

void console_getcursor(struct Console* self, int* x, int* y) {
	*x = self->cursor.x;
	*y = self->cursor.y;
}

void console_scrollup(Console* c) {
	(void) c;
}

void console_linebreak(Console* c) {
	console_unblink(c);
	c->cursor.x = 0;
	c->cursor.y++;
	if(c->cursor.y == c->dim.y) {
		console_scrollup(c);
		c->cursor.y--;
	}
}

void console_advance_cursor(Console* c, int steps) {
	// unblink former position
	console_unblink(c);

	if(c->cursor.x + steps >= c->dim.x)
		console_linebreak(c);
	else if(c->cursor.x + steps < 0) {
		if(c->cursor.y) {
			c->cursor.x = c->dim.x - 1;
			c->cursor.y--;
		}
	} else
		c->cursor.x += steps;
}

void console_fill(Console *c, rect* area, int ch) {
	int x, y;
	for(y = area->topleft.y; y <= area->bottomright.y; y++) {
		for(x = area->topleft.x; x <= area->bottomright.x; x++) {
			console_goto(c, x, y);
			console_putchar(c, ch, 0);
		}
	}
	console_draw(c);
}

static void cursor_move_do(int which, int* dest, int max) {
	if(which) {
		if(*dest + which >= 0 &&  *dest + which < max)
			*dest += which;
	}
}

static void cursor_move(Console* c, int x, int y) {
	int which, *dest, max;
	console_unblink(c);
	which = x; dest = &c->cursor.x; max = c->dim.x;
	cursor_move_do(which, dest, max);
	which = y; dest = &c->cursor.y; max = c->dim.y;
	cursor_move_do(which, dest, max);
	console_blink_cursor(c);
}

void console_cursor_up(Console* c) {
	cursor_move(c, 0, -1);
}

void console_cursor_down(Console* c) {
	cursor_move(c, 0, 1);
}

void console_cursor_left(Console* c) {
	cursor_move(c, -1, 0);
}

void console_cursor_right(Console* c) {
	cursor_move(c, 1, 0);
}

void console_printf(Console* c, const char* fmt, ...) {
	char dest[512];
	va_list ap;
	console_initoutput(c);
	va_start(ap, fmt);
	ssize_t x, result = vsnprintf(dest, sizeof(dest), fmt, ap);
	va_end(ap);
	for(x = 0; x < result; x++) {
		console_printchar(c, dest[x], 0);
	}
}

enum ConsoleBackend console_getbackendtype(Console *c) {
	return c->backendtype;
}
