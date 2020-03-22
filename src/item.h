#pragma once

#include "gfx.h"

#define MAX_COSTS 8
#define MAX_DETAILS 6
#define MAX_INVENTORY 64
#define MAX_ITEMS 64
#define MAX_PILES 128
#define MAX_RECIPES 64
#define MAX_RESOURCES 32
#define MAX_SURFACES 8
#define MAX_STOCKPILE 32

#define NOITEM 0
#define NOPILE 0
#define NOSURFACE 0

typedef enum {
 ITEM_UNIQUE = 0, // nonstackable item
 ITEM_POWGLOVE, // power glove item
 ITEM_RESOURCE, // food, wood, etc.
 ITEM_TOOL, // pick, axe, etc.
  ITEM_AXETYPE = ITEM_TOOL | 0x10,
  ITEM_HOETYPE = ITEM_TOOL | 0x10,
  ITEM_PICKAXETYPE = ITEM_TOOL | 0x10,
  ITEM_SWORDTYPE = ITEM_TOOL | 0x10,
  ITEM_SHOVELTYPE = ITEM_TOOL | 0x10,
 ITEM_PLACABLE, // furniture, lamp, etc.
} item_e;

typedef enum {
 VIEW_NONE = 0,
 VIEW_CONTAINER, // or STORAGE
 VIEW_CRAFT,
} surface_e;

typedef struct {
 char* name;
 int value;
} detail_t;

typedef struct {
 int id;
 char* word;
 item_e type;
 int source; // wood, rock, etc.
 int sprite;
 int color;
} item_t;

typedef struct {
 refer_t item;
 int count;
 int data[2];
} slot_t;

typedef struct {
 refer_t id;
 slot_t slots[MAX_INVENTORY];
} inventory_t, pile_t;

typedef struct {
 slot_t costs[MAX_COSTS];
 slot_t result;
} recipe_t;

typedef struct {
 int id;
 char* word;
 surface_e type;
 recipe_t recipes[MAX_RECIPES];
 refer_t items[MAX_INVENTORY];
} surface_t, view_t; // recipe interface and item access

void freepile(refer_t pile);
refer_t newpile();

refer_t getsurfacebyword(const char* word);
void rendersurface(refer_t menu, refer_t pliant);
void ticksurface(refer_t menu, refer_t pliant);

//void additem(refer_t item, int count, refer_t pile);
//int countitem(refer_t item, refer_t pile);
//int hasitem(refer_t item, int count, refer_t pile);
//int pullitem(refer_t item, int count, refer_t pile);
//void renderhotbaritem(refer_t item, int x, int y, screen_t* screen);
