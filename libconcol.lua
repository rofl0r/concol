-- concol LuaJIT FFI binding
-- Copyright (C) 2012 rofl0r

local ffi = require("ffi")

ffi.cdef[[
typedef struct {
	int x;
	int y;
} point;

typedef struct {
	point topleft;
	point bottomright;
} rect;

typedef enum {
	EV_MOUSE = 0,
	EV_KEY_DOWN = 1,
	EV_TIMER = 2,
	EV_RESIZE = 3,
	EV_COUNT = 4, // the number of total event types
} event_type;

typedef enum {
	MB_NONE = -1,
	MB_LEFT = 0,
	MB_RIGHT = 1,
	MB_MIDDLE = 2,
	MB_COUNT = 3
} mouse_button;

typedef enum {
	ME_BUTTON_DOWN,
	ME_BUTTON_UP,
	ME_WHEEL_DOWN,
	ME_WHEEL_UP,
	ME_MOVE,
} mouse_event_type;

typedef struct {
	point coords;
	mouse_event_type mouse_ev;
	mouse_button button;
} mouse_event;

typedef struct {
	unsigned long counter;
	int active:1; // the timer event is sent to all "subscribed" windows. 
		      //this is to make them aware if the window is active or not.
} timer_event;

typedef struct {
	rect newcoords;
} resize_event;

typedef enum {
	MOD_SHIFT = 1 << 0,
	MOD_ALT   = 1 << 1,
	MOD_CTRL  = 1 << 2,
	MOD_ALTGR = 1 << 3,
	MOD_FLAG  = 1 << 4,
} key_modifier;

typedef struct {
	int ch;
	key_modifier modifiers;
} key_event;

// this struct requires std=gnu89 or gnu99 to work. i am to lazy to make the union non-anonymous.
typedef struct {
	event_type etype;
	union {
		mouse_event mouse_ev;
		timer_event timer_ev;
		key_event key_ev;
		resize_event resize_ev;
	};
} event;

typedef union {
	struct {
		unsigned char a;
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};
	uint32_t asInt;
} rgb_t;

typedef struct {
	rgb_t bgcolor;
	rgb_t fgcolor;
} rgb_tuple;

typedef union {
	struct colors {
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
	} colors;
	uint32_t val;
} sdl_rgb_t;

typedef struct {
	sdl_rgb_t bgcolor;
	sdl_rgb_t fgcolor;
} sdl_rgb_tuple;

typedef struct {
	point dim;
	char* characters[256];
} font;

typedef struct SDLConsole {
	void *surface;
	sdl_rgb_tuple color;
	point res;
	font* fnt;
	int paintmode:1;
	int cursorblink:1;
	int fullscreen:1;
} SDLConsole;

typedef struct TbConsole {
	int fgcolor;
	int bgcolor;
} TbConsole;

typedef struct {
	int fgcol;
	int bgcol;
} Colorpair;

typedef struct NcConsole {
	char org_term[32];
	int hasColors:1;
	int canChangeColors:1;
	int hasMouse:1;
	int maxcolor;

	//attr_t lastattr;
	unsigned int lastattr;
	Colorpair lastused;
	Colorpair active;
	Colorpair termPairs[256];
	rgb_t colors[256];
	
	rgb_t org_colors[256];
	short int org_fgcolors[256];
	short int org_bgcolors[256];
} NcConsole;

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

void console_init(struct Console* self);
void console_cleanup(struct Console* self);
int console_setcolor(struct Console* self, int is_fg, rgb_t mycolor);
int console_setcolors(struct Console* self, rgb_t bgcolor, rgb_t fgcolor);
void console_initoutput(struct Console* self);
void console_getbounds(struct Console* self, int* width, int* height);
void console_getcursor(struct Console* self, int* x, int* y);
void console_goto(struct Console* self, int x, int y);
/* prints a char and NOT advances cursor */
void console_addchar(struct Console* self, int c, unsigned int attributes);
/* prints a char and advances cursor */
void console_printchar(struct Console* self, int c, unsigned int attributes);
/* prints a char and updates (redraws) the screen when doupdate is 1. advances cursor if automove is set. */
void console_putchar(Console* self, int ch, int doupdate);
void console_printf (struct Console* con, const char* fmt, ...);
/* blocking */
int console_getkey(struct Console* self);
/* non-blocking */
int console_getkey_nb(struct Console* self);
void console_sleep(struct Console* self, int ms);
void console_refresh(struct Console* self);
void console_clear(struct Console* self);
void console_lock(void);
void console_unlock(void);
void console_blink_cursor(struct Console* self);
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
void console_toggle_fullscreen(Console *c);
void console_init_graphics(Console* self, point resolution, font* fnt);

extern font testfont;

]]

local N = {} -- exported functions and types

local concol_n = ffi.load("./libconcol256.so") -- libnessdb.so namespace
N.buffer_t = ffi.typeof("char[?]") -- char buffer type
N.console_t = ffi.typeof("struct Console")
N.font_t = ffi.typeof("font")
N.console = ffi.new("struct Console")
N.rgb_t = ffi.typeof("rgb_t")



function N.openlib(lib_path)
	concol_n = ffi.load(lib_path)
end

function N.init()
	concol_n.console_init(N.console)
end

function N.init_graphics(xres, yres)
	local pt = ffi.new("point");
	local fnt = ffi.cast("font *", N.testfont)
	pt.x = xres
	pt.y = yres
	concol_n.console_init_graphics(N.console, pt, fnt)
end


function N.cleanup()
	concol_n.console_cleanup(N.console)
end

function N.getkey()
	return concol_n.console_getkey(N.console)
end

function N.goto(x, y)
	concol_n.console_goto(N.console, x, y)
end

function N.putchar(ch, doupdate)
	concol_n.console_putchar(N.console, string.byte(ch), doupdate)
end

function N.rgb(r, g, b)
	local col
	col = N.rgb_t(0, b, g, r)
	return col
end

function N.setcolor(isfg, col)
	concol_n.console_setcolor(N.console, isfg, col)
end
	

return N