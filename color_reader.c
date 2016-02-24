#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/select.h>
#include "rgb.h"
#include "color_reader.h"

#ifndef XCASE
#define XCASE 0
#endif

static int byte_from_hex(const char* s, unsigned char* out) {
	static const char hex[] = "0123456789abcdef";
	char* h1 = strchr(hex, s[0]);
	char* h2 = strchr(hex, s[1]);
	if(!h1 || !h2) return 1;
	*out = ((h1 - hex) << 4) | (h2 - hex);
	return 0;
}

int color_reader_init(struct color_reader *cr) {
	if(-1==(cr->fd = open("/dev/tty", O_RDWR|O_CLOEXEC))) return 1;
	if(-1==tcgetattr(cr->fd, &cr->t)) goto err;
	struct termios new = cr->t;
	new.c_iflag = 0;
	new.c_oflag &= ~OPOST;
	new.c_lflag &= ~(ISIG | ICANON | ECHO | XCASE);
	new.c_cc[VMIN] = 0;
	new.c_cc[VTIME] = 0;
	if(-1==tcsetattr(cr->fd, TCSAFLUSH, &new)) goto err;
	tcdrain(cr->fd);
	return 0;
err:
	close(cr->fd);
	return 1;
}

int color_reader_get_color(struct color_reader *cr, int colnr, rgb_t*out) {
        char buf[128], *p;
	unsigned l;
	fd_set reed;

	dprintf(cr->fd, "\x1b]4;%d;?\x1b\x5c", colnr);
	FD_ZERO(&reed);
	FD_SET(cr->fd, &reed);
	while(select(cr->fd+1, &reed, 0, 0, 0)<=0);
	for(l=0,p=buf;l<sizeof(buf);l++,p++) {
		ssize_t rd;
		if(1==(rd=read(cr->fd, p, 1))) continue;
		if(rd < 0) return 2;
	}
	if(l<23) return 3;
	*p = 0, p = buf;
	if(memcmp(p,"\x1b]4;",4)) return 4;
	p+=4;l-=4;
	while(l>4 && memcmp(p,"rgb:",4)) p++,l--;
	p+=4;l-=4;
	if(l<14) return 5;
	if(byte_from_hex(p, &out->r)) return 6;
	p+=5;
	if(byte_from_hex(p, &out->g)) return 7;
	p+=5;
	if(byte_from_hex(p, &out->b)) return 8;
	return 0;
}
int color_reader_close(struct color_reader *cr) {
	int err = 0;
	if(tcsetattr(cr->fd, TCSAFLUSH, &cr->t)==-1) err=1;
	close(cr->fd);
	return err;
}
