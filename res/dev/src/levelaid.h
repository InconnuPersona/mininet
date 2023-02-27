#pragma once

#include "level.h"
#include "unituse.h"

/*#define CHUNKANGTH 4
#define INVALIDUNIT -1
#define MAX_TILEUNITS 8

#define GETTILEUNIT(I, X, Y, Level) \
 (Level->tileunits[I + (X + Y * Level->w) * MAX_TILEUNITS])

#define GETUNIT(Index, Level) \
 (Level->units[Index].base)

typedef struct {
 int level;
 short x, y;
 tile_t tiles[CHUNKANGTH * CHUNKANGTH];
} chunk_t;

extern void rendertile(int x, int y, screen_t* screen);
extern void renderunit(refer_t unit, screen_t* screen);
extern void ticktile(int x, int y);
extern void tickunit(refer_t unit);

int begetunit(const char* word, level_t* level);
int calltilemethod(const char* method, int x, int y, const char* format, ...);
void cleanlevel(level_t* level);
void dirtychunk(int x, int y, level_t* level);
int emptyunit(level_t* level);
void generate(level_t* level);
void inserttileunit(refer_t unit, int x, int y, level_t* level);
int intersect(int unit, aabb_t bb, level_t* level);
int isdirtychunk(int x, int y, level_t* level);
int readchunk(int x, int y, chunk_t* chunk, level_t* level);
void removetileunit(refer_t unit, int x, int y, level_t* level);
int seekunit(refer_t id, level_t* level);
void setchunk(chunk_t* chunk, level_t* level);
void sortandrender(refer_t* units, int count, level_t* level, screen_t* screen);
void spawnfiends(int count, level_t* level);
int spritesort(const void* v1, const void* v2);*/
