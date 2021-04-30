#pragma once

#include "base.h"

#define MAX_TILEWORDS 128
#define MAX_DEIGNS 2
#define NOTILE 0

/*typedef enum {
 TILE_NOFLAG = 0x00,
 TILE_TOGRASS = 0x01,
 TILE_TOLAVA = 0x02,
 TILE_TOSAND = 0x04,
 TILE_TOWATER = 0x08,
 
 TILE_TOLIQUID = TILE_TOLAVA | TILE_TOWATER,
} tileflag_e;

typedef union {
 float floats[MAX_DEIGNS];
 int integers[MAX_DEIGNS];
 void* pointers[MAX_DEIGNS];
 refer_t referrals[MAX_DEIGNS];
 char* strings[MAX_DEIGNS];
} deign_t;

typedef struct {
 byte_t id;
 byte_t data;
} tile_t;

typedef struct {
 refer_t word;
 int x, y;
} vtile_t;

typedef struct {
 char* word;
 deign_t deign;
 tileflag_e flags;
} tileword_t;*/

typedef struct {
 refer_t id;
 char* word;
} tile_t;

typedef struct {
 
 byte_t* dirties;
 
 byte_t* tiles;
} chunk_t;



refer_t tileid(const char* word);
const char* tilename(refer_t id);
int flagtile(refer_t id, int flags);
int hasflags(int flags, refer_t id);

//int actontile(int x, int y, refer_t unit, refer_t item);
//int gettileglow(int x, int y);
//void stepontile(int x, int y, refer_t unit);
//void striketile(int x, int y, refer_t unit, int damage);
//int surpassable(int x, int y, refer_t unit);
//void touchtile(int x, int y, refer_t unit);
