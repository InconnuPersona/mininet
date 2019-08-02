#pragma once

#include "base.h"

#define MAX_TILEWORDS 128

#define NOTILE 0

typedef enum {
 TILEFLAG_PASSABLE = 0x01,
 TILEFLAG_TOGRASS = 0x02,
 TILEFLAG_TOLAVA = 0x04,
 TILEFLAG_TOSAND = 0x08,
 TILEFLAG_TOWATER = 0x10,
} tileflag_e;

typedef struct {
 byte_t id;
 byte_t data;
} tile_t;

typedef struct {
 int falsed; // whether the tileword is  
 char* word; // script
 char* name;
 //int extras[4];
 
 tileflag_e flags;
 int luminance;
} tileword_t;

int adjoinsliquid(refer_t tile);
//int flagged(tileflag_e flag, refer_t tile);
int luminance(refer_t tile);
int surpassable(refer_t tile);

refer_t tileid(const char* string);
const char* tilename(refer_t code);

//int tileflag();

//int interact(pliant_t*, refer_t item, int aim);
