/*
CFLAGS="-Wall -DCONSOLE_BACKEND=SDL_CONSOLE" rcb --new --force --debug plasma.c
CFLAGS="-Wall -DCONSOLE_BACKEND=NCURSES_CONSOLE" rcb --new --force --debug plasma.c
CFLAGS="-Wall -DCONSOLE_BACKEND=TERMBOX_CONSOLE -L ../../termbox" rcb --new --force --debug plasma.c
*/

//RcB: LINK "-lm"
#include "../console.h"
#include "../console_keys.h"
#define CONSOLE_FONT TESTFONT
#include "../fonts/allfonts.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define _GNU_SOURCE
#include <math.h>

// http://www.bidouille.org/prog/plasma
typedef float (*transformcb)(float time, float x, float y);
static float sinval1(float time, float x, float y) {
	(void) y;
	return  sinf(x*10+time);
}
static float sinval2(float time, float x, float y) {
	return  sinf(10 * (x * sinf(time/2) + y*cosf(time/3)) + time);
}
static float sinval3(float time, float x, float y) {
	float cx=x+0.5*sinf(time/5.0);
	float cy=y+0.5*cosf(time/3.0);
	return sinf(sqrtf(100*(cx*cx+cy*cy)+1) + time);
}
static float sinval4(float time, float x, float y) {
	return sinf((y*10+time)/2.0);
}
static float sinval5(float time, float x, float y) {
	return sinf((x*10+y*10+time)/2.0);
}
static float sinvalc3(float time, float x, float y) {
	float v1 = sinval1(time, x, y);
	float v2 = sinval2(time, x, y);
	float v3 = sinval3(time, x, y);
	return (v1+v2+v3)/2.0;
}
static float sinvalc4(float time, float x, float y) {
	float v1 = sinval1(time, x, y);
	float v2 = sinval4(time, x, y);
	float v3 = sinval5(time, x, y);
	float v4 = sinval3(time, x, y);
	return (v1+v2+v3+v4)/2.0;
}

static unsigned float_to_col(float c) {
	unsigned r = 128 + (c*127);
	if(c > 1 || c < -1)
		assert(!(r&0xffffff00));
	return r;
}
static unsigned floats_to_col(float r, float g, float b) {
	return (float_to_col(b) << 24) | (float_to_col(g) << 16) | (float_to_col(r) << 8);
}
typedef unsigned (*colorcb)(float v);
static unsigned color1(float v) {
	float r=sinf(v*M_PI);
	float g=cosf(v*M_PI);
	float b = -1;
	return floats_to_col(r,g,b);
}
static unsigned color2(float v) {
	float r = 1;
	float g = cosf(v*M_PI);
	float b = sinf(v*M_PI);
	return floats_to_col(r,g,b);
}
static unsigned color3(float v) {
	float r = sinf(v*M_PI);
	float g = sinf(v*M_PI+2*M_PI/3);
	float b = sinf(v*M_PI+4*M_PI/3);
	return floats_to_col(r,g,b);
}
static unsigned color4(float v) {
	float c = sinf(M_PI*v*5);
	return floats_to_col(c,c,c);
}
static unsigned color5(float v) {
	float c = .5*v*.8;
	return floats_to_col(c,c,c);
}

static unsigned char pal_idx(float f) {
	return 128+f*127.f;
}

typedef struct bmp4 {
	unsigned width, height;
	unsigned *data;
} bmp4;

static void plasma3(bmp4 *dst, float *time, transformcb transform, unsigned pal[256]) {
	unsigned h,w,x,y,*d = dst->data;
	float yy, xx, kx;
	h = dst->height;
	w = dst->width;
	kx = (float)w/h;
	for(y=0; y<h; y++) {
		yy = (float)y/h - 0.5;
		for(x=0; x<w; x++) {
			xx = kx*x/w-kx/2.0;
			float v = transform(*time, xx, yy);
			*(d++) = pal[pal_idx(v)];
		}
	}
	*time += 0.039;
}

static inline bmp4* bmp4_new(unsigned width, unsigned height) {
	bmp4* res = malloc((sizeof *res)+(width*height*sizeof(*res->data)));
	res->width=width;
	res->height=height;
	res->data=(void*)(res+1);
	return res;
}

static void draw(Console *t, bmp4* b) {
	int x,y;
	unsigned *p = b->data;
	for(y=0;y<b->height;y++) for(x=0;x<b->width;x++) {
		console_setcolor(t, 0, *((rgb_t*) p));
		console_goto(t, x, y);
		console_addchar(t, ' ', 0);
		p++;
	}
	console_refresh(t);
}

unsigned pal[256];
void gen_pal(colorcb color) {
	int i;
	for(i=0;i<256;i++) pal[i] = color((float)(i - 128)/128.0f);
}

int main() {
	Console co;
	Console* t = &co;
	struct {int w, h;} dim;
	console_init(t);
	point reso = {800, 600};
	console_init_graphics(&co, reso, FONT);

	console_getbounds(t, &dim.w, &dim.h);

	bmp4* r  = bmp4_new(dim.w, dim.h);
	gen_pal(color3);
	float time = getpid();
	int k;
	while((k = console_getkey_nb(t)) == CK_UNDEF || !(k & CK_MASK)) {
		plasma3(r, &time, sinvalc4, pal);
		draw(t, r);
		console_sleep(t, 30);
	}
	console_cleanup(t);
	printf("got %d\n", k);
}

