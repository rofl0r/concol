#!/bin/sh
fontname=$1
[ -z "$fontname" ] && exit 1
CFLAGS="-DCONSOLE_FONT=$fontname -g -O0" rcb --new --force font2bitfont.c > /dev/null || exit 1
./font2bitfont.out 2>font.data
../../bin2h/bin2h.pl font.data | perl -pe 's/^#.*$//;'
fontname=$(echo $fontname | perl -pe '$_ = lc($_);')
cat << EOF
const font $fontname = {
	{FONTWIDTH, FONTHEIGHT},
	FONTWIDTH * FONTHEIGHT,
	(char*) font_data
};

EOF
rm -f font.data
