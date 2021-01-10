#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "../console.h"
#include "../console_keys.h"
#include "../colors.h"

//#define CONSOLE_FONT TESTFONT
#define CONSOLE_FONT INT10FONT16
#include "../fonts/allfonts.h"

void clrscr(Console *c) {
	int w, h, x, y;
	console_getbounds(c, &w, &h);
	//console_setcolors(c, RGB(200, 80, 200), RGB(80, 80, 80));
	console_setcolors(c, RGB3(BLACK), RGB3(ORANGE));
	for(y=0; y<h; y++)
	for(x = 0; x < w; x++) {
		console_goto(c, x, y);
		console_addchar(c, ' ', 0); // using ' ' as fill character will effectively paint a square in background color.
	}
	console_setcolors(c, RGB(80, 80, 80), RGB(255, 255, 255));
}

int prices[500];
int currprice, maxprice;
int given; int givenmode;

char curr[512]; int currx;

void resetprices() {
	int i = 0;
	for(;i<sizeof(prices)/sizeof(prices[0]);i++) {
		prices[i] = 0;
	}
	currprice = 0;
	maxprice = 0;
	currx = 0;
	curr[0] = 0;
	given = 0;
	givenmode = 0;
}

void print_price(Console *c, int price) {
	int mp=price>0?price:-price,
	    minus=price<0;
	console_printf(c, "%s%6d.%.2d      ", minus?"-":"", mp/100, mp%100);
}
void set_hl_color(Console *c){
	console_setcolors(c, RGB3(BLACK), RGB3(YELLOW));
}
void set_normal_color(Console *c){
	console_setcolors(c, RGB3(BLACK), RGB3(WHITE));
}

void draw(Console *c) {
	int y;
	for(y=0;y<23;y++) {
		console_goto(c, 5, y);
		if(!givenmode && y == currprice)
			set_hl_color(c);
		else
			set_normal_color(c);

		if(y<=maxprice) print_price(c, prices[y]);
	}
	int sumloc = 25;
	console_goto(c, 5, sumloc);
	int i, sum = 0;
	for (i=0;i<=maxprice;i++) sum+=prices[i];
	print_price(c, sum);

	if(given||givenmode) {
		console_goto(c, 5, sumloc+1);
		if(givenmode)set_hl_color(c);
		else set_normal_color(c);
		print_price(c, given);
		set_normal_color(c);
		console_goto(c, 5, sumloc+2);
		print_price(c, given-sum);
	}

	console_refresh(c);
}

int conv(char* s) {
	int f = 0;
	while(*s) {
		if(*s>='0'&&*s<='9')
			f=f*10+(*s-'0');
		s++;
	}
	return f;
}

void resetcurr() {
	currx = 0;
	curr[currx] = 0;
}

void eval_current() {
	if(!givenmode)prices[currprice] = conv(curr);
	else given=conv(curr);
}

void process(int k) {
	switch(k) {
	case CK_ESCAPE:
		resetprices();
		return;
	case '\n': case '\r':
		if(givenmode) return;
		prices[currprice] = conv(curr);
		if(!prices[currprice]) return;
		currprice++;
		if(currprice>maxprice){
			maxprice=currprice;
			prices[currprice] = 0;
			resetcurr();
		} else {
			currx=snprintf(curr, sizeof(curr), "%d", prices[currprice]);
		}
		return;
	case CK_BACKSPACE: /* backspace */
		currx--;
		if(currx<0)currx=0;
		curr[currx] = 0;
		eval_current();
		return;
	case ' ':
		if(!givenmode) currx=snprintf(curr, sizeof(curr), "%d", given);
		givenmode = !givenmode;
		if(!givenmode) resetcurr();
		return;
	case CK_CURSOR_UP: case CK_CURSOR_DOWN:
		if(givenmode) return;
		int dir = k==CK_CURSOR_UP?-1:1;
		if(currprice+dir<=maxprice&&currprice+dir>=0) {
			currprice+=dir;
			currx=snprintf(curr, sizeof(curr), "%d", prices[currprice]);
		}
		return;
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		if(strlen(curr)>6) return;
		curr[currx++] = k;
		curr[currx] = 0;
		eval_current();
		return;
	}
}

static int get_next_key(struct Console *c) {
	int k;
	while ((k = console_getkey(c)) == CK_MOUSE_EVENT) ;
	return k;
}


int main(void) {
	struct Console co;
	struct Console* c = &co;
	int w, h, x, y = 0;
	console_init(c);
	if(getenv("CONSOLE_DEBUG"))
		kill(getpid(), SIGSTOP);

	point reso = {640, 480};
	console_init_graphics(c, reso, FONT);

	resetprices();
	clrscr(c);

	for(;;) {
		console_refresh(c);
		draw(c);
		int k;
		k = get_next_key(c);
		if(k==CK_QUIT || k == 'q') break;
		dprintf(666, "%d\n", k);
		process(k);
		if(k==CK_ESCAPE) clrscr(c);
	}

	console_clear(c);
	console_cleanup(c);
	return 0;
}
