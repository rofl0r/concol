#ifndef ALLFONTS_H
#define ALLFONTS_H


// allowed values for CONSOLE_FONT (do not use BITFONT_START)
#define TESTFONT 1
#define BITFONT_START 2
#define INT10FONT08 (BITFONT_START + 0)
#define INT10FONT14 (BITFONT_START + 1)
#define INT10FONT16 (BITFONT_START + 2)

/* for usage with rcb you need to set -DCONSOLE_FONT=INT10FONT14
 * or another one while compiling. this will get the right headers
 * and the c files they link to, to be added to the build */

#if (CONSOLE_FONT==TESTFONT)
#  include "testfont_old.h"
#  define FONT (&testfont)
#elif (CONSOLE_FONT >= BITFONT_START)
#  include "bitfont.h"
#  include "dosfonts.h"
#  if (CONSOLE_FONT==INT10FONT08)
#    define FONT (bitfont_to_font(&int10_font_08))
#  elif (CONSOLE_FONT==INT10FONT14)
#    define FONT (bitfont_to_font(&int10_font_14))
#  elif (CONSOLE_FONT==INT10FONT16)
#    define FONT (bitfont_to_font(&int10_font_16))
#  else
#    error "unknown font"
#  endif
#else
#  error "need to set CONSOLE_FONT"
#endif


#endif
