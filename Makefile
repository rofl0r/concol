#need to set BACKEND to either TERMBOX, NCURSES, SDL, or SDL2
#e.g. $ make BACKEND=SDL2 -j4

LIBBASENAME=concol256
LIBNAME=lib$(LIBBASENAME)

LINKLIBS_TERMBOX=-ltermbox
LINKLIBS_SDL=-lSDL
LINKLIBS_SDL2=-lSDL2
PKG_CONFIG = pkg-config
LINKLIBS_NCURSES := $(shell $(PKG_CONFIG) --libs ncurses) -lpthread
LINKLIBS_LEPT := $(shell $(PKG_CONFIG) --libs lept)

CFLAGS_OWN=-Wall -Wextra -static -std=c99
CFLAGS_DBG=-g -O0
CFLAGS_OPT=-Os -s
CFLAGS_OPT_AGGRESSIVE=-O3 -s -flto -fwhole-program
CPPFLAGS_BE=-DCONSOLE_BACKEND=$(subst 2,,$(BACKEND))_CONSOLE

DYNEXT=.so
STAEXT=.a

TEST=examples/console_test
PLASMA=examples/plasma
CONPIX=examples/console_conpix
CONPIXS=examples/console_conpix_scroll

EXAMPLES=$(TEST) $(PLASMA) $(CONPIX) $(CONPIXS)

-include config.mak

OUTLIB=$(LIBNAME)_$(BACKEND)
DYNLIB=$(OUTLIB)$(DYNEXT)
STALIB=$(OUTLIB)$(STAEXT)
MAINLIB=$(LIBNAME)$(DYNEXT)

ifeq ($(BACKEND),)
 $(error need to set BACKEND!)
endif
ifeq ($(BACKEND),TERMBOX)
 BACKEND_SRCS=tbconsole.c tbconsole_chartab.c
 LINKLIBS=$(LINKLIBS_TERMBOX)
else
 ifeq ($(BACKEND),NCURSES)
  BACKEND_SRCS=ncconsole.c color_reader.c
  LINKLIBS=$(LINKLIBS_NCURSES)
 else
  ifeq ($(subst 2,,$(BACKEND)),SDL)
   BACKEND_SRCS=sdlconsole.c sdlconsole_chartab.c
   FONTSRCS=fonts/testfont.c fonts/int10font08.c fonts/int10font14.c fonts/int10font16.c
  endif
  ifeq ($(BACKEND),SDL)
   LINKLIBS=$(LINKLIBS_SDL)
  endif
  ifeq ($(BACKEND),SDL2)
   SDL2ADD=-DUSE_SDL2=1
   LINKLIBS=$(LINKLIBS_SDL2)
  endif
 endif
endif

#ifndef $(LINKLIBS)
#	$(error "need to set BACKEND to either termbox, ncurses, or sdl")
#endif

SRCS=console.c $(BACKEND_SRCS) $(FONTSRCS)
OBJS=$(SRCS:.c=.o)


all: $(STALIB) $(DYNLIB) $(MAINLIB)

clean-symlink:
	rm -f $(MAINLIB)

clean-all:
	rm -f $(LIBNAME)*.a
	rm -f $(LIBNAME)*.so
	rm -f *.o
	rm -f examples/*.o
	rm -f fonts/*.o

clean:
	rm -f $(DYNLIB)
	rm -f $(STALIB)
	rm -f $(OBJS)
	rm -f $(EXAMPLES)

$(STALIB): $(OBJS)
	ar rc $@ $(OBJS)
	ranlib $@

$(DYNLIB): $(OBJS)
	$(CC) -shared $(LDFLAGS) -o $@ $(OBJS) $(LINKLIBS)
#	$(CC) -shared $(LDFLAGS) -Wl,-soname=$@ -o $@ $(OBJS) $(LINKLIBS)

$(MAINLIB): $(DYNLIB)
	ln -sf $(DYNLIB) $(MAINLIB)

%.o: %.c
	$(CC) -fPIC $(CPPFLAGS) $(CFLAGS) $(INC) $(CPPFLAGS_BE) $(SDL2ADD) -c -o $@ $<

$(TEST): $(TEST).o $(STALIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKLIBS)

$(PLASMA): $(PLASMA).o $(STALIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKLIBS) -lm

$(CONPIX): $(CONPIX).o $(STALIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKLIBS) $(LINKLIBS_LEPT)

$(CONPIXS): $(CONPIXS).o $(STALIB)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKLIBS) $(LINKLIBS_LEPT)

examples: $(EXAMPLES)


.PHONY: all clean examples
