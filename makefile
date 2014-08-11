CC=gcc
CFLAGS=-O2
UNAME=$(shell uname)
LINK=-lSDL -lSDL_image -lSDL_ttf
MACLINK=-lSDL -lSDL_image -lSDL_ttf -lSDLmain -framework Cocoa
WINLINK=-lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf

ifeq ($(UNAME),Darwin)
all: 
	@echo "building"
	$(CC) $(CFLAGS) mac/source/fuckyou.c $(MACLINK) -o mac/fuckyou
	./mac/fuckyou
else ifeq ($(UNAME),MINGW32_NT-6.1)
all:
	@echo "building"
	$(CC) $(CFLAGS) windows/source/fuckyou.c $(WINLINK) -o windows/fuckyou.exe
	windows/fuckyou.exe
else
all: 
	@echo "building"
	$(CC) $(CFLAGS) linux/source/fuckyou.c $(LINK) -o linux/fuckyou
	./linux/fuckyou
endif
clean:
	rm -rf fuckyou
