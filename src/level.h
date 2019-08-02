#pragma once

#include "gfx.h"
#include "unit.h"
#include "tile.h"

#define CHUNKANGTH 4
#define INVALIDUNIT -1
#define LEVELSIZE 128
#define MAX_SAMPLES 128
#define MAX_TILEUNITS 4
#define MAX_UNITS 256

typedef struct level_s {
 int w, h;
 int depth;
 
 int dirtcolor, sandcolor, grasscolor;
 int monsterdensity;
 
 byte_t* dirties; // chunk bitmap
 
 tile_t* tiles;
 refer_t* tileunits;
 
 int spritecount;
 refer_t* sprites;
 
 unit_u units[MAX_UNITS];
} level_t;

void bindlevel(level_t* level);
void createlevel(int width, int height, int depth, level_t* parent);
void emptylevel(int width, int height, int depth);
void ticklevel();

void renderbackground(int xs, int ys, screen_t* screen);
void renderlights(int xs, int ys, screen_t* screen);
void rendersprites(int xs, int ys, screen_t* screen);

int getdata(int x, int y);
int gettile(int x, int y);

int getdepth();
int getunitid(refer_t unit);

refer_t* getunits(aabb_t aabb);
refer_t seekbond(int bond);
refer_t* seekclass(const char* word, aabb_t aabb);

void setdata(int x, int y, int value);
void settile(int x, int y, int tile, int value);

void efface(refer_t unit);
refer_t initiate(const char* unit, int x, int y);
refer_t place(unit_u* unit, int x, int y);
refer_t spawn(const char* unit);
void spawnfiends(int count);
refer_t summon(unit_u* unit);

void renderunit(unit_u* unit, level_t* level, screen_t* screen);
void tickunit(unit_u* unit, level_t* level);
