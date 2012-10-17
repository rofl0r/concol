#ifndef ALLFONTS_H
#define ALLFONTS_H


// allowed values for CONSOLE_FONT (do not use BITFONT_START)
#define TESTFONT 1
#define BITFONT_START 2
#define INT10FONT08 (BITFONT_START + 0)
#define INT10FONT14 (BITFONT_START + 1)
#define INT10FONT16 (BITFONT_START + 2)

#if (CONSOLE_FONT==TESTFONT)
//RcB: SKIPON "CONSOLE_FONT=INT10FONT"
#  include "testfont.h"
//RcB: SKIPOFF "CONSOLE_FONT=INT10FONT"
#  define FONT (&testfont)
#elif (CONSOLE_FONT >= BITFONT_START)
//RcB: SKIPON "CONSOLE_FONT=TESTFONT"
#  include "bitfont.h"
#  include "dosfonts.h"
//RcB: SKIPOFF "CONSOLE_FONT=TESTFONT"
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