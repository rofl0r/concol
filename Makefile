#need to set BACKEND to either termbox, ncurses, or sdl

LIBNAME=libconcol256

LINKLIBS_TERMBOX="-ltermbox"
LINKLIBS_SDL="-lSDL"
LINKLIBS_NCURSES="-lncurses"

CFLAGS_OWN=-Wall -Wextra -static -std=c99
CFLAGS_DBG=-g -O0
CFLAGS_OPT=-Os -s
CFLAGS_OPT_AGGRESSIVE=-O3 -s -flto -fwhole-program

DYNEXT=.so
STAEXT=.a

-include config.mak

OUTLIB=$(LIBNAME)_$(BACKEND)
DYNLIB=$(OUTLIB)$(DYNEXT)
STALIB=$(OUTLIB)$(STAEXT)
MAINLIB=$(LIBNAME)$(DYNEXT)

ifeq ($(BACKEND),termbox)
	BACKEND_SRCS=tbconsole.c
	LINKLIBS=$(LINKLIBS_TERMBOX)
else 
	ifeq ($(BACKEND),ncurses)
		BACKEND_SRCS=ncconsole.c
		LINKLIBS=$(LINKLIBS_NCURSES)
	else
		ifeq ($(BACKEND),sdl)
			BACKEND_SRCS=sdlconsole.c
			LINKLIBS=$(LINKLIBS_SDL)
		endif
	endif
endif

#ifndef $(LINKLIBS)
#	$(error "need to set BACKEND to either termbox, ncurses, or sdl")
#endif


SRCS=console.c $(BACKEND_SRCS)
OBJS=$(SRCS:.c=.o)


all: $(STALIB) $(DYNLIB) $(MAINLIB)

clean:
	rm -f $(DYNLIB)
	rm -f $(STALIB)
	rm -f *.o

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

.PHONY: all
