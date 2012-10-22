#include "../font.h"
#include "bitfont.h"
#include "allfonts_old.h"
#include "../../../lib/include/bitarray.h"
#include <stdio.h>

int main() {
	bitfont* New = font_to_bitfont(FONT);
	fprintf(stdout, "#include \"bitfont.h\"\n#define FONTWIDTH %d\n#define FONTHEIGHT %d\n\n"
		, New->dim.x, New->dim.y);
	fwrite(New->characters, BA_SIZE_REQUIRED(256 * New->bitperchar), 1, stderr);
	return 0;
}
