INCLUDES="."
LINKDIRS=
LINKLIBS=-llept -lm

SRCS=$(sort $(wildcard ./*.c))
PROGS=$(SRCS:.c=.out)

MAINFILE=pato.c

CFLAGS_OWN=-Wall -D_GNU_SOURCE
CFLAGS_DBG=-g
CFLAGS_OPT=-s -Os

-include config.mak

CFLAGS_RCB_OPT=${CFLAGS_OWN} ${CFLAGS_OPT} -I ${INCLUDES} ${CFLAGS}
CFLAGS_RCB_DBG=${CFLAGS_OWN} ${CFLAGS_DBG} -I ${INCLUDES} ${CFLAGS}

all: $(PROGS)

clean:
	rm -f *.o
	rm -f $(PROGS)
	rm -f *.rcb

%.out: %.c
	CFLAGS="${CFLAGS_RCB_DBG}" rcb --force $(RCBFLAGS) $< $(LINKLIBS)
	


.PHONY: all optimized debug
