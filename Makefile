PREFIX = /usr/local

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2
CFLAGS   = -std=c99 -pedantic -Wall -Werror -Wno-deprecated-declarations -Os ${CPPFLAGS}
LDFLAGS  = -lX11

CC = gcc

SRC = dwmblocks.c
OBJ = ${SRC:.c=.o}

all: options dwmblocks

options:
	@echo dwmblocks build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: blocks.h

dwmblocks: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwmblocks ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwmblocks /usr/local/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwmblocks

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dwmblocks

.PHONY: all options clean install uninstall
