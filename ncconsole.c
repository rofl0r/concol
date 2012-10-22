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

#include "ncconsole_chartab.c"

#define MIN_COLORPAIR_NUMBER 1
#define MIN_COLOR_NUMBER 0

//#define CONSOLE_DEBUG
static rgb_t invalid_color = RGB(0,0,0);

#ifdef CONSOLE_DEBUG	
static FILE* dbg = NULL;
#endif

static void console_savecolors(struct NcConsole* self);
static void console_restorecolors(struct NcConsole* self);
static int console_setcursescolor(struct NcConsole* self, int colornumber, rgb_t color);
static int console_setcolorpair(struct NcConsole* self, int pair, int fgcol, int bgcol);
static int console_usecolorpair(struct NcConsole* self, int pair);

#include <pthread.h>
//RcB: LINK "-lpthread"

static pthread_mutex_t resize_mutex;
static int got_resize_signal = 0;

/* sigwinch signal handler */
static void resized(int dummy) {
	(void) dummy;
	pthread_mutex_lock(&resize_mutex);
	got_resize_signal = 1;
	pthread_mutex_unlock(&resize_mutex);
}

static inline int console_fromthousand(int in) {
	return in == 0 ? 0 : in == 1000 ? 255 : (in * 1000 * 1000) / 3921568;
}

static inline int console_tothousand(int in) {
	return in == 0 ? 0 : in == 255 ? 1000 : (in * 3921568) / (1000 * 1000);
}

static inline void console_inittables(struct Console* self) {
	struct NcConsole *con = &self->backend.nc;
	int i;
	for (i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		con->colors[i] = invalid_color;
		con->termPairs[i].fgcol = -1;
		con->termPairs[i].bgcol = -1;
	}
}

void console_init(struct Console* con) {
	memset(con, 0, sizeof(struct Console));
	con->backendtype = cb_ncurses;
	pthread_mutex_init(&resize_mutex, NULL);

	struct NcConsole *self = &con->backend.nc;
	
	strncpy(self->org_term, getenv("TERM"), sizeof(self->org_term));
	if(memcmp(self->org_term, "xterm", 5) == 0) setenv("TERM", "xterm-256color", 1);
	invalid_color.a = 255;
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

	
	self->hasColors = has_colors();
	self->canChangeColors = self->hasColors ? can_change_color() : 0;
	if (self->hasColors) start_color();

	if (self->canChangeColors)
		console_savecolors(self);
	
	if((self->hasMouse = (mousemask(ALL_MOUSE_EVENTS |  
		BUTTON1_PRESSED | BUTTON2_PRESSED | BUTTON3_PRESSED |
		BUTTON1_RELEASED | BUTTON2_RELEASED | BUTTON3_RELEASED |
		REPORT_MOUSE_POSITION | BUTTON_SHIFT | BUTTON_ALT | BUTTON_CTRL,
		NULL) != (mmask_t) ERR))) 
		mouseinterval(0) /* prevent ncurses from making click events.
		this way we always get an event for buttondown and up. 
		we won't get any mouse movement events either way. */;
#ifdef CONSOLE_DEBUG
	fprintf(dbg, "hasmouse: %d\n", (int) self->hasMouse);
#endif
	self->lastattr = 0;
	
	self->maxcolor = 0;
	
	self->lastused.fgcol = -1;
	self->lastused.bgcol = -1;
	
	con->dim.x = stdscr->_maxx + 1;
	con->dim.y = stdscr->_maxy + 1;
	
	signal(SIGWINCH, resized);
}

void console_cleanup(struct Console* con) {
	struct NcConsole *self = &con->backend.nc;
	clear();
	refresh();
	if (self->canChangeColors) console_restorecolors(self);
	endwin();
#ifdef CONSOLE_DEBUG
	fclose(dbg);
#endif
	setenv("TERM", self->org_term, 1);
	pthread_mutex_destroy(&resize_mutex);
}

static void console_savecolors(struct NcConsole *self) {
	short int i;
	short int r,g,b;
	short int fg, bg;
	for (i = MIN_COLOR_NUMBER; i < CONSOLE_COLORPAIRCOUNT; i++) {
		color_content(i, &r, &g, &b);
		self->org_colors[i] = RGB(console_fromthousand(r), console_fromthousand(g), console_fromthousand(b));
	}
	for (i = MIN_COLORPAIR_NUMBER; i < CONSOLE_COLORPAIRCOUNT; i++) {
		pair_content(i, &fg, &bg);
		self->org_fgcolors[i] = fg;
		self->org_bgcolors[i] = bg;
	}
}

static void console_restorecolors(struct NcConsole *self) {
	int i;
	for (i = MIN_COLOR_NUMBER; i <= self->maxcolor; i++) {
		init_color(i, 
			console_tothousand(self->org_colors[i].r), 
			console_tothousand(self->org_colors[i].g),
			console_tothousand(self->org_colors[i].b)
		);
	}
	for (i = MIN_COLORPAIR_NUMBER; i < self->maxcolor + MIN_COLORPAIR_NUMBER; i++) {
		init_pair(i, self->org_fgcolors[i], self->org_fgcolors[i]);
	}
}

