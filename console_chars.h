#ifndef CONSOLE_CHARS_H
#define CONSOLE_CHARS_H

/* naming of the constants
 
 * single border:
   cc_ulcorner (ul = upper left)
   
 * double border:
   cc_ulcorner_d et al
   
 * double border on vertical lines, single border on horizontal lines, like ||-
   dv = double vertical, sh = single horizontal
   cc_ulcorner_dv_sh
   
 * double border on horizontal lines, single border on vertical lines, like |=
   cc_ulcorner_dh_sv
   
   these constants are mostly named after their ncurses equivalent, e.g.
   ACS_BLOCK. some other useful symbols have beend added to complete the set.
   see the ncurses header that defines ACS_BLOCK et al for more information.

*/

enum {
	cc_block = 0, /* solid block, filling entire cell */
	cc_board,
	cc_btee,
	cc_btee_d,
	cc_btee_dh_sv,
	cc_btee_dv_sh,
	cc_bullet,
	cc_ckboard,
	cc_darrow,
	cc_degree,
	cc_diamond,
	cc_double_arrow_gt, /* >> */
	cc_double_arrow_lt, /* << */
	cc_gequal,
	cc_hline,
	cc_hline_bold,
	cc_hline_d,
	cc_lantern,
	cc_larrow,
	cc_lequal,
	cc_llcorner,
	cc_llcorner_d,
	cc_llcorner_dh_sv,
	cc_llcorner_dv_sh,
	cc_lrcorner,
	cc_lrcorner_d,
	cc_lrcorner_dh_sv,
	cc_lrcorner_dv_sh,
	cc_ltee,
	cc_ltee_d,
	cc_ltee_dh_sv,
	cc_ltee_dv_sh,
	cc_nequal,
	cc_pi,
	cc_plminus,
	/* plus is the center crossing of lines. like + */
	cc_plus,
	cc_plus_d,
	cc_plus_dh_sv,
	cc_plus_dv_sh,
	cc_rarrow,
	cc_rtee,
	cc_rtee_d,
	cc_rtee_dh_sv,
	cc_rtee_dv_sh,
	/* scan line 1, .. 9, whatever that means */
	cc_s1,
	cc_s3,
	cc_s7,
	cc_s9,
	/* shadow - this is a solid block filling half of a cell */
	cc_shadow_bottom,
	cc_shadow_left,
	cc_shadow_right,
	cc_shadow_top,
	cc_sterling,
	/* t = top, b = bottom, r= right, ...: 
	 * tee is the crossing of 3 lines, like T on top or -| on the right */
	cc_ttee,
	cc_ttee_d,
	cc_ttee_dh_sv,
	cc_ttee_dv_sh,
	cc_uarrow,
	cc_ulcorner,
	cc_ulcorner_d,
	cc_ulcorner_dh_sv,
	cc_ulcorner_dv_sh,
	cc_urcorner,
	cc_urcorner_d,
	cc_urcorner_dh_sv,
	cc_urcorner_dv_sh,
	cc_vline,
	cc_vline_bold,
	cc_vline_d,
	CC_MAX
} console_chars;

extern unsigned int console_chartab[CC_MAX];

#endif
