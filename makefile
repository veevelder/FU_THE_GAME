CC=gcc
CFLAGS=-O2
UNAME=$(shell uname)
LINK=-lSDL -lSDL_image -lSDL_ttf
MACLINK=-lSDL -lSDL_image -lSDL_ttf -lSDLmain -framework Cocoa

ifeq ($(UNAME),Darwin)
all: 
	@echo "building"
	$(CC) $(CFLAGS) code/source/fuckyou.c $(MACLINK) -o fuckyou
else
all: 
	@echo "building"
	$(CC) $(CFLAGS) code/source/fuckyou.c $(LINK) -o fuckyou
endif
clean:
	rm -rf fuckyou
