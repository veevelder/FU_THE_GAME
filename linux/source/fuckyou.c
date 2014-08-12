/*
Fuck You The Game
fuckyou.c
Licenced Under the MIT Licence so Fuck Off
Matt "The Big Fuck" Van Veldhuizen
Karl "The Little Fuck" Ott
*/

#include "../header/fuckyou.h"

SDL_Surface *
load_image(const char* filename) {
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load(filename);
    if(loadedImage != NULL) {
        optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
        SDL_FreeSurface(loadedImage);
        if(optimizedImage != NULL) {
            SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
        }
    }
    if(optimizedImage == NULL) {
        printf("failed to load %s\n", filename);
        exit(0);
    }
    return optimizedImage;
}

void 
apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface(source, clip, destination, &offset);
}

void 
init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("sdl failed to init\n");
        exit(0);
    }
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if(screen == NULL) {
        printf("screen failed to set\n");
        exit(0);
    }
    if(TTF_Init() == -1) {
        printf("font faild to init\n");
        exit(0);
    }
    SDL_WM_SetCaption("FUCK YOU THE GAME", NULL);
}

void
load_files() {
    /*Global Surfaces*/
    surfaces[bg] = load_image("assets/main_menu/bg_menu.png");
    surfaces[fingerSprite] = load_image("assets/main_menu/finger_sprite.png");

    /*Main Menu Files*/
    surfaces[mmFinger] = load_image("assets/main_menu/middlefinger.png");
    surfaces[mmTitle] = load_image("assets/main_menu/title.png");
    surfaces[mmSubtitle] = load_image("assets/main_menu/subtitle.png");

    /*Pause Menu Files*/
    surfaces[pmBG] = load_image("assets/pause_menu/pmBG.png");
    surfaces[pmTitle] = load_image("assets/pause_menu/pmTitle.png");
    surfaces[pmSubtitle] = load_image("assets/pause_menu/pmSubtitle.png");

    /*Game Files*/
    surfaces[dot] = load_image("assets/images/dot.png");
    surfaces[cloudOne] = load_image("assets/clouds/cumulus-big1.png");
    surfaces[cloudTwo] = load_image("assets/clouds/cumulus-big3.png");
    surfaces[bgLevelOne] = load_image("assets/level_one/bg_levelOne.png");
    surfaces[fgLevelOne] = load_image("assets/level_one/fg_levelOne.png");

    /*Controls Menu*/
    surfaces[cmTitle] = load_image("assets/controls_menu/cmTitle.png");
    surfaces[cmSubtitle] = load_image("assets/controls_menu/cmSubtitle.png");
    surfaces[cmControls] = load_image("assets/controls_menu/cmControls.png");
}

void
set_font() {
    font20 = TTF_OpenFont("assets/fonts/ARCADEPI.TTF", 20);
    font40 = TTF_OpenFont("assets/fonts/ARCADEPI.TTF", 40);
    if(font20 == NULL || font40 == NULL) {
        printf("font failed to open\n");
        exit(0);
    }
}

void
apply_text(int x, int y, TTF_Font *myFont, char * text, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
    source = TTF_RenderText_Solid(myFont, text, textColor);
    if(source == NULL) {
        printf("failed to print %s\n", text);
        exit(0);
    }
    apply_surface(x, y, source, destination, clip);
}

void
clean_up() {
    int i = 0;
    for(; i < NUMSURF; i++) {
        SDL_FreeSurface(surfaces[i]);
    }
    TTF_CloseFont(font20);
    TTF_CloseFont(font40);
    TTF_Quit();
    SDL_Quit();
}

