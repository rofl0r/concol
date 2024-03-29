/*
 *
 * Created on: 29.11.2010
 *
 *
 * author: rofl0r
 *
 * License: LGPL 2.1+ with static linking exception
 *
 *
 *
 * library for sane 256 color handling in xterm
 *
 * without manual bookkeeping for colorpairs
 *
 *
 */

#include "console.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include "console_keys.h"
#include "color_reader.h"

#include "ncconsole_chartab.c"

#define MIN_PAIR 1
#define MAX_PAIR (CONSOLE_COLORPAIRCOUNT-1)
#define MIN_COLOR 0

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static const rgb_t invalid_color = RGBA_INIT(0,0,0,255);

#ifdef CONSOLE_DEBUG
static FILE* dbg = NULL;
#define PDEBUG(fmt, args...) do { if(dbg) fprintf(dbg, fmt, ## args); } while(0)
#else
#define PDEBUG(fmt, args...) do {} while (0)
#endif

static inline int self_hasColors(struct NcConsole* self) {
	return self->flags & NC_HASCOLORS;
}
static inline int self_canChangeColors(struct NcConsole* self) {
	return self->flags & NC_CANCHANGECOLORS;
}
static inline int self_hasMouse(struct NcConsole* self) {
	return self->flags & NC_HASMOUSE;
}

static void console_savecolors(struct NcConsole* self);
static void console_restorecolors(struct NcConsole* self);
static int console_setcursescolor(struct NcConsole* self, int colornumber, rgb_t color);
static int console_setcolorpair(struct NcConsole* self, int pair, int fgcol, int bgcol);
static int console_usecolorpair(struct NcConsole* self, int pair);

#include <pthread.h>
#pragma RcB2 LINK "-lpthread"

static pthread_mutex_t resize_mutex;

static inline int console_fromthousand(int in) {
	return in == 0 ? 0 : in == 1000 ? 255 : (in * 1000 * 1000) / 3921568;
}

static inline int console_tothousand(int in) {
	return in == 0 ? 0 : in == 255 ? 1000 : (in * 3921568) / (1000 * 1000);
}

static inline void console_inittables(struct Console* self) {
	struct NcConsole *con = &self->backend.nc;
	int i;
	for (i = 0; i <= MAX_PAIR; i++) {
		con->colors[i] = invalid_color;
		con->pairs[i].fgcol = -1;
		con->pairs[i].bgcol = -1;
	}
}

void console_init(struct Console* con) {
	memset(con, 0, sizeof(struct Console));
	con->backendtype = cb_ncurses;
	pthread_mutex_init(&resize_mutex, NULL);
}

void console_cleanup(struct Console* con) {
	struct NcConsole *self = &con->backend.nc;
	clear();
	refresh();
	if (self_canChangeColors(self)) console_restorecolors(self);
	endwin();
#ifdef CONSOLE_DEBUG
	fclose(dbg);
#endif
	pthread_mutex_destroy(&resize_mutex);
}

int console_getcolorcount(Console *c) {
	return c->backend.nc.maxcolors;
}

static void console_savecolors(struct NcConsole *self) {
	short int i;
	short int r,g,b;
	short int fg, bg;
	struct color_reader cr;
	int use_cr;
	use_cr = self->flags & NC_SUPPORTSCOLORREADER && !color_reader_init(&cr);
	int maxc = MIN(self->maxcolors, CONSOLE_MAXSAVECOLORS);
	for (i = MIN_COLOR; i < maxc; i++) {
		if(use_cr) color_reader_get_color(&cr, i, &self->org_colors[i]);
		else {
			color_content(i, &r, &g, &b);
			self->org_colors[i] = RGB(console_fromthousand(r), console_fromthousand(g), console_fromthousand(b));
		}
	}
	if(use_cr) color_reader_close(&cr);
	for (i = MIN_PAIR; i < maxc+MIN_PAIR; i++) {
		pair_content(i, &fg, &bg);
		self->org_fgcolors[i-MIN_PAIR] = fg;
		self->org_bgcolors[i-MIN_PAIR] = bg;
	}
}

static void console_restorecolors(struct NcConsole *self) {
	int i, maxc = MIN(self->maxcolors, CONSOLE_MAXSAVECOLORS);
	for (i = MIN_COLOR; i < maxc; i++) {
		init_color(i,
			console_tothousand(self->org_colors[i].r),
			console_tothousand(self->org_colors[i].g),
			console_tothousand(self->org_colors[i].b)
		);
	}
	for (i = MIN_PAIR; i < maxc+MIN_PAIR; i++) {
		init_pair(i, self->org_fgcolors[i-MIN_PAIR], self->org_bgcolors[i-MIN_PAIR]);
	}
	color_set(MIN_PAIR, NULL);
}

