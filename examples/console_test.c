#include <stdlib.h>
#include "../ncconsole.h"
#include "../ncconsole_chars.h"
#include "../colors.h"

int main(void) {
	static const char japh[] = "just another perl/unix hacker";
	static const size_t jl = sizeof(japh) - 1;
	struct NcConsole co;
	struct Console* c = &co.super;
	int w, h, x, y = 0;
	console_init(c);
	console_getbounds(c, &w, &h);
	//console_setcolors(c, RGB(200, 80, 200), RGB(80, 80, 80));
	console_setcolors(c, RGB3(BLACK), RGB3(ORANGE));

	for(x = 0; x < w; x++) {
		console_goto(c, x, y);
		console_addchar(c, ' ', 0); // using ' ' as fill character will effectively paint a square in background color.
	}
	console_setcolors(c, RGB(80, 80, 80), RGB(255, 255, 255));

	for(y = 1; y < h - 1; y++) {
		for(x = 0; x < w; x++) {
			console_goto(c, x, y);
			console_addchar(c, CC_CKBOARD, 0);
		}
	}
	y = h - 1;
	console_setcolors(c, RGB(200, 80, 200), RGB(80, 80, 80));
	for(x = 0; x < w; x++) {
		console_goto(c, x, y);
		console_addchar(c, ' ', 0);
	}

	console_setcolors(c, RGB(0, 0, 200), RGB(255, 255, 80));
	
	y = 15;

	console_goto(c, 20, y++);
	console_printchar(c, CC_ULCORNER, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_URCORNER, 0);
	
	console_goto(c, 20, y++);
	console_printchar(c, CC_VLINE, 0);
	for(x=0; x<8; x++)
		console_printchar(c, ' ', 0);
	console_printchar(c, CC_VLINE, 0);
	
	console_goto(c, 20, y++);
	console_printchar(c, CC_LLCORNER, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_HLINE, 0);console_printchar(c, CC_HLINE, 0);
	console_printchar(c, CC_LRCORNER, 0);
	y++;
	console_goto(c, 20, y++);
	console_printchar(c, CC_RTEE, 0);console_printchar(c, CC_LTEE, 0);
	console_printchar(c, CC_BTEE, 0);console_printchar(c, CC_TTEE, 0);
	console_printchar(c, CC_PLUS, 0);console_printchar(c, CC_S1, 0);
	console_printchar(c, CC_S9, 0);console_printchar(c, CC_DIAMOND, 0);
	
	console_goto(c, 20, y++);
	console_printchar(c, CC_CKBOARD, 0);console_printchar(c, CC_DEGREE, 0);
	console_printchar(c, CC_PLMINUS, 0);console_printchar(c, CC_BULLET, 0);
	console_printchar(c, CC_LARROW, 0);console_printchar(c, CC_RARROW, 0);
	console_printchar(c, CC_DARROW, 0);console_printchar(c, CC_UARROW, 0);

	console_goto(c, 20, y++);
	console_printchar(c, CC_BOARD, 0);console_printchar(c, CC_LANTERN, 0);
	console_printchar(c, CC_BLOCK, 0);
	console_refresh(c);
	console_goto(c, 20, y++);
	for(x = 0; x < jl; x++) {
		delay_output(rand() % 200 + 80);
		console_printchar(c, japh[x], 0);
		console_refresh(c);
	}
	
	console_refresh(c);
	
	console_getkey(c);
	console_clear(c);
	console_cleanup(c);
	return 0;
}
