#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "../console.h"
#include "../colors.h"

//#define CONSOLE_FONT TESTFONT
#define CONSOLE_FONT INT10FONT08
#include "../fonts/allfonts.h"

int main(void) {
	static const char japh[] = "just another perl/unix hacker";
	static const size_t jl = sizeof(japh) - 1;
	struct Console co;
	struct Console* c = &co;
	int w, h, x, y = 0;
	console_init(c);
	if(getenv("CONSOLE_DEBUG"))
		kill(getpid(), SIGSTOP);

	point reso = {800, 600};
	console_init_graphics(&co, reso, FONT);
	
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
			console_addchar(c, CCT(cc_ckboard), 0);
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
	console_printchar(c, CCT(cc_ulcorner), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_urcorner), 0);
	
	console_goto(c, 20, y++);
	console_printchar(c, CCT(cc_vline), 0);
	for(x=0; x<8; x++)
		console_printchar(c, ' ', 0);
	console_printchar(c, CCT(cc_vline), 0);
	
	
	console_goto(c, 20, y++);
	console_printchar(c, CCT(cc_llcorner), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_hline), 0);console_printchar(c, CCT(cc_hline), 0);
	console_printchar(c, CCT(cc_lrcorner), 0);
	y++;
	
	console_goto(c, 20, y++);
	console_printchar(c, CCT(cc_rtee), 0);console_printchar(c, CCT(cc_ltee), 0);
	console_printchar(c, CCT(cc_btee), 0);console_printchar(c, CCT(cc_ttee), 0);
	console_printchar(c, CCT(cc_plus), 0);console_printchar(c, CCT(cc_s1), 0);
	console_printchar(c, CCT(cc_s9), 0);console_printchar(c, CCT(cc_diamond), 0);
	/* 
	console_goto(c, 20, y++);
	console_printchar(c, CCT(cc_ckboard), 0);console_printchar(c, CCT(cc_degree), 0);
	console_printchar(c, CCT(cc_plminus), 0);console_printchar(c, CCT(cc_bullet), 0);
	console_printchar(c, CCT(cc_larrow), 0);console_printchar(c, CCT(cc_rarrow), 0);
	console_printchar(c, CCT(cc_darrow), 0);console_printchar(c, CCT(cc_uarrow), 0);
	

	console_goto(c, 20, y++);
	console_printchar(c, CCT(cc_board), 0);console_printchar(c, CCT(cc_lantern), 0);
	console_printchar(c, CCT(cc_block), 0);
	*/
	
	console_refresh(c);
	console_goto(c, 20, y++);
	for(x = 0; (unsigned) x < jl; x++) {
		//delay_output
		console_sleep(c, rand() % 200 + 80);
		console_printchar(c, japh[x], 0);
		console_refresh(c);
	}
	
	console_refresh(c);
	
	console_getkey(c);
	console_clear(c);
	console_cleanup(c);
	return 0;
}