// needs color additionally to be used by restorecolors
static int console_setcursescolor(struct NcConsole* self, int colornumber, rgb_t color) {
	PDEBUG("setcursescolor: %d (%d, %d, %d)\n", colornumber, color.r, color.g, color.b);

	if(colornumber > MAX_PAIR) return 0;

	// we use rgb values in the range 0-0xFF, while ncurses max is 1000
	if(!self_canChangeColors(self)) return 0;

	int nr = console_tothousand(color.r);
	int ng = console_tothousand(color.g);
	int nb = console_tothousand(color.b);

	PDEBUG("init_color: %d (%d, %d, %d)\n", colornumber+1, nr, ng, nb);

	return init_color(colornumber+MIN_COLOR, nr, ng, nb) != ERR;
}

#include "nearestcolor.c"

int console_setcolor(struct Console* con, int is_fg, rgb_t mycolor) {
	struct NcConsole *self = &con->backend.nc;
	int i;

	if( ! (self->flags & NC_CANCHANGECOLORS) ) {
		int nearest = getNearestColor(mycolor);
		if(is_fg) self->active.fgcol = nearest;
		else self->active.bgcol = nearest;
		return 1;
	}

	short* which = is_fg ? &self->active.fgcol : &self->active.bgcol;

	PDEBUG("setcolor: (%d, %d, %d), fg: %d\n", mycolor.r, mycolor.g, mycolor.b, is_fg);

	// see if it's the actual color...
	if (*which >= 0) {
		if (self->colors[*which].asInt == mycolor.asInt) return 1;
	}

	// this (c|sh)ould be optimized by using a hashmap
	for (i = 0; i <= MAX_PAIR; i++) {
		if (self->colors[i].asInt == invalid_color.asInt) {
				self->colors[i] = mycolor;
				if(!console_setcursescolor(self, i, mycolor))
					PDEBUG("setting color failed\n");
				if (i > self->maxcolor) self->maxcolor = i;
				found:
				*which = i;
				PDEBUG("found at: %d\n", i);
				return 1;
		} else if (self->colors[i].asInt == mycolor.asInt)
			goto found;
	}
	return 0; // "could not set color");
}

// sends the right "colorpair" to ncurses
void console_initoutput(struct Console* con) {
	struct NcConsole *self = &con->backend.nc;
	int i;
	if (self->active.fgcol == -1) console_setcolor(con, 1, RGB(0xFF, 0xFF, 0xFF));
	if (self->active.bgcol == -1) console_setcolor(con, 0, RGB(0, 0, 0));
	if(self->lastused.fgcol == self->active.fgcol && self->lastused.bgcol == self->active.bgcol)
		return;

	PDEBUG("initoutput: with fg: %d, bg: %d\n", self->active.fgcol, self->active.bgcol);

	for(i = 0; i <= MAX_PAIR; i++) {
		if(self->pairs[i].fgcol == self->active.fgcol) {
				if (self->pairs[i].bgcol != self->active.bgcol)
					continue;
				else {
					console_usecolorpair(self, i);
					return;
				}
		} else if (self->pairs[i].fgcol == -1) {
				console_setcolorpair(self, i, self->active.fgcol, self->active.bgcol);
				console_usecolorpair(self, i);
				return;
		}
	}
	return; // "colorpair not found");
}

static int console_setcolorpair(struct NcConsole* self, int pair, int fgcol, int bgcol) {
	if(fgcol > MAX_PAIR || bgcol > MAX_PAIR) return 0; // "color pair is out of index");
	if (!self_hasColors(self)) return 0;
	PDEBUG("setcolorpair: %d (fg: %d, bg: %d)\n", pair, fgcol, bgcol);

	self->pairs[pair].fgcol = fgcol;
	self->pairs[pair].bgcol = bgcol;
	return init_pair(pair+MIN_PAIR, fgcol+MIN_COLOR, bgcol+MIN_COLOR) != FALSE;
}

static int console_usecolorpair(struct NcConsole* self, int pair) {
	if(pair > MAX_PAIR) return 0;
	if (!self_hasColors(self)) return 0;
	self->lastused.fgcol = self->active.fgcol;
	self->lastused.bgcol = self->active.bgcol;

	//if (self->lastattr) wattr_off(stdscr,self->lastattr,NULL);
	self->lastattr = COLOR_PAIR(pair + MIN_PAIR);
	color_set(pair + MIN_PAIR, NULL);
	//wattr_on(stdscr, self->lastattr, NULL);
	return 1;
}

void console_getbounds(struct Console* self, int* x, int* y) {
	if(stdscr) {
		getmaxyx(stdscr, self->dim.y, self->dim.x);
		*x = self->dim.x;
		*y = self->dim.y;
	} else {
		*y = -1;
		*x = -1;
	}
}

