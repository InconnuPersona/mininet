#pragma once

#include "base.h"

#define GAMETITLE "Mininet"
#define RENDERWIDTH 160
#define RENDERHEIGHT 120
#define SCALE 3
#define TILESCALE 8

typedef enum {
 MIRROR_X = 0x01,
 MIRROR_Y = 0x02,
} sprite_e;

typedef struct {
 int width, height;
 color_t* pixels;
} spritesheet_t;

typedef struct {
 int w, h, x, y;
 color_t* pixels;
 
 spritesheet_t sheet;
} screen_t;

int getcolor(int a, int b, int c, int d);

void loadspritesheet(spritesheet_t* sheet, const char* path);

void clearscreen(screen_t* screen, int color);
void createscreen(screen_t* screen, int width, int height, const char* icons);
void offsetscreen(screen_t* screen, int x, int y);
void overlayscreens(screen_t* screen, screen_t* other, int xa, int ya);
void renderframe(const char* title, int x0, int y0, int x1, int y1, screen_t* screen);
void renderlight(int x, int y, int radius, screen_t* screen);
void renderfont(const char* message, int x, int y, int color, screen_t* screen);
void rendersprite(int x, int y, int tile, int color, int flags, screen_t* screen);

void createview();
void closeview();
void fillpalette();
int hasfocus();
void updatetexture();
