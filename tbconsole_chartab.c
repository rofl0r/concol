#include "console_chars.h"

/* 
 * http://dev.networkerror.org/utf8/?start=9600&end=10400&cols=4&show_uni_int=on&show_uni_hex=on&show_html_ent=on&show_raw_hex=on&show_raw_bin=on
 * 
 * zodiac ~ 0x2648 *
 * chess figures 0x2654
 * "ok" and "chancel" 0x2713
 * crosses and stars 0x2719
 * skull, radioactive, ... 0x2620
 */

/* single border :
 * different flavors until incl 0x250f */

unsigned int console_chartab[CC_MAX] = {
	/* maybe 2588 */
	[cc_block] = 0x2593U,
	[cc_board] = 0,
	[cc_btee] = 0x2534U,
	[cc_btee_d] = 0x2569U,
	[cc_btee_dh_sv] = 0x2567U,
	[cc_btee_dv_sh] = 0x2568U,
	[cc_bullet] = 0,
	[cc_ckboard] = 0x2591U,
	[cc_darrow] = 0x2193U,
	[cc_degree] = 0xB0,
	[cc_diamond] = 0,
	/* alternatives 0x226a/b */
	[cc_double_arrow_gt] = 0xbbU,
	[cc_double_arrow_lt] = 0xabU,
	[cc_gequal] = 0x2265U,
	[cc_hline] = 0x2500U,
	[cc_hline_bold] = 0x2501U,
	[cc_hline_d] = 0x2550U,
	[cc_lantern] = 0,
	[cc_larrow] = 0x2190U,
	[cc_lequal] = 0x2264U,
	[cc_llcorner] = 0x2514U,
	[cc_llcorner_d] = 0x255AU,
	[cc_llcorner_dh_sv] = 0x2558U,
	[cc_llcorner_dv_sh] = 0x2559U,
	[cc_lrcorner] = 0x2518U,
	[cc_lrcorner_d] = 0x255DU,
	[cc_lrcorner_dh_sv] = 0x255bU,
	[cc_lrcorner_dv_sh] = 0x255cU,
	[cc_ltee] = 0x251cU,
	[cc_ltee_d] = 0x2560U,
	[cc_ltee_dh_sv] = 0x255eU,
	[cc_ltee_dv_sh] = 0x255fU,
	[cc_nequal] = 0x2260U,
	[cc_pi] = 0,
	[cc_plminus] = 0xb1U,
	[cc_plus] = 0x253cU,
	[cc_plus_d] = 0x256cU,
	[cc_plus_dh_sv] = 0x256aU,
	[cc_plus_dv_sh] = 0x256bU,
	[cc_rarrow] = 0x2192U,
	[cc_rtee] = 0x2524U,
	[cc_rtee_d] = 0x2563U,
	[cc_rtee_dh_sv] = 0x2561U,
	[cc_rtee_dv_sh] = 0x2562U,
	[cc_s1] = 0,
	[cc_s3] = 0,
	[cc_s7] = 0,
	[cc_s9] = 0,
	[cc_shadow_bottom] = 0x2584U,
	[cc_shadow_left] = 0x258cU,
	[cc_shadow_right] = 0x2590U,
	[cc_shadow_top] = 0x2580U,
	[cc_sterling] = 0xa3U,
	[cc_ttee] = 0x252cU,
	[cc_ttee_d] = 0x2566U,
	[cc_ttee_dh_sv] = 0x2564U,
	[cc_ttee_dv_sh] = 0x2564U,
	[cc_uarrow] = 0x2191U,
	[cc_ulcorner] = 0x250cU,
	[cc_ulcorner_d] = 0x2554U,
	[cc_ulcorner_dh_sv] = 0x2552U,
	[cc_ulcorner_dv_sh] = 0x2553U,
	[cc_urcorner] = 0x2510U,
	[cc_urcorner_d] = 0x2557U,
	[cc_urcorner_dh_sv] = 0x2555U,
	[cc_urcorner_dv_sh] = 0x2556U,
	[cc_vline] = 0x2502U,
	[cc_vline_bold] = 0x2503U,
	[cc_vline_d] = 0x2551U,
};