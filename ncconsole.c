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

#include "ncconsole.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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



static inline int console_fromthousand(int in) {
	return in == 0 ? 0 : in == 1000 ? 255 : (in * 1000 * 1000) / 3921568;
}

static inline int console_tothousand(int in) {
	return in == 0 ? 0 : in == 255 ? 1000 : (in * 3921568) / (1000 * 1000);
}

static inline void console_inittables(struct Console* con) {
	struct NcConsole *self = (struct NcConsole*) con;
	int i;
	for (i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		self->colors[i] = invalid_color;
		self->termPairs[i].fgcol = -1;
		self->termPairs[i].bgcol = -1;
	}
}

void console_init(struct Console* con) {
	struct NcConsole *self = (struct NcConsole*) con;
	strncpy(self->org_term, getenv("TERM"), sizeof(self->org_term));
	setenv("TERM", "xterm-256color", 1);
	invalid_color.a = 255;
	self->active.fgcol = -1;
	self->active.fgcol = -1;
	
	self->lastattr = 0;
	
	console_inittables(con);
	
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	
	self->hasColors = has_colors();
	self->canChangeColors = self->hasColors ? can_change_color() : 0;
	if (self->hasColors) start_color();

	if (self->canChangeColors)
		console_savecolors(self);
	
	self->lastattr = 0;
	
	self->maxcolor = 0;
	
	self->lastused.fgcol = -1;
	self->lastused.bgcol = -1;
	
#ifdef CONSOLE_DEBUG	
	dbg = fopen("console.log", "w");
#endif	
	
}

void console_quit(struct Console* con) {
	struct NcConsole *self = (struct NcConsole*) con;
	clear();
	refresh();
	if (self->canChangeColors) console_restorecolors(self);
	endwin();
#ifdef CONSOLE_DEBUG
	fclose(dbg);
#endif
	setenv("TERM", self->org_term, 1);
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
	struct NcConsole *self = (struct NcConsole*) con;
	int i;
	int* which = is_fg ? &self->active.fgcol : &self->active.bgcol;
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcolor: (%d, %d, %d), fg: %d\n", mycolor.r, mycolor.g, mycolor.b, fg);
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

int console_setcolors(struct Console* self, rgb_t bgcolor, rgb_t fgcolor) {
	return
		console_setcolor(self, 0, bgcolor) +
		console_setcolor(self, 1, fgcolor);
}

// sends the right "colorpair" to ncurses
void console_initoutput(struct Console* con) {
	struct NcConsole *self = (struct NcConsole*) con;
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

static int console_usecolorpair(struct NcConsole* con, int pair) {
	struct NcConsole *self = (struct NcConsole*) con;
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

void console_getsize(struct Console* con, int* x, int* y){
	(void)con;
	if(stdscr) {
		*x = stdscr->_maxx + 1;
		*y = stdscr->_maxy + 1;
	} else { *y = -1; *x = -1; }
}

void console_goto(struct Console* con, int x, int y) {
	struct NcConsole *self = (struct NcConsole*) con;
	move(y, x);
	self->super.cursor.x = x;
	self->super.cursor.y = y;
}

// print a char at current location
void console_addchar(struct Console* con, int c, unsigned int attributes) {
	struct NcConsole *self = (struct NcConsole*) con;
	console_initoutput(con);
	waddch(stdscr, c | attributes | self->lastattr);
	//waddch(stdscr, c | attributes);
}

// prints a char and advances cursor
void console_printchar(struct Console* con, int c, unsigned int attributes) {
	struct NcConsole *self = (struct NcConsole*) con;
	int newx = self->super.cursor.x == stdscr->_maxx ? 1 : self->super.cursor.x + 1;
	//FIXME this looks like a copy paste error
	int newy = self->super.cursor.x == stdscr->_maxx ? self->super.cursor.y + 1 : self->super.cursor.y;
	console_addchar(con, c, attributes);
	console_goto(con, newx, newy);
}


void console_printxy (struct Console* con, int x, int y, char* text) {
	console_initoutput(con);
	mvprintw(x, y, "%s", text, 0);
}

int console_getkey(struct Console* con) {
	(void)con;
	return wgetch(stdscr);
}

void console_sleep(struct Console* con, int ms) {
	(void)con;
	napms(ms);
}

void console_refresh(struct Console* con) {
	(void)con;
	refresh();
}

void console_clear(struct Console* con) {
	(void)con;
	clear();
}