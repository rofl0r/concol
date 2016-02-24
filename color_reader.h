#ifndef  COLOR_READER_H
#define  COLOR_READER_H

#include <termios.h>
#include "rgb.h"

/* code to read *actual* colors from terminal.
   this is not possible with ncurses
   (color_content() returns impl.-defined default colors).
   all functions return 0 on success, and non-0 on failure. */

struct color_reader {
	int fd;
	struct termios t;
};

int color_reader_init(struct color_reader *cr);
int color_reader_get_color(struct color_reader *cr, int colnr, rgb_t*out);
int color_reader_close(struct color_reader *cr);

//RcB: DEP "color_reader.c"
#endif
