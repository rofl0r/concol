#ifndef TBCONSOLE_CHARS_H
#define TBCONSOLE_CHARS_H

/* single border */
/*different flavors until incl 0x250f*/
#define CC_ULCORNER 0x250cU
#define CC_LLCORNER 0x2514U
#define CC_URCORNER 0x2510U
#define CC_LRCORNER 0x2518U

#define CC_RTEE 0x2524U
#define CC_LTEE 0x251cU
#define CC_BTEE 0x2534U
#define CC_TTEE 0x252cU

#define CC_HLINE 0x2500U
#define CC_HLINE_BOLD 0x2501U
#define CC_VLINE 0x2502U
#define CC_VLINE_BOLD 0x2503U

#define CC_PLUS 0x253cU

/*double border */
#define CC_ULCORNER_D 0x2554U
#define CC_LLCORNER_D 0x255AU
#define CC_URCORNER_D 0x2557U
#define CC_LRCORNER_D 0x255DU

#define CC_RTEE_D 0x2563U
#define CC_LTEE_D 0x2560U
#define CC_BTEE_D 0x2569U
#define CC_TTEE_D 0x2566U

#define CC_HLINE_D 0x2550U
#define CC_VLINE_D 0x2551U

#define CC_PLUS_D 0x256cU

/* double border on vertical lines, single border on horizontal lines, like ||- */
#define CC_ULCORNER_DV_SH 0x2553U
#define CC_LLCORNER_DV_SH 0x2559U
#define CC_URCORNER_DV_SH 0x2556U
#define CC_LRCORNER_DV_SH 0x255cU

#define CC_RTEE_DV_SH 0x2562U
#define CC_LTEE_DV_SH 0x255fU
#define CC_BTEE_DV_SH 0x2568U
#define CC_TTEE_DV_SH 0x2564U

#define CC_PLUS_DV_SH 0x256bU

/* double border on horizontal lines, single border on vertical lines, like |= */
#define CC_ULCORNER_DH_SV 0x2552U
#define CC_LLCORNER_DH_SV 0x2558U
#define CC_URCORNER_DH_SV 0x2555U
#define CC_LRCORNER_DH_SV 0x255bU

#define CC_RTEE_DH_SV 0x2561U
#define CC_LTEE_DH_SV 0x255eU
#define CC_BTEE_DH_SV 0x2567U
#define CC_TTEE_DH_SV 0x2564U

#define CC_PLUS_DH_SV 0x256aU


#define CC_S1 0
#define CC_S9 0
#define CC_DIAMOND 0
#define CC_CKBOARD 0x2591U
#define CC_DEGREE 0xB0
#define CC_PLMINUS 0xb1U
#define CC_BULLET 0
#define CC_LARROW 0x2190U
#define CC_RARROW 0x2192U
#define CC_DARROW 0x2193U
#define CC_UARROW 0x2191U
/* board types 0x2591 - 93*/
#define CC_BOARD 0
#define CC_LANTERN 0
/* maybe 2588 */
#define CC_BLOCK 0x2593U

#define CC_S3 0
#define CC_S7 0
#define CC_LEQUAL 0x2264U
#define CC_GEQUAL 0x2265U
#define CC_PI 0
#define CC_NEQUAL 0x2260U
#define CC_STERLING 0xa3U

#define CC_SHADOW_LEFT 0x258cU
#define CC_SHADOW_RIGHT 0x2590U
#define CC_SHADOW_TOP 0x2580U
#define CC_SHADOW_BOTTOM 0x2584U

/* alternatives 0x226a/b */
#define CC_DOUBLE_ARROW_LT 0xabU /* << */
#define CC_DOUBLE_ARROW_GT 0xbbU /* >> */

/* 
 * http://dev.networkerror.org/utf8/?start=9600&end=10400&cols=4&show_uni_int=on&show_uni_hex=on&show_html_ent=on&show_raw_hex=on&show_raw_bin=on
 * 
 * zodiac ~ 0x2648 *
 * chess figures 0x2654
 * "ok" and "chancel" 0x2713
 * crosses and stars 0x2719
 * skull, radioactive, ... 0x2620
 */

#endif
