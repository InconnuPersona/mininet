#pragma once

#include "gfx.h"
#include "bind.h"

#define CHUNKANGTH 4
#define INVALIDUNIT -1
#define MAX_LEVELS 5
#define MAX_SAMPLES 128
#define MAX_UNITS 256
#define MAX_TILEUNITS 8

#define VALIDLEVEL (level)

struct Unit {
 int id;
 int base; // index to units[]
 int x, y;
 int halfW, halfH;
 
 Unit();

 int depth();

 sol::object getClass();

 void touch(Unit* other);
 
 void test();
};

struct LevelI {
 void bind(int depth);
 

 int seed();
 int w();
 int h();
 int depth();

 //
 void generate(int w, int h, int seed);

 int getData(int x, int y);
 int getTile(int x, int y);
 
 void setData(int x, int y, int data);
 void setTile(int x, int y, int tile, int value);

 bool hasUnit(refer_t id);
 Unit* getUnit(refer_t id);

 //void drop();
 refer_t spawn(const char* word);

 void render(int xs, int ys, screen_t* screen);
 void tick();

 bool valid();
};

extern LevelI level;

void enablelevel();
