#ifndef NCCONSOLECHARS_H
#define NCCONSOLECHARS_H

#include <ncurses.h>

/*
 * ACS_ULCORNER	WACS_ULCORNER 	+ 	upper left-hand corner 
 * ACS_LLCORNER	WACS_LLCORNER 	+ 	lower left-hand corner 
 * ACS_URCORNER	WACS_URCORNER 	+ 	upper right-hand corner 
 * ACS_LRCORNER	WACS_LRCORNER 	+ 	lower right-hand corner 
 * ACS_RTEE 	WACS_RTEE 	+ 	right tee 
 * ACS_LTEE 	WACS_LTEE 	+ 	left tee 
 * ACS_BTEE 	WACS_BTEE 	+ 	bottom tee 
 * ACS_TTEE 	WACS_TTEE 	+ 	top tee 
 * ACS_HLINE 	WACS_HLINE 	- 	horizontal line 
 * ACS_VLINE 	WACS_VLINE 	| 	vertical line 
 * ACS_PLUS 	WACS_PLUS 	+ 	plus 
 * ACS_S1 	WACS_S1 	- 	scan line 1 
 * ACS_S9 	WACS_S9 	_ 	scan line 9 
 * ACS_DIAMOND 	WACS_DIAMOND 	+ 	diamond 
 * ACS_CKBOARD 	WACS_CKBOARD 	: 	checker board (stipple) 
 * ACS_DEGREE 	WACS_DEGREE 	' 	degree symbol 
 * ACS_PLMINUS 	WACS_PLMINUS 	# 	plus/minus 
 * ACS_BULLET 	WACS_BULLET 	o 	bullet 
 * ACS_LARROW 	WACS_LARROW 	< 	arrow pointing left 
 * ACS_RARROW 	WACS_RARROW 	> 	arrow pointing right 
 * ACS_DARROW 	WACS_DARROW 	v 	arrow pointing down 
 * ACS_UARROW 	WACS_UARROW 	^ 	arrow pointing up 
 * ACS_BOARD 	WACS_BOARD 	# 	board of squares 
 * ACS_LANTERN 	WACS_LANTERN 	# 	lantern symbol 
 * ACS_BLOCK 	WACS_BLOCK 	# 	solid square block
 * */

#define CC_ULCORNER ACS_ULCORNER
#define CC_LLCORNER ACS_LLCORNER
#define CC_URCORNER ACS_URCORNER
#define CC_LRCORNER ACS_LRCORNER
#define CC_RTEE ACS_RTEE
#define CC_LTEE ACS_LTEE
#define CC_BTEE ACS_BTEE
#define CC_TTEE ACS_TTEE
#define CC_HLINE ACS_HLINE
#define CC_VLINE ACS_VLINE
#define CC_PLUS ACS_PLUS
#define CC_S1 ACS_S1
#define CC_S9 ACS_S9
#define CC_DIAMOND ACS_DIAMOND
#define CC_CKBOARD ACS_CKBOARD
#define CC_DEGREE ACS_DEGREE
#define CC_PLMINUS ACS_PLMINUS
#define CC_BULLET ACS_BULLET
#define CC_LARROW ACS_LARROW
#define CC_RARROW ACS_RARROW
#define CC_DARROW ACS_DARROW
#define CC_UARROW ACS_UARROW
#define CC_BOARD ACS_BOARD
#define CC_LANTERN ACS_LANTERN
#define CC_BLOCK ACS_BLOCK
#define CC_S3 ACS_S3
#define CC_S7 ACS_S7
#define CC_LEQUAL ACS_LEQUAL
#define CC_GEQUAL ACS_GEQUAL
#define CC_PI ACS_PI
#define CC_NEQUAL ACS_NEQUAL
#define CC_STERLING ACS_STERLING

/* double border on vertical lines, single border on horizontal lines, like ||- */
#define CC_ULCORNER_DV_SH 0
#define CC_LLCORNER_DV_SH 0
#define CC_URCORNER_DV_SH 0
#define CC_LRCORNER_DV_SH 0

#define CC_RTEE_DV_SH 0
#define CC_LTEE_DV_SH 0
#define CC_BTEE_DV_SH 0
#define CC_TTEE_DV_SH 0

#define CC_PLUS_DV_SH 0

/* double border on horizontal lines, single border on vertical lines, like |= */
#define CC_ULCORNER_DH_SV 0
#define CC_LLCORNER_DH_SV 0
#define CC_URCORNER_DH_SV 0
#define CC_LRCORNER_DH_SV 0

#define CC_RTEE_DH_SV 0
#define CC_LTEE_DH_SV 0
#define CC_BTEE_DH_SV 0
#define CC_TTEE_DH_SV 0

#define CC_PLUS_DH_SV 0

#define CC_SHADOW_LEFT 0
#define CC_SHADOW_RIGHT 0
#define CC_SHADOW_TOP 0
#define CC_SHADOW_BOTTOM 0

#define CC_DOUBLE_ARROW_LT 0 /* << */
#define CC_DOUBLE_ARROW_GT 0 /* >> */



#endif