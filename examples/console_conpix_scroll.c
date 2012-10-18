//============================================================================
// Name        : conpix.c
// Author      : rofl0r
// Version     :
// Copyright   : GPL v2
// Description : a picture viewer for the console. uses xterm-256color mode.
//============================================================================

//gcc -Wall -Wextra console_conpix_scroll.c console.c -llept -lncurses -o conpix_scroll

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <leptonica/allheaders.h>
//RcB: LINK "-llept"

#include "../console.h"
#include "../console_keys.h"

#define CONSOLE_FONT TESTFONT
#include "../fonts/allfonts.h"

int main(int argc, char** argv) {
	char* filename;
	int scaleFullScreen = 0;
	Console co;
	Console* t = &co;
	int cx; int cy;
	int w, h;
	int iterX, iterY;

	struct Pix* pngfile;
	struct Pix* ping;
	struct Pix* palette;
	struct Pix* pix32;
	int i, ix, iy;
	int c;
	
	for (i = 1; i<argc; i++) {
		if (strlen(argv[i]) > 1 && !memcmp(argv[i], "-f", 2)) 
			scaleFullScreen=1;
		else filename = argv[i];
	}

	if (access(filename, R_OK)) {
		puts("file not found!");
		puts("c0npix by rofl0r");
		puts("================");
		printf("arguments: %s [-f] somefile.[jpg|png|bmp|tiff]\n", argv[0]);
		puts("where -f means scale to fullscreen");
		puts("export TERM=xterm-256color before usage is recommended.");
		exit(1);
	}

	console_init(t);
	point reso = {800, 600};
	console_init_graphics(&co, reso, FONT);

	console_getbounds(t, &cx, &cy);

	pngfile = pixRead(filename);

	pixGetDimensions(pngfile, &w, &h, NULL);

	ping = pixScale(pngfile, 2.0, 1.0 );
	
	pixDestroy(&pngfile);
	
	palette = pixOctreeColorQuant(ping, 240, 1);
	
	if (palette == NULL) { puts("palette is nul"); goto finish_him; }

	pix32 = pixConvertTo32(palette);

	iterX = pix32->w;
	iterY = pix32->h;

	int* bufptr = (int*) pix32->data;
	if (bufptr == NULL) {
		puts("bufptr is null");
		goto finish_him;
	}
	
	int startx = 0;
	int starty = 0;
	
	paint:
	for(iy = starty; iy < starty + cy; iy++) {
		bufptr = (int*) pix32->data + (iy * pix32->w) + startx;
		for(ix = startx; ix < startx + cx; ix++) {
			console_setcolor(t, 0, *((rgb_t*) bufptr));
			console_goto(t, ix - startx, iy - starty);
			console_addchar(t, ' ', 0);
			bufptr++;
		}
	}
	console_draw(t);
	//console_printfxy(t, 0, 0, "%d", (int) c);
	
	while ((c = console_getkey(t)) != 'q') {

		console_setcolor(t, 0, RGB(0,0,0));
		
		switch(c) {
			case CK_CURSOR_UP: 
				if(starty > 0) starty--;
				break;
			case CK_CURSOR_DOWN:
				if(starty < pix32->h - cy)
					starty++;
				break;
			case CK_CURSOR_LEFT:
				if(startx > 0)
					startx--;
				break;
			case CK_CURSOR_RIGHT:
				if(startx < pix32->w - cx)
					startx++;
				break;
			default:
				goto loopend; // ignore mouse movement and similar stuff
				break;
		}
		goto paint;
		loopend: ;
	}
	
	pixDestroy(&palette);
	pixDestroy(&pix32);

	console_refresh(t);
	
	finish_him:
	//console_getkey(t);
	
	console_cleanup(t);

	return 0;
}
