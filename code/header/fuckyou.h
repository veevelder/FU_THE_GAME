/*
Fuck You The Game
fuckyou.h
Licenced Under the MIT Licence so Fuck Off
Matt "The Big Fuck" Van Veldhuizen
Karl "The Little Fuck" Ott
*/


#ifndef __FUCKYOU_H__
#define __FUCKYOU_H__

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1200

/* Globals */
#define SCREEN_BPP 32
#define FRAMES_PER_SECOND 60

#define DOT_WIDTH 20
#define DOT_HEIGHT 20
#define CLOUDONE_HEIGHT 174
#define CLOUDONE_WIDTH 157
#define BACKGROUND_HEIGHT 600
#define BACKGROUND_WIDTH 800


#define LEVEL_WIDTH 5120 
#define LEVEL_HEIGHT 1200

#define FINGER_WIDTH 35
#define FINGER_HEIGHT 32
SDL_Rect clipsFinger[6];

/*Level One Blocks*/
#define BLOCK_WIDTH 64 
#define BLOCK_HEIGHT 64
SDL_Rect clipsBlock[29];
/*g3* = ground 3x3 block sprite * = location from top left to bottom right
i.e. 0 = top left and 1 = top middle etc.*/
enum {bush, vine, grass, g30, g31, g32, g10, g33, g34, g35, g11, g36, g37,
	g38, g00, tree0, tree1, tree2, g20, g21, tree3, tree4, tree5, tree6,
	tree7, tree8, rock0, rock1, rock2};

SDL_Rect clipsControls[12];
enum {wKey, aKey, sKey, dKey, upKey, dwKey, enKey, esKey, spKey, lClick, rClick,
        aim};

SDL_Surface* screen = NULL;

SDL_Window* window = NULL;

unsigned int SOMETIME = 0;
int quit = 0;

/*State Machine*/
typedef enum {mainmenu, pausemenu, controlsmenu, running} estate;
estate state = mainmenu;
estate prevState = mainmenu;

/*Global Surfaces*/
#define NUMSURF 35
SDL_Surface* surfaces[NUMSURF];
enum {bg, fingerSprite, mmFinger, mmTitle, mmSubtitle, mmNewGame, mmControls,
        mmExit, pmBG, pmResume, pmControls, pmExit, pmTitle, pmSubtitle, dot, 
        cloudOne, cloudTwo, cmTitle, cmSubtitle, cmBack, cmExit, cmControls, 
        cmUp, cmDw, cmEs, cmA, cmS, cmD, cmSp, cmEn, cmLC, cmRC, cmAim, 
        bgLevelOne, fgElements};

/*Font*/
TTF_Font *font40 = NULL;
TTF_Font *font20 = NULL;
SDL_Color textColor = {0, 0, 0};

SDL_Event event;

SDL_Rect camera = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

SDL_Surface* load_image(const char*);
SDL_Surface* SDL_ScaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height);
void apply_surface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
void init();
void load_files();
void clean_up();

typedef struct {
    int offSet, frame, status;
} Finger;

void showFinger(Finger *);

typedef struct {
} levelBlock;

void showLevelBlock(levelBlock *);

typedef struct {
    SDL_Rect dpos;
    int xVel, yVel;
} Dot;

void handle_input(Dot *, Finger *);
void move(Dot *);
void show(Dot *);
void set_camera(Dot *);

typedef struct {
    int startTicks;
    int pausedTicks;
    int paused;
    int started;
} Timer;

void start(Timer *);
void stop(Timer *);
void pause(Timer *);
void unpause(Timer *);
int get_ticks(Timer *);

typedef struct {
    int x, y;
    SDL_Surface * cloudType;
} Clouds;

void moveCloud(Clouds *);
void showCloud(Clouds *);
void init_cloud(Clouds *, SDL_Surface *);

#define C_BLOCK 200 
typedef struct {
    SDL_Rect pos;
    int face;
} Surface;

Surface SURF[C_BLOCK];
#endif
