CC = gcc
RFLAGS = -O2 -std=c99 -Wall -Wextra -pedantic -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED
DFLAGS = -O0 -g -std=c99 -Wall -Wextra -pedantic -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED
LFLAGS = -L/usr/lib -ltermbox
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)
TARGETS = $(OBJECTS:%.o=%)

ifeq ($(MODE),debug)
	CFLAGS=$(DFLAGS)
else
	MODE=release
	CFLAGS=$(RFLAGS)
endif

all: $(TARGETS)

$(TARGETS): $(OBJECTS)

$(OBJECTS): $(SOURCES)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

%:%.o
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

.PHONY: debug
debug:
	make MODE=debug

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGETS)