/* Dot functions */
void 
handle_input(Dot * x, Finger * f) {
    if (event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
        case SDLK_w:
            x->yVel -= DOT_HEIGHT/2; 
            break;
        case SDLK_a:
            x->xVel -= DOT_HEIGHT/2; 
            break;
        case SDLK_s:
            x->yVel += DOT_HEIGHT/2; 
            break;
        case SDLK_d:
            x->xVel += DOT_HEIGHT/2; 
            break;
        case SDLK_UP: 
            if(state == controlsmenu) {
                if(f->offSet != 355) {
                    f->offSet -= 50;
                }
            }
            else {
                if(f->offSet != 305) {
                    f->offSet -= 50;
                }
            }
            break;
        case SDLK_DOWN:
            if(f->offSet != 405) {
                f->offSet += 50;
            }
            break;
        case SDLK_ESCAPE: 
            if(state == running) { 
                state = pausemenu; 
            }
            break;
        case SDLK_RETURN:
            /*New Game*/
            if(f->offSet == 305 && state == mainmenu) {
                f->offSet = 305;
                state = running;
            }
            /*Resume Game*/
            if(f->offSet == 305 && state == pausemenu) {
                f->offSet = 305;
                state = running;
            }
            /*Back*/
            if(f->offSet == 355 && state == controlsmenu) {
                f->offSet = 305;
                state = prevState;
            }
            /*Controls*/
            if(f->offSet == 355) {
                f->offSet = 355;
                prevState = state;
                state = controlsmenu;
            }
            /*Exit*/
            if(f->offSet == 405) {
                quit = 1;
            }
            break;
        } 
    }
    else if(event.type == SDL_KEYUP) {
        switch(event.key.keysym.sym) {
        case SDLK_w: 
            x->yVel += DOT_HEIGHT/2; 
            break;
        case SDLK_s: 
            x->yVel -= DOT_HEIGHT/2; 
            break;
        case SDLK_a: 
            x->xVel += DOT_HEIGHT/2; 
            break;
        case SDLK_d: 
            x->xVel -= DOT_HEIGHT/2; 
            break;
        }
    }
}

int
check_collision(SDL_Rect A) {
    int i;
    for(i = 0; i < 19; i++) {

        if((A.x + A.w) > SURF[i].pos.x && (A.y + A.h) > SURF[i].pos.y && 
            A.x < (SURF[i].pos.x + SURF[i].pos.w) &&
            A.y < (SURF[i].pos.y + SURF[i].pos.h)) {
            return 0;
        }
    }
    return 1;
}

void 
move(Dot * d) {
    d->dpos.x += d->xVel;
    if((d->dpos.x<0) || (d->dpos.x+DOT_WIDTH > LEVEL_WIDTH) || check_collision(d->dpos) == 0) {
        d->dpos.x -= d->xVel;
    }
    d->dpos.y += d->yVel;
    if((d->dpos.y<0) || (d->dpos.y+DOT_HEIGHT > LEVEL_HEIGHT) || check_collision(d->dpos) == 0) {
        d->dpos.y -= d->yVel;
    }
}

void 
show(Dot * x) {
    apply_surface(x->dpos.x - camera.x, x->dpos.y - camera.y, surfaces[dot], screen, NULL);
}

