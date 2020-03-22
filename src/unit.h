#pragma once

#include "units.h"

#define MAX_ELEMENTS 32
#define MAX_UNITWORDS 64

#define ELEMENTSIZE sizeof(int)
#define NOUNIT 0

typedef union {
 int data[MAX_ELEMENTS];
 
 drop_t drop;
 fiend_t fiend;
 mob_t mob;
// movable_t movable;
 pliant_t pliant;
// trace_t trace;
 unit_t base;
} unit_u;

typedef struct {
 int extra;
 int id;
 char* name;
 unit_e super;
 char* word;
 
 // unit shared
 int halfheight, halfwidth;
 
 // mob shared
 int maxhealth;
 
 // movable only
 refer_t surface;
 
 // pliant only
 int maxstamina;
} unitword_t;

struct level_s;

void bindunitword(const char* word, refer_t bind);
refer_t designunit(const char* word, int extra);
int getclassdepth(unit_e super);
int getunitwidth(const char* word);

refer_t getclass(const char* word);
int isclass(unit_e super, refer_t word);

unitword_t* getunitword(int id);
unitword_t* getunitbyword(const char* word);

void createunit(unit_t* unit, const char* word);
