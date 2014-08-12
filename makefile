CC=gcc
CFLAGS=-O2
UNAME=$(shell uname)
LINK=-lSDL -lSDL_image -lSDL_ttf
MACLINK=-lSDL -lSDL_image -lSDL_ttf -lSDLmain -framework Cocoa
WINLINK=-lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf

ifeq ($(UNAME),Darwin)
all: 
	@echo "building"
	$(CC) $(CFLAGS) code/source/fuckyou.c $(MACLINK) -o fuckyou
	./fuckyou
else ifeq ($(UNAME),MINGW32_NT-6.1)
all:
	@echo "building"
	$(CC) $(CFLAGS) code/source/fuckyou.c $(WINLINK) -o fuckyou.exe
	fuckyou.exe
else
all: 
	@echo "building"
	$(CC) $(CFLAGS) code/source/fuckyou.c $(LINK) -o fuckyou
	./fuckyou
endif
clean:
	rm -rf fuckyou
	rm -rf fuckyou.exe
