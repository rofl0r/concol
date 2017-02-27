#ifndef ALLFONTS_H
#define ALLFONTS_H


// allowed values for CONSOLE_FONT (do not use BITFONT_START)
#define NOFONT 0
#define TESTFONT 1
#define BITFONT_START 2
#define INT10FONT08 (BITFONT_START + 0)
#define INT10FONT14 (BITFONT_START + 1)
#define INT10FONT16 (BITFONT_START + 2)

/* for usage with rcb you need to set -DCONSOLE_FONT=INT10FONT14
 * or another one while compiling. this will get the right headers
 * and the c files they link to, to be added to the build */

#ifndef CONSOLE_BACKEND
#  error "CONSOLE_BACKEND must be set"
#endif

/* we don't need a font for terminal backends */
#if (CONSOLE_FONT==NOFONT) || !defined(CONSOLE_FONT)
#  if (CONSOLE_BACKEND==SDL_CONSOLE)
#    error "SDL_CONSOLE backend requires use of a font"
#  endif
#  ifdef NULL
#    define FONT NULL
#  else
#    define FONT 0UL
#  endif
#elif (CONSOLE_FONT==TESTFONT)
//RcB: SKIPON "CONSOLE_FONT=INT10FONT"
#  include "testfont.h"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT"
#  define FONT (&testfont)
#elif (CONSOLE_FONT >= BITFONT_START)
//RcB: SKIPON "CONSOLE_FONT=TESTFONT"
#  if (CONSOLE_FONT==INT10FONT08)
//RcB: SKIPON "CONSOLE_FONT=INT10FONT14"
//RcB: SKIPON "CONSOLE_FONT=INT10FONT16"
#    include "int10font08.h"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT14"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT16"
#    define FONT (&int10font08)
#  elif (CONSOLE_FONT==INT10FONT14)
//RcB: SKIPON "CONSOLE_FONT=INT10FONT08"
//RcB: SKIPON "CONSOLE_FONT=INT10FONT16"
#    include "int10font14.h"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT08"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT16"
#    define FONT (&int10font14)
#  elif (CONSOLE_FONT==INT10FONT16)
//RcB: SKIPON "CONSOLE_FONT=INT10FONT08"
//RcB: SKIPON "CONSOLE_FONT=INT10FONT14"
#    include "int10font16.h"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT08"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT14"
#    define FONT (&int10font16)
//RcB: SKIPOFF "CONSOLE_FONT=TESTFONT"
#  else
#    error "unknown font"
#  endif
#endif


#endif
