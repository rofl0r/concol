/*
 * Rgb.h
 *
 * Created on: 29.11.2010
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */

#ifndef RGB_H_
#define RGB_H_

#include <stdint.h>
#include "endianness.h"

typedef union {
	struct {
	#ifdef IS_LITTLE_ENDIAN
		unsigned char a;
		unsigned char b;
		unsigned char g;
		unsigned char r;
	#else
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	#endif
	};
	uint32_t asInt;
} rgb_t;

typedef struct {
	rgb_t bgcolor;
	rgb_t fgcolor;
} rgb_tuple;


#ifdef IS_LITTLE_ENDIAN
#define RGB(x,y,z) ((rgb_t) {{0, z, y, x}})
#else
#define RGB(x,y,z) ((rgb_t) {{x, y, z, 0}})
#endif

#define RGB_INIT(R,G,B) {.r = R, .g = G, .b = B, .a = 0}
#define RGBA_INIT(R,G,B,A) {.r = R, .g = G, .b = B, .a = A}

#endif /* RGB_H_ */