// needs color additionally to be used by restorecolors
static int console_setcursescolor(struct NcConsole* self, int colornumber, rgb_t color) {
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcursescolor: %d (%d, %d, %d)\n", colornumber, color.r, color.g, color.b);
#endif
	
	if(colornumber >= CONSOLE_COLORPAIRCOUNT) return 0;
	
	// we use rgb values in the range 0-0xFF, while ncurses max is 1000
	if(!self->canChangeColors) return 0;

	int nr = console_tothousand(color.r);
	int ng = console_tothousand(color.g);
	int nb = console_tothousand(color.b);
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "init_color: %d (%d, %d, %d)\n", colornumber+1, nr, ng, nb);
#endif	
	
	return init_color(colornumber+MIN_COLOR_NUMBER, nr, ng, nb) != FALSE;
}

int console_setcolor(struct Console* con, int is_fg, rgb_t mycolor) {
	struct NcConsole *self = &con->backend.nc;
	int i;
	int* which = is_fg ? &self->active.fgcol : &self->active.bgcol;
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcolor: (%d, %d, %d), fg: %d\n", mycolor.r, mycolor.g, mycolor.b, is_fg);
#endif
	
	
	// see if it's the actual color...
	if (*which >= 0) {
		if (self->colors[*which].asInt == mycolor.asInt) return 1;
	}

	// this (c|sh)ould be optimized by using a hashmap
	for (i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		if (self->colors[i].asInt == invalid_color.asInt) {
				self->colors[i] = mycolor;
				console_setcursescolor(self, i, mycolor);
				if (i > self->maxcolor) self->maxcolor = i;
				found:
				*which = i;
#ifdef CONSOLE_DEBUG
				if(dbg) fprintf(dbg, "found at: %d\n", i);
#endif
				
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
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "initoutput: with fg: %d, bg: %d\n", self->active.fgcol, self->active.bgcol);
#endif
	
	for(i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		if(self->termPairs[i].fgcol == self->active.fgcol) {
				if (self->termPairs[i].bgcol != self->active.bgcol)
					continue;
				else {
					console_usecolorpair(self, i);
					return;
				}
		} else if (self->termPairs[i].fgcol == -1) {
				console_setcolorpair(self, i, self->active.fgcol, self->active.bgcol);
				console_usecolorpair(self, i);
				return;
		}
	}
	return; // "colorpair not found");
}

static int console_setcolorpair(struct NcConsole* self, int pair, int fgcol, int bgcol) {
	if(fgcol >= CONSOLE_COLORPAIRCOUNT || bgcol >= CONSOLE_COLORPAIRCOUNT) return 0; // "color pair is out of index");
	if (!self->hasColors) return 0;
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcolorpair: %d (fg: %d, bg: %d)\n", pair, fgcol, bgcol);
#endif
	
	self->termPairs[pair].fgcol = fgcol;
	self->termPairs[pair].bgcol = bgcol;
	return init_pair(pair+MIN_COLORPAIR_NUMBER, fgcol+MIN_COLOR_NUMBER, bgcol+MIN_COLOR_NUMBER) != FALSE;
}

static int console_usecolorpair(struct NcConsole* self, int pair) {
	if(pair >= CONSOLE_COLORPAIRCOUNT) return 0;
	if (!self->hasColors) return 0;
	self->lastused.fgcol = self->active.fgcol;
	self->lastused.bgcol = self->active.bgcol;
	
	//if (self->lastattr) wattr_off(stdscr,self->lastattr,NULL);
	self->lastattr = COLOR_PAIR(pair + MIN_COLORPAIR_NUMBER);
	color_set(pair + MIN_COLORPAIR_NUMBER, NULL);
	//wattr_on(stdscr, self->lastattr, NULL);
	return 1;
}

void console_getbounds(struct Console* self, int* x, int* y) {
	if(stdscr) {
		self->dim.x = *x = stdscr->_maxx + 1;
		self->dim.y = *y = stdscr->_maxy + 1;
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
	int newx = self->cursor.x == stdscr->_maxx ? 1 : self->cursor.x + 1;
	int newy = self->cursor.x == stdscr->_maxx ? self->cursor.y + 1 : self->cursor.y;
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
#ifdef CONSOLE_DEBUG
			fprintf(dbg, "x: %d, y:%d, button: %d, ev: %d, stage: %ld\n", 
			       self->mouse.coords.x, self->mouse.coords.y, self->mouse.button,
			       self->mouse.mouse_ev, (long) mouse_ev.bstate);
#endif
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
			
		default:
			ret = key;
	}
	return ret;
}

#include <stropts.h>
#include <sys/ioctl.h>
static int deal_with_resize_signal(void) {
	struct winsize termSize;
	int ret = 0;
	pthread_mutex_lock(&resize_mutex);
	if(got_resize_signal) {
		if(ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &termSize) >= 0)
			resize_term((int)termSize.ws_row, (int)termSize.ws_col);
		ret = 1;
		got_resize_signal = 0;
	}
	pthread_mutex_unlock(&resize_mutex);
	return ret;
}

/* if no input is waiting, the value ERR (-1) is returned */
int console_getkey(struct Console* con) {
	if(deal_with_resize_signal()) return CK_RESIZE_EVENT;
	int ret = wgetch(stdscr);
	int res = translate_event(con, ret);
#ifdef CONSOLE_DEBUG
	fprintf(dbg, "getkey res: %d\n", res);
#endif
	return res;
}

int console_getkey_nb(struct Console* con) {
	if(deal_with_resize_signal()) return CK_RESIZE_EVENT;
	int ret;
	timeout(0); // set NCURSES getch to nonblocking
	ret = wgetch(stdscr);
	timeout(-1); // set NCURSES getch to blocking
	return translate_event(con, ret);
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
void console_init_graphics(Console* self, point resolution, bitfont* fnt) {(void) self; (void) resolution; (void) fnt;}

