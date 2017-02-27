# this is a Makefile include file to embed into projects shipping
# concol as part of their release tarball.
#
# it assumes you have a SRCS variable in your Makefile that lists all required
# .c files, which are then converted into their object file companions.
# i.e. something like OBJS=$(SRCS:.c=.o)
# you need to set the following variables in the Makefile:
# CONCOL_BASEDIR : directory containing the concol sources
# BACKEND : must be set to one of SDL or NCURSES
# you probably want to make BACKEND variable user-choosable via a configure
# script or something, the results of which you include from your Makefile
# as well.
# in case you don't want to use pkg-config for NCURSES LDFLAGS, you can set
# the NCURSES_LIBS variable to e.g. -lncurses or similar.
# you can also set SDL_LIBS in a similar fashion, if the default of -lSDL
# doesn't work for you for some reason.

CONCOL_BASEDIR ?= .
PKG_CONFIG ?= pkg-config

SRCS += $(CONCOL_BASEDIR)/console.c

ifeq ($(BACKEND),NCURSES)
  SRCS += $(CONCOL_BASEDIR)/ncconsole.c
  SRCS += $(CONCOL_BASEDIR)/color_reader.c
  ifeq ($(NCURSES_LIBS),)
    NCURSES_LIBS := $(shell $(PKG_CONFIG) --libs ncurses)
  endif
  LIBS += $(NCURSES_LIBS)
  CFLAGS+=-DCONSOLE_BACKEND=NCURSES_CONSOLE
  CFLAGS+=-DCONSOLE_FONT=NOFONT
else ifeq ($(BACKEND),SDL)
  CONSOLE_FONT ?= INT10FONT14
  ifeq ($(CONSOLE_FONT),INT10FONT14)
    SRCS+=$(CONCOL_BASEDIR)/fonts/int10font14.c
  else ifeq ($(CONSOLE_FONT),INT10FONT16)
    SRCS+=$(CONCOL_BASEDIR)/fonts/int10font16.c
  else ifeq ($(CONSOLE_FONT),INT10FONT08)
    SRCS+=$(CONCOL_BASEDIR)/fonts/int10font08.c
  else
    SRCS+=$(CONCOL_BASEDIR)/fonts/testfont.c
  endif
  SRCS += $(CONCOL_BASEDIR)/sdlconsole.c
  CFLAGS+=-DCONSOLE_FONT=$(CONSOLE_FONT)
  CFLAGS+=-DCONSOLE_BACKEND=SDL_CONSOLE
  ifeq ($(SDL_LIBS),)
    SDL_LIBS = -lSDL
  endif
  LIBS += $(SDL_LIBS)
else
  $(error need to set BACKEND to one of SDL or NCURSES)
endif

