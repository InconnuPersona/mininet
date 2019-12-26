#pragma once

#include "base.h"

#define MAX_ELEMENTS 32
#define MAX_UNITWORDS 64

#define ELEMENTSIZE sizeof(int)
#define NOUNIT 0

#define CLASS_UNIT \
 refer_t id; \
 int extant; \
 refer_t word; \
 refer_t owner; \
 int x, y;

#define CLASS_DROP \
 CLASS_UNIT; \
 refer_t item; \
 float xa, ya, za; \
 float xx, yy, zz; \
 int aim, hurttime, lifetime, time; \
 int xk, yk;

#define CLASS_MOB \
 CLASS_UNIT; \
 refer_t pile; \
 int health, dir, xk, yk; \
 int hurted, ticked, swum, walked;

//#define CLASS_FIEND 
// CLASS_MOB; 
// int level, xa, ya;

// The command contains all movement and action command data while the medium of such acts is
// identified under the object; the target is the acted character, whether an item or mob.
#define CLASS_PLIANT \
 CLASS_MOB; \
 refer_t bond; \
 int commands; \
 refer_t object; \
 refer_t target; \
 refer_t inhand, item; \
 int aim, score; \
 int attacktime, breathtime, dodgetime, stairtime; \
 int stamina, tireddelay;

// item is the item which is the furniture, and taker is the player that is queued for pickup
// the taker is whoever is picking up the movable, while the user is whoever is engaged with it
//#define CLASS_MOVABLE 
// CLASS_UNIT; 
// refer_t taker; 
// refer_t user; 
// int push, pushtime;

// traces are not sent over the network, they are played only by the clients
//#define CLASS_TRACE 
// CLASS_UNIT; 
// int time;

typedef enum {
 ATT_FRIENDLY,
 ATT_HOSTILE, // air wizard, slime
  ATT_UNDEAD, // zombie, skeleton, etc.
 ATT_NEUTRAL, // player
  ATT_SPECTER, // spectator, ghost, invulnerable
} attitude_e;

// flag enum
typedef enum {
 CMD_NONE = 0,
 CMD_ATTACK = 0x01,
 CMD_CRAFT = 0x02,
 //CMD_INTERACT = 0x04, // NULL for player menu
 CMD_MENU = 0x04,
 CMD_MOVEUP = 0x08,
 CMD_MOVEDOWN = 0x10,
 CMD_MOVELEFT = 0x20,
 CMD_MOVERIGHT = 0x40,
} command_e;

typedef enum {
 SUPER_NULL = 0,
 SUPER_UNIT = 0x01,
 SUPER_DROP = 0x02,
 SUPER_MOVABLE = 0x04,
 SUPER_TRACE = 0x08,
 SUPER_MOB = 0x10,
//  SUPER_BEAST = SUPER_MOB | 1,
  SUPER_FIEND = SUPER_MOB | 2,
  SUPER_PLIANT = SUPER_MOB | 3,
} unit_e;

typedef struct {
 CLASS_DROP;
} drop_t;

typedef struct {
 CLASS_MOB;
} mob_t;

typedef struct {
 CLASS_PLIANT;
} pliant_t;

typedef struct {
 CLASS_UNIT;
} unit_t;

typedef union {
 int data[MAX_ELEMENTS];
 
 drop_t drop;
 mob_t mob;
 pliant_t pliant;
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
 //char* name;
 //refer_t surface;
 
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
