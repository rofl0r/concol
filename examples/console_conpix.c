//============================================================================
// Name        : conpix.c
// Author      : rofl0r
// Version     :
// Copyright   : GPL v2
// Description : a picture viewer for the console. uses xterm-256color mode.
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <leptonica/allheaders.h>

#include "../console.h"
#define CONSOLE_FONT TESTFONT

#include "../fonts/allfonts.h"
//#include "strlib.h"
//RcB: LINK "-llept"

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif


int main(int argc, char** argv) {
	char* filename;
	int scaleFullScreen = 0;
	Console co;
	Console* t = &co;
	struct {int w, h;} condim;
	struct {int w, h;} pixdim;
	struct {float x, y;} scalefact = { 1.0, 1.0 };
	float factor;
	struct Pix* pngfile;
	struct Pix* ping;
	struct Pix* palette;
	struct Pix* pix32;
	int i, ix, iy;
	
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

	console_getbounds(t, &condim.w, &condim.h);

	pngfile = pixRead(filename);

	pixGetDimensions(pngfile, &pixdim.w, &pixdim.h, NULL);

	if(scaleFullScreen) {
		scalefact.x = (condim.w * 1.0) / (pixdim.w * 2.0);
		scalefact.y = (condim.h * 1.0) / (pixdim.h * 1.0);
		ping = pixScale(pngfile, scalefact.x * 2.0, scalefact.y);
	} else {
		scalefact.x = (condim.w * 1.0) / (pixdim.w * 1.0);
		scalefact.y = (condim.h * 1.0) / (pixdim.h * 1.0);
		factor = MIN(scalefact.x, scalefact.y);
		if (condim.w *2 < pixdim.w || condim.h < pixdim.h)
			ping = pixScale(pngfile, factor, factor);
		else
			// scale to double width so the proportion of the font is correct
			ping = pixScale(pngfile, 2.0, 1.0 );
	}
	pixDestroy(&pngfile);
	
	palette = pixOctreeColorQuant(ping, 240, 1);
	
	if (palette == NULL) { puts("palette is nul"); goto finish_him; }

	pix32 = pixConvertTo32(palette);
	pixGetDimensions(pix32, &pixdim.w, &pixdim.h, NULL);

	int startx = 0, starty = 0;
	int endx = startx + MIN(condim.w, pixdim.w) ;
	int endy = starty + MIN(condim.h, pixdim.h);

	int* bufptr = (int*) pix32->data;
	if (bufptr == NULL) {
		puts("bufptr is null");
		goto finish_him;
	}

	for (iy = 0; iy < endy; iy++){
		for (ix = 0; ix < endx; ix++){
			console_setcolor(t, 0, *((rgb_t*) bufptr));
			console_goto(t, ix, iy);
			console_addchar(t, ' ', 0);
			bufptr++;
		}
	}
	pixDestroy(&palette);
	pixDestroy(&pix32);

	console_refresh(t);
	
	finish_him:
	console_getkey(t);
	
	console_cleanup(t);

	return 0;
}
