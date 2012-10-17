#include "console_chars.h"
#include <ncurses.h>

unsigned int console_chartab[CC_MAX];
/* the ncurses crap is not determinable at compile time... sigh */
static void ncurses_chartab_init(void) {
	console_chartab[cc_block] = ACS_BLOCK;
	console_chartab[cc_board] = ACS_BOARD;
	console_chartab[cc_btee] = ACS_BTEE;
	console_chartab[cc_btee_d] = 0;
	console_chartab[cc_btee_dh_sv] = 0;
	console_chartab[cc_btee_dv_sh] = 0;
	console_chartab[cc_bullet] = ACS_BULLET;
	console_chartab[cc_ckboard] = ACS_CKBOARD;
	console_chartab[cc_darrow] = ACS_DARROW;
	console_chartab[cc_degree] = ACS_DEGREE;
	console_chartab[cc_diamond] = ACS_DIAMOND;
	console_chartab[cc_double_arrow_gt] = 0;
	console_chartab[cc_double_arrow_lt] = 0;
	console_chartab[cc_gequal] = ACS_GEQUAL;
	console_chartab[cc_hline] = ACS_HLINE;
	console_chartab[cc_hline_bold] = 0;
	console_chartab[cc_hline_d] = 0;
	console_chartab[cc_lantern] = ACS_LANTERN;
	console_chartab[cc_larrow] = ACS_LARROW;
	console_chartab[cc_lequal] = ACS_LEQUAL;
	console_chartab[cc_llcorner] = ACS_LLCORNER;
	console_chartab[cc_llcorner_d] = 0;
	console_chartab[cc_llcorner_dh_sv] = 0;
	console_chartab[cc_llcorner_dv_sh] = 0;
	console_chartab[cc_lrcorner] = ACS_LRCORNER;
	console_chartab[cc_lrcorner_d] = 0;
	console_chartab[cc_lrcorner_dh_sv] = 0;
	console_chartab[cc_lrcorner_dv_sh] = 0;
	console_chartab[cc_ltee] = ACS_LTEE;
	console_chartab[cc_ltee_d] = 0;
	console_chartab[cc_ltee_dh_sv] = 0;
	console_chartab[cc_ltee_dv_sh] = 0;
	console_chartab[cc_nequal] = ACS_NEQUAL;
	console_chartab[cc_pi] = ACS_PI;
	console_chartab[cc_plminus] = ACS_PLMINUS;
	console_chartab[cc_plus] = ACS_PLUS;
	console_chartab[cc_plus_d] = 0;
	console_chartab[cc_plus_dh_sv] = 0;
	console_chartab[cc_plus_dv_sh] = 0;
	console_chartab[cc_rarrow] = ACS_RARROW;
	console_chartab[cc_rtee] = ACS_RTEE;
	console_chartab[cc_rtee_d] = 0;
	console_chartab[cc_rtee_dh_sv] = 0;
	console_chartab[cc_rtee_dv_sh] = 0;
	console_chartab[cc_s1] = ACS_S1;
	console_chartab[cc_s3] = ACS_S3;
	console_chartab[cc_s7] = ACS_S7;
	console_chartab[cc_s9] = ACS_S9;
	console_chartab[cc_shadow_bottom] = 0;
	console_chartab[cc_shadow_left] = 0;
	console_chartab[cc_shadow_right] = 0;
	console_chartab[cc_shadow_top] = 0;
	console_chartab[cc_sterling] = ACS_STERLING;
	console_chartab[cc_ttee] = ACS_TTEE;
	console_chartab[cc_ttee_d] = 0;
	console_chartab[cc_ttee_dh_sv] = 0;
	console_chartab[cc_ttee_dv_sh] = 0;
	console_chartab[cc_uarrow] = ACS_UARROW;
	console_chartab[cc_ulcorner] = ACS_ULCORNER;
	console_chartab[cc_ulcorner_d] = 0;
	console_chartab[cc_ulcorner_dh_sv] = 0;
	console_chartab[cc_ulcorner_dv_sh] = 0;
	console_chartab[cc_urcorner] = ACS_URCORNER;
	console_chartab[cc_urcorner_d] = 0;
	console_chartab[cc_urcorner_dh_sv] = 0;
	console_chartab[cc_urcorner_dv_sh] = 0;
	console_chartab[cc_vline] = ACS_VLINE;
	console_chartab[cc_vline_bold] = 0;
	console_chartab[cc_vline_d] = 0;
}