void 
set_camera(Dot * x) {
    camera.x = (x->dpos.x + DOT_WIDTH/2) - SCREEN_WIDTH/2;
    camera.y = (x->dpos.y + DOT_HEIGHT/2) - SCREEN_HEIGHT/2;

    if(camera.x < 0) {
        camera.x = 0;
    }
    if(camera.y < 0) {
        camera.y = 0;
    }
    if(camera.x > LEVEL_WIDTH - camera.w) {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if(camera.y > LEVEL_HEIGHT - camera.h) {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}
/*Cloud Functions*/
void 
init_cloud(Clouds * z, SDL_Surface * pic) {
    z->x = rand() % 1050 + 400;
    z->y = rand() % 600 + 1;
    z->cloudType = pic;    
}

void 
moveCloud(Clouds * cloud) {
    cloud->x--;
    if(cloud->x<-200) {
        cloud->x = rand() % 1050 + 850;
        cloud->y = rand() % 600 + 1;
    }
    if(cloud->x%200 == 0) {
        cloud->y -= 2;
    }
    if(cloud->x%200 == 100) {
        cloud->y += 2;
    }
}

void 
showCloud(Clouds * x) {
    apply_surface(x->x, x->y, x->cloudType, screen, NULL);
}

/* Timer functions */
void 
start(Timer * x) {
    x->started = 1;
    x->paused = 0;
    x->startTicks = SDL_GetTicks();
}

void 
stop(Timer * x) {
    x->started = 0;
    x->paused = 0;
}

void 
pause(Timer * x) {
    if((x->started == 1) && (x->paused == 0)) {
        x->paused = 1;
        x->pausedTicks = SDL_GetTicks() - x->startTicks;
    }
}

void 
unpause(Timer * x) {
    if(x->paused == 1) {
        x->paused = 0;
        x->startTicks = SDL_GetTicks() - x->pausedTicks;
        x->pausedTicks = 0;
    }
}

int 
get_ticks(Timer * x) {
    if(x->started == 1) {
        if(x->paused == 1) {
            return x->pausedTicks;
        } 
        else {
            return SDL_GetTicks() - x->startTicks;
        }
    }
    return 0;
}

void 
set_clips() {
    /*Finger Animation Stuff*/
    clipsFinger[0].x = 0;
    clipsFinger[0].y = 0;
    clipsFinger[0].w = FINGER_WIDTH;
    clipsFinger[0].h = FINGER_HEIGHT;

    clipsFinger[1].x = FINGER_WIDTH;
    clipsFinger[1].y = 0;
    clipsFinger[1].w = FINGER_WIDTH;
    clipsFinger[1].h = FINGER_HEIGHT;

    clipsFinger[2].x = FINGER_WIDTH * 2;
    clipsFinger[2].y = 0;
    clipsFinger[2].w = FINGER_WIDTH;
    clipsFinger[2].h = FINGER_HEIGHT;

    clipsFinger[3].x = FINGER_WIDTH * 3;
    clipsFinger[3].y = 0;
    clipsFinger[3].w = FINGER_WIDTH;
    clipsFinger[3].h = FINGER_HEIGHT;

    clipsFinger[4].x = FINGER_WIDTH * 4;
    clipsFinger[4].y = 0;
    clipsFinger[4].w = FINGER_WIDTH;
    clipsFinger[4].h = FINGER_HEIGHT;

    clipsFinger[5].x = FINGER_WIDTH * 5;
    clipsFinger[5].y = 0;
    clipsFinger[5].w = FINGER_WIDTH;
    clipsFinger[5].h = FINGER_HEIGHT;

    /*Control Clips*/
    clipsControls[wKey].x = 0;
    clipsControls[wKey].y = 0;
    clipsControls[wKey].w = BLOCK_WIDTH;
    clipsControls[wKey].h = BLOCK_HEIGHT;

    clipsControls[aKey].x = BLOCK_WIDTH;
    clipsControls[aKey].y = 0;
    clipsControls[aKey].w = BLOCK_WIDTH;
    clipsControls[aKey].h = BLOCK_HEIGHT;

    clipsControls[sKey].x = BLOCK_WIDTH * 2;
    clipsControls[sKey].y = 0;
    clipsControls[sKey].w = BLOCK_WIDTH;
    clipsControls[sKey].h = BLOCK_HEIGHT;

    clipsControls[dKey].x = BLOCK_WIDTH * 3;
    clipsControls[dKey].y = 0;
    clipsControls[dKey].w = BLOCK_WIDTH;
    clipsControls[dKey].h = BLOCK_HEIGHT;

    clipsControls[upKey].x = 0;
    clipsControls[upKey].y = BLOCK_HEIGHT;
    clipsControls[upKey].w = BLOCK_WIDTH;
    clipsControls[upKey].h = BLOCK_HEIGHT;

    clipsControls[dwKey].x = BLOCK_WIDTH;
    clipsControls[dwKey].y = BLOCK_HEIGHT;
    clipsControls[dwKey].w = BLOCK_WIDTH;
    clipsControls[dwKey].h = BLOCK_HEIGHT;

    clipsControls[enKey].x = BLOCK_WIDTH * 2;
    clipsControls[enKey].y = BLOCK_HEIGHT;
    clipsControls[enKey].w = BLOCK_WIDTH * 2;
    clipsControls[enKey].h = BLOCK_HEIGHT;

    clipsControls[esKey].x = 0;
    clipsControls[esKey].y = BLOCK_HEIGHT * 2;
    clipsControls[esKey].w = BLOCK_WIDTH;
    clipsControls[esKey].h = BLOCK_HEIGHT;

    clipsControls[spKey].x = BLOCK_WIDTH;
    clipsControls[spKey].y = BLOCK_HEIGHT * 2;
    clipsControls[spKey].w = BLOCK_WIDTH * 3;
    clipsControls[spKey].h = BLOCK_HEIGHT;

    clipsControls[lClick].x = 0;
    clipsControls[lClick].y = BLOCK_HEIGHT * 3;
    clipsControls[lClick].w = BLOCK_WIDTH * 2;
    clipsControls[lClick].h = BLOCK_HEIGHT * 3;

    clipsControls[rClick].x = BLOCK_HEIGHT * 2;
    clipsControls[rClick].y = BLOCK_HEIGHT * 3;
    clipsControls[rClick].w = BLOCK_WIDTH * 2;
    clipsControls[rClick].h = BLOCK_HEIGHT * 3;

    clipsControls[aim].x = 0;
    clipsControls[aim].y = BLOCK_HEIGHT * 6;
    clipsControls[aim].w = BLOCK_WIDTH * 2;
    clipsControls[aim].h = BLOCK_HEIGHT * 4;

    /*level one clips*/
    clipsBlock[bush].x = BLOCK_WIDTH;
    clipsBlock[bush].y = 0;
    clipsBlock[bush].w = BLOCK_WIDTH;
    clipsBlock[bush].h = BLOCK_HEIGHT;

    clipsBlock[vine].x = BLOCK_WIDTH * 2;
    clipsBlock[vine].y = 0;
    clipsBlock[vine].w = BLOCK_WIDTH;
    clipsBlock[vine].h = BLOCK_HEIGHT;

    clipsBlock[grass].x = BLOCK_WIDTH * 4;
    clipsBlock[grass].y = 0;
    clipsBlock[grass].w = BLOCK_WIDTH;
    clipsBlock[grass].h = BLOCK_HEIGHT;

    clipsBlock[g30].x = 0;
    clipsBlock[g30].y = BLOCK_HEIGHT;
    clipsBlock[g30].w = BLOCK_WIDTH;
    clipsBlock[g30].h = BLOCK_HEIGHT;

    clipsBlock[g31].x = BLOCK_WIDTH;
    clipsBlock[g31].y = BLOCK_HEIGHT;
    clipsBlock[g31].w = BLOCK_WIDTH;
    clipsBlock[g31].h = BLOCK_HEIGHT;

    clipsBlock[g32].x = BLOCK_WIDTH * 2;
    clipsBlock[g32].y = BLOCK_HEIGHT;
    clipsBlock[g32].w = BLOCK_WIDTH;
    clipsBlock[g32].h = BLOCK_HEIGHT;

    clipsBlock[g10].x = BLOCK_WIDTH * 7;
    clipsBlock[g10].y = BLOCK_HEIGHT;
    clipsBlock[g10].w = BLOCK_WIDTH;
    clipsBlock[g10].h = BLOCK_HEIGHT;

    clipsBlock[g33].x = 0;
    clipsBlock[g33].y = BLOCK_HEIGHT * 2;
    clipsBlock[g33].w = BLOCK_WIDTH;
    clipsBlock[g33].h = BLOCK_HEIGHT;

    clipsBlock[g34].x = BLOCK_WIDTH;
    clipsBlock[g34].y = BLOCK_HEIGHT * 2;
    clipsBlock[g34].w = BLOCK_WIDTH;
    clipsBlock[g34].h = BLOCK_HEIGHT;

    clipsBlock[g35].x = BLOCK_WIDTH * 2;
    clipsBlock[g35].y = BLOCK_HEIGHT * 2;
    clipsBlock[g35].w = BLOCK_WIDTH;
    clipsBlock[g35].h = BLOCK_HEIGHT;

    clipsBlock[g11].x = BLOCK_WIDTH * 7;
    clipsBlock[g11].y = BLOCK_HEIGHT * 2;
    clipsBlock[g11].w = BLOCK_WIDTH;
    clipsBlock[g11].h = BLOCK_HEIGHT;

    clipsBlock[g36].x = 0;
    clipsBlock[g36].y = BLOCK_HEIGHT * 3;
    clipsBlock[g36].w = BLOCK_WIDTH;
    clipsBlock[g36].h = BLOCK_HEIGHT;

    clipsBlock[g37].x = BLOCK_WIDTH;
    clipsBlock[g37].y = BLOCK_HEIGHT * 3;
    clipsBlock[g37].w = BLOCK_WIDTH;
    clipsBlock[g37].h = BLOCK_HEIGHT;

    clipsBlock[g38].x = BLOCK_WIDTH * 2;
    clipsBlock[g38].y = BLOCK_HEIGHT * 3;
    clipsBlock[g38].w = BLOCK_WIDTH;
    clipsBlock[g38].h = BLOCK_HEIGHT;

    clipsBlock[g00].x = BLOCK_WIDTH * 7;
    clipsBlock[g00].y = BLOCK_HEIGHT * 3;
    clipsBlock[g00].w = BLOCK_WIDTH;
    clipsBlock[g00].h = BLOCK_HEIGHT;

    clipsBlock[tree0].x = 0;
    clipsBlock[tree0].y = BLOCK_HEIGHT * 4;
    clipsBlock[tree0].w = BLOCK_WIDTH;
    clipsBlock[tree0].h = BLOCK_HEIGHT;

    clipsBlock[tree1].x = BLOCK_WIDTH;
    clipsBlock[tree1].y = BLOCK_HEIGHT * 4;
    clipsBlock[tree1].w = BLOCK_WIDTH;
    clipsBlock[tree1].h = BLOCK_HEIGHT;

    clipsBlock[tree2].x = BLOCK_WIDTH * 2;
    clipsBlock[tree2].y = BLOCK_HEIGHT * 4;
    clipsBlock[tree2].w = BLOCK_WIDTH;
    clipsBlock[tree2].h = BLOCK_HEIGHT;

    clipsBlock[g20].x = BLOCK_WIDTH * 6;
    clipsBlock[g20].y = BLOCK_HEIGHT * 4;
    clipsBlock[g20].w = BLOCK_WIDTH;
    clipsBlock[g20].h = BLOCK_HEIGHT;

    clipsBlock[g21].x = BLOCK_WIDTH * 7;
    clipsBlock[g21].y = BLOCK_HEIGHT * 4;
    clipsBlock[g21].w = BLOCK_WIDTH;
    clipsBlock[g21].h = BLOCK_HEIGHT;

    clipsBlock[tree3].x = 0;
    clipsBlock[tree3].y = BLOCK_HEIGHT * 5;
    clipsBlock[tree3].w = BLOCK_WIDTH;
    clipsBlock[tree3].h = BLOCK_HEIGHT;

    clipsBlock[tree4].x = BLOCK_WIDTH;
    clipsBlock[tree4].y = BLOCK_HEIGHT * 5;
    clipsBlock[tree4].w = BLOCK_WIDTH;
    clipsBlock[tree4].h = BLOCK_HEIGHT;

    clipsBlock[tree5].x = BLOCK_WIDTH * 2;
    clipsBlock[tree5].y = BLOCK_HEIGHT * 5;
    clipsBlock[tree5].w = BLOCK_WIDTH;
    clipsBlock[tree5].h = BLOCK_HEIGHT;

    clipsBlock[tree6].x = 0;
    clipsBlock[tree6].y = BLOCK_HEIGHT * 6;
    clipsBlock[tree6].w = BLOCK_WIDTH;
    clipsBlock[tree6].h = BLOCK_HEIGHT;

    clipsBlock[tree7].x = BLOCK_WIDTH;
    clipsBlock[tree7].y = BLOCK_HEIGHT * 6;
    clipsBlock[tree7].w = BLOCK_WIDTH;
    clipsBlock[tree7].h = BLOCK_HEIGHT;

    clipsBlock[tree8].x = BLOCK_WIDTH * 2;
    clipsBlock[tree8].y = BLOCK_HEIGHT * 6;
    clipsBlock[tree8].w = BLOCK_WIDTH;
    clipsBlock[tree8].h = BLOCK_HEIGHT;

    clipsBlock[rock0].x = 0;
    clipsBlock[rock0].y = BLOCK_HEIGHT * 7;
    clipsBlock[rock0].w = BLOCK_WIDTH;
    clipsBlock[rock0].h = BLOCK_HEIGHT;

    clipsBlock[rock1].x = BLOCK_WIDTH;
    clipsBlock[rock1].y = BLOCK_HEIGHT * 7;
    clipsBlock[rock1].w = BLOCK_WIDTH;
    clipsBlock[rock1].h = BLOCK_HEIGHT;

    clipsBlock[rock2].x = BLOCK_WIDTH * 2;
    clipsBlock[rock2].y = BLOCK_HEIGHT * 7;
    clipsBlock[rock2].w = BLOCK_WIDTH;
    clipsBlock[rock2].h = BLOCK_HEIGHT;
}

void
showFinger(Finger * f) {
    int local = SDL_GetTicks();
    if(f->status == 1 && (local - SOMETIME > 100)) {
            SOMETIME = local;
            f->frame++;
    }
    if(f->status == 0 && (local - SOMETIME > 100)) {
            SOMETIME = local;
            f->frame--;
    }
    if(f->frame == 5) {
        f->status = 0;
    }
    if(f->frame == 0) {
        f->status = 1;
    }

    apply_surface(300, f->offSet, surfaces[fingerSprite], screen, &clipsFinger[f->frame]);
}

void
showLevelBlock(levelBlock * l) {
	/*random generate map here?*/
    apply_surface(0, 0, surfaces[fgLevelOne], surfaces[bgLevelOne], &clipsBlock[bush]);
}

int 
main(int argc, char* args[]) {
    init();
    load_files();
    set_font();

    /*Seeding Random Number*/
    srand(time(NULL));

    SOMETIME = SDL_GetTicks();
    Dot myDot;
    myDot.dpos.x = LEVEL_WIDTH - (SCREEN_WIDTH/2);
    myDot.dpos.y = LEVEL_HEIGHT - DOT_HEIGHT - 192;
    myDot.dpos.w = DOT_WIDTH;
    myDot.dpos.h = DOT_HEIGHT;
    myDot.xVel = 0;
    myDot.yVel = 4;

    Timer fps;
    fps.startTicks = 0;
    fps.pausedTicks = 0;
    fps.paused = 0;
    fps.started = 0;

    Clouds oneCloud, twoCloud, threeCloud, fourCloud, fiveCloud, sixCloud;
    init_cloud(&oneCloud, surfaces[cloudOne]);
    init_cloud(&twoCloud, surfaces[cloudTwo]);
    init_cloud(&threeCloud, surfaces[cloudOne]);
    init_cloud(&fourCloud, surfaces[cloudTwo]);
    init_cloud(&fiveCloud, surfaces[cloudOne]);
    init_cloud(&sixCloud, surfaces[cloudTwo]);

    Finger f;
    f.offSet = 305;
    f.frame = 0;
    f.status = 1;

    levelBlock l;
    l.offSet = 305;
    l.frame = 0;
    l.status = 1;
    set_clips();

    while(quit == 0) {
        start(&fps);
        while(SDL_PollEvent(&event)) {
            handle_input(&myDot, &f);
            if(event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        switch(state) {
        case mainmenu: 
            apply_surface(0, 0, surfaces[bg], screen, &camera);
            showCloud(&oneCloud);
            showCloud(&twoCloud);
            showCloud(&threeCloud);
            showCloud(&fourCloud);
            showCloud(&fiveCloud);
            showCloud(&sixCloud);
            moveCloud(&oneCloud);
            moveCloud(&twoCloud);
            moveCloud(&threeCloud);
            moveCloud(&fourCloud);
            moveCloud(&fiveCloud);
            moveCloud(&sixCloud);
            showFinger(&f);
            apply_surface(15, 20, surfaces[mmTitle], screen, &camera);
            apply_surface(520, 20, surfaces[mmSubtitle], screen, &camera);
            apply_surface(400, 220, surfaces[mmFinger], screen, &camera);
            apply_text(61, 300, font40, "New Game", surfaces[mmNewGame], screen, &camera);
            apply_text(89, 350, font40, "Controls", surfaces[mmControls], screen, &camera);
            apply_text(201, 400, font40, "Exit", surfaces[mmExit], screen, &camera);
            break;
        case pausemenu:
            apply_surface(0, 0, surfaces[bg], screen, NULL);
            showCloud(&oneCloud);
            showCloud(&twoCloud);
            showCloud(&threeCloud);
            showCloud(&fourCloud);
            showCloud(&fiveCloud);
            showCloud(&sixCloud);
            moveCloud(&oneCloud);
            moveCloud(&twoCloud);
            moveCloud(&threeCloud);
            moveCloud(&fourCloud);
            moveCloud(&fiveCloud);
            moveCloud(&sixCloud);
            apply_surface(15, 20, surfaces[pmTitle], screen, NULL);
            apply_surface(550, 90, surfaces[pmSubtitle], screen, NULL);
            showFinger(&f);
            apply_text(125, 300, font40, "Resume", surfaces[pmResume], screen, NULL);
            apply_text(89, 350, font40, "Controls", surfaces[pmControls], screen, NULL);
            apply_text(201, 400, font40, "Exit", surfaces[pmExit], screen, NULL);
            break;
        case controlsmenu:
            apply_surface(0, 0, surfaces[bg], screen, NULL);
            showCloud(&oneCloud);
            showCloud(&twoCloud);
            showCloud(&threeCloud);
            showCloud(&fourCloud);
            showCloud(&fiveCloud);
            showCloud(&sixCloud);
            moveCloud(&oneCloud);
            moveCloud(&twoCloud);
            moveCloud(&threeCloud);
            moveCloud(&fourCloud);
            moveCloud(&fiveCloud);
            moveCloud(&sixCloud);
            showFinger(&f);
            apply_surface(15, 20, surfaces[cmTitle], screen, NULL);
            apply_text(185, 350, font40, "Back", surfaces[cmBack], screen, NULL);
            apply_text(201, 400, font40, "Exit", surfaces[cmExit], screen, NULL);
            apply_text(625, 355, font20, "Finger Up", surfaces[cmUp], screen, NULL);
            apply_surface(555, 332, surfaces[cmControls], screen, &clipsControls[upKey]);
            apply_text(625, 419, font20, "Finger Down", surfaces[cmDw], screen, NULL);
            apply_surface(555, 396, surfaces[cmControls], screen, &clipsControls[dwKey]);
            apply_text(625, 483, font20, "Pause", surfaces[cmEs], screen, NULL);
            apply_surface(555, 460, surfaces[cmControls], screen, &clipsControls[esKey]);
            apply_text(413, 355, font20, "Move Left", surfaces[cmA], screen, NULL);
            apply_surface(345, 332, surfaces[cmControls], screen, &clipsControls[aKey]);
            apply_text(413, 419, font20, "Crouch", surfaces[cmS], screen, NULL);
            apply_surface(345, 396, surfaces[cmControls], screen, &clipsControls[sKey]);
            apply_text(413, 483, font20, "Move Right", surfaces[cmD], screen, NULL);
            apply_surface(345, 460, surfaces[cmControls], screen, &clipsControls[dKey]);
            apply_text(413, 547, font20, "Jump", surfaces[cmSp], screen, NULL);
            apply_surface(217, 524, surfaces[cmControls], screen, &clipsControls[spKey]);
            apply_text(625, 547, font20, "Finger Select", surfaces[cmEn], screen, NULL);
            apply_surface(491, 524, surfaces[cmControls], screen, &clipsControls[enKey]);
            apply_text(185, 189, font20, "Shoot", surfaces[cmLC], screen, NULL);
            apply_surface(50, 120, surfaces[cmControls], screen, &clipsControls[lClick]);
            apply_text(415, 189, font20, "Stab", surfaces[cmRC], screen, NULL);
            apply_surface(280, 120, surfaces[cmControls], screen, &clipsControls[rClick]);
            apply_text(634, 189, font20, "Aim", surfaces[cmAim], screen, NULL);
            apply_surface(500, 120, surfaces[cmControls], screen, &clipsControls[aim]);
            apply_surface(500, 20, surfaces[cmSubtitle], screen, NULL);
            break;
        case running:
            move(&myDot);
            set_camera(&myDot);
            apply_surface(0, 0, surfaces[bgLevelOne], screen, &camera);
            showCloud(&oneCloud);
            showCloud(&twoCloud);
            showCloud(&threeCloud);
            showCloud(&fourCloud);
            showCloud(&fiveCloud);
            showCloud(&sixCloud);
            moveCloud(&oneCloud);
            moveCloud(&twoCloud);
            moveCloud(&threeCloud);
            moveCloud(&fourCloud);
            moveCloud(&fiveCloud);
            moveCloud(&sixCloud);
            showLevelBlock(&l);
            show(&myDot);
            break;
        }
        if(SDL_Flip(screen) == -1) {
            return 1;
        }
        if(get_ticks(&fps) < 1000 / FRAMES_PER_SECOND) {
            SDL_Delay((1000/FRAMES_PER_SECOND) - get_ticks(&fps));
        }
    }
    clean_up();
    return 0;
}

