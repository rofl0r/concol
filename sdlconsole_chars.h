#ifndef SDLCONSOLE_CHARS_H
#define SDLCONSOLE_CHARS_H

/* single border */
#define CC_ULCORNER 218
#define CC_LLCORNER 192
#define CC_URCORNER 191
#define CC_LRCORNER 217

#define CC_RTEE 180
#define CC_LTEE 195
#define CC_BTEE 193
#define CC_TTEE 194

#define CC_HLINE 196
#define CC_HLINE_BOLD 0
#define CC_VLINE 179
#define CC_VLINE_BOLD 0

#define CC_PLUS 197

/*double border */
#define CC_ULCORNER_D 201
#define CC_LLCORNER_D 200
#define CC_URCORNER_D 187
#define CC_LRCORNER_D 188

#define CC_RTEE_D 185
#define CC_LTEE_D 204
#define CC_BTEE_D 202
#define CC_TTEE_D 203

#define CC_HLINE_D 205
#define CC_VLINE_D 186

#define CC_PLUS_D 206

/* double border on vertical lines, single border on horizontal lines, like ||- */
#define CC_ULCORNER_DV_SH 214
#define CC_LLCORNER_DV_SH 211
#define CC_URCORNER_DV_SH 183
#define CC_LRCORNER_DV_SH 189

#define CC_RTEE_DV_SH 182
#define CC_LTEE_DV_SH 199
#define CC_BTEE_DV_SH 208
#define CC_TTEE_DV_SH 210

#define CC_PLUS_DV_SH 215

/* double border on horizontal lines, single border on vertical lines, like |= */
#define CC_ULCORNER_DH_SV 213
#define CC_LLCORNER_DH_SV 212
#define CC_URCORNER_DH_SV 184
#define CC_LRCORNER_DH_SV 190

#define CC_RTEE_DH_SV 181
#define CC_LTEE_DH_SV 198
#define CC_BTEE_DH_SV 207
#define CC_TTEE_DH_SV 209

#define CC_PLUS_DH_SV 216

#define CC_S1 0
#define CC_S9 0
#define CC_DIAMOND 4
#define CC_CKBOARD 178
#define CC_DEGREE 248
#define CC_PLMINUS 241
#define CC_BULLET 0
#define CC_LARROW 27
#define CC_RARROW 26
#define CC_DARROW 25
#define CC_UARROW 24
/* 176 - 178 are some kind of CKBOARD */
#define CC_BOARD 0
#define CC_LANTERN 0
// solid block, filling entire cell
#define CC_BLOCK 219

#define CC_S3 0
#define CC_S7 0
#define CC_LEQUAL 243
#define CC_GEQUAL 242
#define CC_PI 227
#define CC_NEQUAL 0
#define CC_STERLING 156

/* shadow - this is a solid block filling half of a cell*/
#define CC_SHADOW_LEFT 221
#define CC_SHADOW_RIGHT 222
#define CC_SHADOW_TOP 223
#define CC_SHADOW_BOTTOM 220

#define CC_DOUBLE_ARROW_LT 174 /* << */
#define CC_DOUBLE_ARROW_GT 175 /* >> */


#endif
