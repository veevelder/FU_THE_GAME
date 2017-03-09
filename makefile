CC=gcc
CFLAGS=-O2
UNAME=$(shell uname)
LINK=-lSDL2 -lSDL2_image -lSDL2_ttf
MACLINK=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2main -framework Cocoa
WINLINK=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

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
