#pragma once

#include "gfx.h"
//#include "unit.h"

#define BLOCKSCALE (TILESCALE * 2)
#define CHUNKANGTH 4
#define INVALIDUNIT -1
#define MAX_LEVELS 5
#define MAX_SAMPLES 128
#define MAX_UNITS 256
#define MAX_TILEUNITS 8

#define VALIDLEVEL (level)

void bindlevel(int depth);
void enablelevel();
void createlevels(int width, int height, int seed);
void emptylevel(int width, int height);
void renderlevel(int level, int xs, int ys, screen_t* screen);
void ticklevel();
void tickunits(aabb_t aabb);

//void rendertiles();
//void renderunits();

/*void renderbackground(int xs, int ys, screen_t* screen);
void renderlights(int xs, int ys, screen_t* screen);
void rendersprites(int xs, int ys, screen_t* screen);*/


int getdata(int x, int y);
int gettile(int x, int y);
/*refer_t* getunits(aabb_t aabb); */

void setdata(int x, int y, int value);
void settile(int x, int y, int tile, int value);

/*unit_u* getunit(refer_t id);
int hasunit(refer_t id);
refer_t* seekunits(const char* word, aabb_t aabb);

refer_t createunitid();
refer_t dropitem(refer_t item, int x, int y);
void efface(refer_t unit);
refer_t initiate(const char* word, int x, int y);
refer_t place(unit_u* unit);
refer_t relay(refer_t unit, level_t* other);*/
refer_t spawn(const char* word);
/*void boundunit(refer_t unit, int angth, aabb_t* aabb);*/
