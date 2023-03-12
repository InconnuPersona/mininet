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

enum cmd_e : int {
 CMD_ATTACK = 0x01,
 CMD_MENU = 0x02,
 CMD_MOVEDOWN = 0x04,
 CMD_MOVELEFT = 0x08,
 CMD_MOVERIGHT = 0x10,
 CMD_MOVEUP = 0x20,
};

struct Unit {
 bool exist;
 int id;
 int base; // index to unitdefs[]
 int x, y;
 int halfW, halfH;
 
 Unit();

 virtual void input(int cmds);

 int depth();

 sol::object getClass();

 bool moveStep(int xa, int ya);
 virtual bool move(int xa, int ya);

 virtual void touch(Unit* other);
 
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
 void getUnits(aabb_t bb, std::vector<Unit*>& units);

 //void drop();
 refer_t spawn(const char* word);

 void render(int xs, int ys, screen_t* screen);
 void tick();
 void tickUnits(aabb_t bb);

 bool valid();
};

extern LevelI level;

void enablelevel();
