#include "sdlconsole.h"
#include "console_keys.h"
#include <SDL/SDL.h>

#ifdef IN_KDEVELOP_PARSER
#define INCLUDED_FROM_SDLCONSOLE
#endif

#ifdef INCLUDED_FROM_SDLCONSOLE

static int sdlkey_to_ascii(Console* self, struct SDL_keysym* key) {
	SDLConsole *c = &self->backend.sdl;
	int shift;

	if(     (key->mod & KMOD_LSHIFT) || (key->mod & KMOD_RSHIFT))
		shift = 1;
	else if((key->mod & KMOD_LALT)   || (key->mod & KMOD_RALT))
		shift = -1; // ALT
	else if((key->mod & KMOD_LCTRL)  || (key->mod & KMOD_RCTRL))
		shift = -2; // CTRL
	else if(key->mod & KMOD_MODE)
		shift = -3; // ALT GR
	else
		shift = 0;

	printf("shift: %d, mod %d, sym %d, name %s\n", shift, key->mod, key->sym, SDL_GetKeyName(key->sym));
	switch(shift) {
		// alt
		case -1:
			if(key->sym == SDLK_RETURN)
				console_toggle_fullscreen(self);
			break;
		//ctrl
		case -2:
			if(c->paintmode)
				switch(key->sym) {
				case SDLK_q:
					return 201;
				case SDLK_a:
					return 204;
				case SDLK_y:
					return 200;
				case SDLK_x:
					return 205;
				case SDLK_s:
					return 206;
				case SDLK_w:
					return 203;
				case SDLK_c:
					return 188;
				case SDLK_d:
					return 185;
				case SDLK_e:
					return 187;

				case SDLK_b:
					return 176;
				case SDLK_n:
					return 177;
				case SDLK_m:
					return 178;

				case SDLK_SPACE:
					return 219;

				default:
					return key->sym;
			} else
				return key->sym;

			break;
		//alt gr
		case -3:
			printf("alt gr pressed!\n");
			switch(key->sym) {
				case SDLK_q:
					return '@';
				case SDLK_7:
					return '{';
				case SDLK_8:
					return '[';
				case SDLK_9:
					return ']';
				case SDLK_0:
					return '}';

				case SDLK_WORLD_63:
					return '\\';
				case SDLK_PLUS:
					return '~';
				case SDLK_LESS:
					return '|';

				default:
					return key->sym;
			}

		case 1:
			switch(key->sym) {
				case SDLK_a:
					return 'A';
				case SDLK_b:
					return 'B';
				case SDLK_c:
					return 'C';
				case SDLK_d:
					return 'D';
				case SDLK_e:
					return 'E';
				case SDLK_f:
					return 'F';
				case SDLK_g:
					return 'G';
				case SDLK_h:
					return 'H';
				case SDLK_i:
					return 'I';
				case SDLK_j:
					return 'J';
				case SDLK_k:
					return 'K';
				case SDLK_l:
					return 'L';
				case SDLK_m:
					return 'M';
				case SDLK_n:
					return 'N';
				case SDLK_o:
					return 'O';
				case SDLK_p:
					return 'P';
				case SDLK_q:
					return 'Q';
				case SDLK_r:
					return 'R';
				case SDLK_s:
					return 'S';
				case SDLK_t:
					return 'T';
				case SDLK_u:
					return 'U';
				case SDLK_v:
					return 'V';
				case SDLK_w:
					return 'W';
				case SDLK_x:
					return 'X';
				case SDLK_y:
					return 'Y';
				case SDLK_z:
					return 'Z';

				case SDLK_1:
					return '!';
				case SDLK_2:
					return '"';
				case SDLK_3:
					return '-';
				case SDLK_4:
					return '$';
				case SDLK_5:
					return '%';
				case SDLK_6:
					return '&';
				case SDLK_7:
					return '/';
				case SDLK_8:
					return '(';
				case SDLK_9:
					return ')';
				case SDLK_0:
					return '=';

				case SDLK_HASH:
					return '\'';

				case SDLK_PLUS:
					return '*';

				case SDLK_LESS:
					return '>';

				case SDLK_WORLD_63:
					return '?';
				case SDLK_PERIOD:
					return ':';
				case SDLK_COMMA:
					return ';';
				case SDLK_MINUS:
					return '_';

				default:
					return key->sym;
			}
		default:
			switch(key->sym) {
				case SDLK_DELETE:
					return CK_DEL;
				case SDLK_BACKSPACE:
					return CK_BACKSPACE;
				case SDLK_RETURN:
					return CK_RETURN;
				case SDLK_a:
					return 'a';
				case SDLK_b:
					return 'b';
				case SDLK_c:
					return 'c';
				case SDLK_d:
					return 'd';
				case SDLK_e:
					return 'e';
				case SDLK_f:
					return 'f';
				case SDLK_g:
					return 'g';
				case SDLK_h:
					return 'h';
				case SDLK_i:
					return 'i';
				case SDLK_j:
					return 'j';
				case SDLK_k:
					return 'k';
				case SDLK_l:
					return 'l';
				case SDLK_m:
					return 'm';
				case SDLK_n:
					return 'n';
				case SDLK_o:
					return 'o';
				case SDLK_p:
					return 'p';
				case SDLK_q:
					return 'q';
				case SDLK_r:
					return 'r';
				case SDLK_s:
					return 's';
				case SDLK_t:
					return 't';
				case SDLK_u:
					return 'u';
				case SDLK_v:
					return 'v';
				case SDLK_w:
					return 'w';
				case SDLK_x:
					return 'x';
				case SDLK_y:
					return 'y';
				case SDLK_z:
					return 'z';
				default:
					return key->sym;
			}
	}
	return 0;
}