void console_goto(struct Console* self, int x, int y) {
	move(y, x);
	self->cursor.x = x;
	self->cursor.y = y;
}

// print a char at current location
void console_addchar(struct Console* self, int c, unsigned int attributes) {
	struct NcConsole *con = &self->backend.nc;
	console_initoutput(self);
	waddch(stdscr, c | attributes | con->lastattr);
	//waddch(stdscr, c | attributes);
}

// prints a char and advances cursor
void console_printchar(struct Console* self, int c, unsigned int attributes) {
	int maxy, maxx;
	getmaxyx(stdscr, maxy, maxx);
	(void) maxy;
	int newx = self->cursor.x == maxx ? 1 : self->cursor.x + 1;
	int newy = self->cursor.x == maxx ? self->cursor.y + 1 : self->cursor.y;
	console_addchar(self, c, attributes);
	console_goto(self, newx, newy);
}

void console_putchar(Console* self, int ch, int doupdate) {
	console_addchar(self, ch, 0);
	if(self->automove) console_advance_cursor(self, 1);
	if(doupdate) console_refresh(self);
}

/*
void console_printf (struct Console* con, const char* fmt, ...) {
	console_initoutput(con);
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = vsnprintf(buf, sizeof(buf), fmt, ap);
	(void) result;
	va_end(ap);
	mvprintw(con->cursor.y, con->cursor.x, "%s", buf, 0);
}

void console_printfxy (struct Console* con, int x, int y, const char* fmt, ...) {
	console_initoutput(con);
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	ssize_t result = vsnprintf(buf, sizeof(buf), fmt, ap);
	(void) result;
	va_end(ap);
	mvprintw(y, x, "%s", buf, 0);
}
*/

static int check_modifier_state(mmask_t state) {
	int ret = 0;
	if(state & BUTTON_SHIFT) ret |= CK_MOD_SHIFT;
	if(state & BUTTON_ALT)   ret |= CK_MOD_ALT;
	if(state & BUTTON_CTRL)  ret |= CK_MOD_CTRL;
	return ret;
}

static int translate_event(struct Console *self, int key) {
	int ret = CK_UNDEF;
	if(key == -1) return ret;
	MEVENT mouse_ev;

	switch(key) {
		case KEY_MOUSE:
			if (getmouse(&mouse_ev) == ERR) {
				ret = CK_UNDEF;
				break;
			}
			ret = CK_MOUSE_EVENT;
			self->mouse.coords.x = mouse_ev.x;
			self->mouse.coords.y = mouse_ev.y;
			if(mouse_ev.bstate & BUTTON1_PRESSED ||
			   mouse_ev.bstate & BUTTON2_PRESSED ||
			   mouse_ev.bstate & BUTTON3_PRESSED) {
				self->mouse.mouse_ev = ME_BUTTON_DOWN;
				if     (mouse_ev.bstate & BUTTON1_PRESSED) self->mouse.button = MB_LEFT;
				else if(mouse_ev.bstate & BUTTON2_PRESSED) self->mouse.button = MB_RIGHT;
				else if(mouse_ev.bstate & BUTTON3_PRESSED) self->mouse.button = MB_MIDDLE;
			} else if(mouse_ev.bstate & BUTTON1_RELEASED ||
			          mouse_ev.bstate & BUTTON2_RELEASED ||
			          mouse_ev.bstate & BUTTON3_RELEASED) {
				self->mouse.mouse_ev = ME_BUTTON_UP;
				if     (mouse_ev.bstate & BUTTON1_RELEASED) self->mouse.button = MB_LEFT;
				else if(mouse_ev.bstate & BUTTON2_RELEASED) self->mouse.button = MB_RIGHT;
				else if(mouse_ev.bstate & BUTTON3_RELEASED) self->mouse.button = MB_MIDDLE;
			} else if(mouse_ev.bstate & (1 << 28)) {
				// ncurses 5.7 - 5.9 wrongly reports button up events as BUTTON5_TRIPLE_CLICKED
				// keep current button
				self->mouse.mouse_ev = ME_BUTTON_UP;
			} else {
				self->mouse.mouse_ev = ME_MOVE;
				self->mouse.button = MB_NONE;
			}
			PDEBUG("x: %d, y:%d, button: %d, ev: %d, stage: %ld\n",
			       self->mouse.coords.x, self->mouse.coords.y, self->mouse.button,
			       self->mouse.mouse_ev, (long) mouse_ev.bstate);

			ret |= check_modifier_state(mouse_ev.bstate);
			break;
		case KEY_RESIZE:
			ret = CK_RESIZE_EVENT;
			break;
		case KEY_UP:
			ret = CK_CURSOR_UP;
			break;
		case KEY_DOWN:
			ret = CK_CURSOR_DOWN;
			break;
		case KEY_LEFT:
			ret = CK_CURSOR_LEFT;
			break;
		case KEY_RIGHT:
			ret = CK_CURSOR_RIGHT;
			break;
		case KEY_BACKSPACE: case 127:
			ret = CK_BACKSPACE;
			break;
		default:
			ret = key;
	}
	return ret;
}

