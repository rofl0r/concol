#need to set BACKEND to either termbox, ncurses, or sdl

LIBBASENAME=concol256
LIBNAME=lib$(LIBBASENAME)

LINKLIBS_TERMBOX="-ltermbox"
LINKLIBS_SDL="-lSDL"
LINKLIBS_NCURSES="-lncurses"

CFLAGS_OWN=-Wall -Wextra -static -std=c99
CFLAGS_DBG=-g -O0
CFLAGS_OPT=-Os -s
CFLAGS_OPT_AGGRESSIVE=-O3 -s -flto -fwhole-program

DYNEXT=.so
STAEXT=.a

TEST=examples/console_test
TESTO=$(TEST).o

CONPIX=examples/console_conpix
CONPIXO=$(CONPIX).o

CONPIXS=examples/console_conpix_scroll
CONPIXSO=$(CONPIXS).o


-include config.mak

OUTLIB=$(LIBNAME)_$(BACKEND)
DYNLIB=$(OUTLIB)$(DYNEXT)
STALIB=$(OUTLIB)$(STAEXT)
MAINLIB=$(LIBNAME)$(DYNEXT)

ifeq ($(BACKEND),termbox)
	BACKEND_SRCS=tbconsole.c tbconsole_chartab.c
	LINKLIBS=$(LINKLIBS_TERMBOX)
else 
	ifeq ($(BACKEND),ncurses)
		BACKEND_SRCS=ncconsole.c
		LINKLIBS=$(LINKLIBS_NCURSES)
	else
		ifeq ($(BACKEND),sdl)
			BACKEND_SRCS=sdlconsole.c sdlconsole_chartab.c
			LINKLIBS=$(LINKLIBS_SDL)
		endif
	endif
endif

#ifndef $(LINKLIBS)
#	$(error "need to set BACKEND to either termbox, ncurses, or sdl")
#endif


SRCS=console.c fonts/testfont.c fonts/dosfonts.c fonts/bitfont.c $(BACKEND_SRCS)
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

$(STALIB): $(OBJS)
	ar rc $@ $(OBJS)
	ranlib $@

$(DYNLIB): $(OBJS)
	$(CC) -shared $(LDFLAGS) -o $@ $(OBJS) $(LINKLIBS)
#	$(CC) -shared $(LDFLAGS) -Wl,-soname=$@ -o $@ $(OBJS) $(LINKLIBS)

$(MAINLIB): $(DYNLIB)
	ln -sf $(DYNLIB) $(MAINLIB)

%.o: %.c
	$(CC) -fPIC $(CPPFLAGS) $(CFLAGS) $(INC) -c -o $@ $<

test: $(TESTO)
	$(CC) $(CFLAGS) -o $(TEST) $(TESTO) -L. -l$(LIBBASENAME)

conpix: $(CONPIXO)
	$(CC) $(CFLAGS) -o $(CONPIX) $(CONPIXO) -L. -l$(LIBBASENAME) -llept

conpix-scroll: $(CONPIXSO)
	$(CC) $(CFLAGS) -o $(CONPIXS) $(CONPIXSO) -L. -l$(LIBBASENAME) -llept

examples: test conpix conpix-scroll


.PHONY: all