#include "colors.h"
static void print_all_fonts(Console* c) {
	size_t i;
	int x, y, w, h;
	console_getbounds(c, &w, &h);
	console_goto(c, 0, 0);
	console_setcolors(c, RGB3(WHITE), RGB3(BLACK));
	console_printf(c, "%.3d %c ", 0, ' ');
	for(i = 1; i < 256; i++) {
		console_getcursor(c, &x, &y);
		if(x + 6 > w)
			console_linebreak(c);
		console_printf(c, "%.3d %c ", (int) i, (int) i);
	}
	console_draw(c);
}

#if 0
static
#endif
int sdlconsole_translate_event(Console* self, SDL_Event* ev) {
	SDLConsole* c = &self->backend.sdl;
	int keymods;
	SDL_Event event = *ev;
	switch (event.type) {
		case SDL_QUIT:
			return CK_QUIT;
		case SDL_KEYDOWN:
			keymods = 0;
			if(event.key.keysym.mod & KMOD_LSHIFT || event.key.keysym.mod & KMOD_RSHIFT) keymods |= CK_MOD_SHIFT;
			if(event.key.keysym.mod & KMOD_LALT   || event.key.keysym.mod & KMOD_RALT  ) keymods |= CK_MOD_ALT;
			if(event.key.keysym.mod & KMOD_LCTRL  || event.key.keysym.mod & KMOD_RCTRL ) keymods |= CK_MOD_CTRL;
			if(event.key.keysym.mod & KMOD_LMETA  || event.key.keysym.mod & KMOD_RMETA ) keymods |= CK_MOD_FLAG;
			if(event.key.keysym.mod & KMOD_MODE) keymods |= CK_MOD_ALTGR;
			/* there's apparently a bug in SDL 1.2.0: when i press 'c', KMOD_NUM is set
			 if(event.key.keysym.mod & KMOD_NUM) keymods |= CK_MOD_NUMERICPAD; */
			switch (event.key.keysym.sym) {
				case SDLK_PAGEUP:
					return keymods | CK_PAGE_UP;
				case SDLK_PAGEDOWN:
					return keymods | CK_PAGE_DOWN;
				case SDLK_UP:
					return keymods | CK_CURSOR_UP;
				case SDLK_DOWN:
					return keymods | CK_CURSOR_DOWN;
				case SDLK_LEFT:
					return keymods | CK_CURSOR_LEFT;
				case SDLK_RIGHT:
					return keymods | CK_CURSOR_RIGHT;
				case SDLK_F1: case SDLK_F2: case SDLK_F3:
				case SDLK_F4: case SDLK_F5: case SDLK_F6:
				case SDLK_F7: case SDLK_F8: case SDLK_F9:
				case SDLK_F10: case SDLK_F11: case SDLK_F12:
					if(event.key.keysym.sym == SDLK_F12) {
						if(keymods & CK_MOD_ALT) {
							print_all_fonts(self);
							return CK_UNDEF;
						}
					}
					return keymods | (CK_F1 + (event.key.keysym.sym - SDLK_F1));
				case SDLK_RSHIFT:
					return keymods | CK_RSHIFT;
				case SDLK_LSHIFT:
					return keymods | CK_LSHIFT;
				//case SDLK_NUMLOCK:
				case SDLK_LCTRL:
					return keymods | CK_LCTRL;
				case SDLK_RCTRL:
					return keymods | CK_RCTRL;
				case SDLK_MODE: //alt gr
					return keymods | CK_ALTGR;
				case SDLK_ESCAPE:
					return keymods | CK_ESCAPE;
				case SDLK_RETURN:
					if(keymods & CK_MOD_ALT) console_toggle_fullscreen(self);
					return keymods | CK_RETURN;
				case SDLK_BACKSPACE:
					return keymods | CK_BACKSPACE;
				case SDLK_DELETE:
					return keymods | CK_DEL;
				case SDLK_HOME:
					return keymods | CK_HOME;
				case SDLK_END:
					return keymods | CK_END;
				case SDLK_KP_PLUS:
					return keymods | CK_PLUS;
				case SDLK_KP_MINUS:
					return keymods | CK_MINUS;
				case SDLK_KP_DIVIDE:
					return keymods | '%';
				case SDLK_KP_MULTIPLY:
					return keymods | '*';
				case SDLK_KP_ENTER:
					return keymods | CK_RETURN;
				case SDLK_KP0: case SDLK_KP1: case SDLK_KP2: case SDLK_KP3:
				case SDLK_KP4: case SDLK_KP5: case SDLK_KP6: case SDLK_KP7:
				case SDLK_KP8: case SDLK_KP9:
					return keymods | ('0' + (event.key.keysym.sym - SDLK_KP0));
				default:
					return keymods | sdlkey_to_ascii(self, &event.key.keysym);
			}
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			self->mouse.mouse_ev = (event.type == SDL_MOUSEBUTTONDOWN) ? ME_BUTTON_DOWN : ME_BUTTON_UP;

			switch(event.button.button) {
				case SDL_BUTTON_MIDDLE:
					self->mouse.button = MB_LEFT;
					break;
				case SDL_BUTTON_RIGHT:
					self->mouse.button = MB_LEFT;
					break;
				case SDL_BUTTON_LEFT:
					self->mouse.button = MB_LEFT;
					break;
				case SDL_BUTTON_WHEELUP:
				case SDL_BUTTON_WHEELDOWN:
					self->mouse.mouse_ev = (event.button.button == SDL_BUTTON_WHEELUP) ? ME_WHEEL_UP : ME_WHEEL_DOWN;
					break;
				default:
					self->mouse.button = MB_NONE;
					break;
			}
			self->mouse.coords.x = event.button.x / c->fnt->dim.x;
			self->mouse.coords.y = event.button.y / c->fnt->dim.y;

			return CK_MOUSE_EVENT;
		case SDL_MOUSEMOTION:
			if(event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) self->mouse.button = MB_LEFT;
			else if(event.motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) self->mouse.button = MB_RIGHT;
			else if(event.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) self->mouse.button = MB_MIDDLE;
			else self->mouse.button = MB_NONE;
			self->mouse.coords.x = event.motion.x / c->fnt->dim.x;
			self->mouse.coords.y = event.motion.y / c->fnt->dim.y;
			self->mouse.mouse_ev = ME_MOVE;
			return CK_MOUSE_EVENT;
		case SDL_VIDEORESIZE:
			console_resize(self, event.resize.w, event.resize.h);
			return CK_RESIZE_EVENT;
		default:
			break;
	}
	return CK_UNDEF;
}

#endif