#include <strings.h>
#include <sys/ioctl.h>
static void deal_with_resize_signal(void) {
	struct winsize termSize;
	pthread_mutex_lock(&resize_mutex);
	if(ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &termSize) >= 0)
		resizeterm((int)termSize.ws_row, (int)termSize.ws_col);
	refresh();
	pthread_mutex_unlock(&resize_mutex);
}

/* if no input is waiting, the value ERR (-1) is returned */
int console_getkey(struct Console* con) {
	int ret = wgetch(stdscr);
	int res = translate_event(con, ret);
	if(res == CK_RESIZE_EVENT) deal_with_resize_signal();
	PDEBUG("getkey res: %d\n", res);
	return res;
}

int console_getkey_nb(struct Console* con) {
	int ret;
	timeout(0); // set NCURSES getch to nonblocking
	ret = wgetch(stdscr);
	timeout(-1); // set NCURSES getch to blocking
	int res = translate_event(con, ret);
	if(res == CK_RESIZE_EVENT) deal_with_resize_signal();
	return res;
}

void console_sleep(struct Console* con, int ms) {
	(void)con;
	napms(ms);
}
/*The refresh and wrefresh routines (or wnoutrefresh and doupdate)
 * must be called to get actual output to the terminal, 
 * as other routines merely manipulate data structures.*/
void console_refresh(struct Console* con) {
	(void)con;
	refresh();
}

void console_clear(struct Console* con) {
	(void)con;
	clear();
}

void console_blink_cursor(struct Console* self) { (void) self; }
void console_lock(void) {}
void console_unlock(void) {}
void console_toggle_fullscreen(struct Console* self) { (void) self; }

static int get_maxcolors(const char* orgterm) {
	if(!strncmp(orgterm,"rxvt-unicode",12)) return 64;
	return COLORS > 256 ? 256 : COLORS;
}

void console_init_graphics(Console* con, point resolution, font* fnt) {
	(void) resolution; (void) fnt;
	char org_term[64];

	struct NcConsole *self = &con->backend.nc;

	snprintf(org_term, sizeof org_term, "%s", getenv("TERM"));

	if(!strcmp(org_term, "xterm")) {
		setenv("TERM", "xterm-256color", 1);
		self->flags |= NC_SUPPORTSCOLORREADER;
	} else if(!strcmp(org_term, "rxvt-unicode")) {
		setenv("TERM", "rxvt-unicode-256color", 1);
		self->flags |= NC_SUPPORTSCOLORREADER;
	} else if(!strcmp(org_term, "xterm-256color")) {
		self->flags |= NC_SUPPORTSCOLORREADER;
	}


	self->active.fgcol = -1;
	self->active.fgcol = -1;

	self->lastattr = 0;

	console_inittables(con);

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	nonl(); // get return key events

	// the ncurses table is apparently only initialised after initscr() oslt
	ncurses_chartab_init();

#ifdef CONSOLE_DEBUG
	dbg = fopen("console.log", "w");
#endif

	if(!getenv("CONCOL_NO_COLORS")) {
		if (has_colors()) self->flags |= NC_HASCOLORS;
	}
	if (self_hasColors(self) && can_change_color())
		self->flags |= NC_CANCHANGECOLORS;

	if (self_hasColors(self)) start_color();
	self->maxcolors = get_maxcolors(org_term);

	if (self_canChangeColors(self))
		console_savecolors(self);

	if(mousemask(ALL_MOUSE_EVENTS |
		BUTTON1_PRESSED | BUTTON2_PRESSED | BUTTON3_PRESSED |
		BUTTON1_RELEASED | BUTTON2_RELEASED | BUTTON3_RELEASED |
		REPORT_MOUSE_POSITION | BUTTON_SHIFT | BUTTON_ALT | BUTTON_CTRL,
		NULL) != (mmask_t) ERR) {
		mouseinterval(0) /* prevent ncurses from making click events.
		this way we always get an event for buttondown and up.
		we won't get any mouse movement events either way. */;
		self->flags |= NC_HASMOUSE;
	}
	PDEBUG("hasmouse: %d\n", self_hasMouse(self));
	self->lastattr = 0;

	self->maxcolor = 0;

	self->lastused.fgcol = -1;
	self->lastused.bgcol = -1;

	getmaxyx(stdscr, con->dim.y, con->dim.x);
}

void console_settitle(Console *self, const char *title) {
	(void) self;
	(void) title;
}
