#pragma once

#include "base.h"

#define MAX_ELEMENTS 32
#define MAX_UNITWORDS 64

#define ELEMENTSIZE sizeof(int)
#define NOUNIT 0

// bond is the client to which the entity is bound; owner refers to a master bond not a bonded being; 
// a pile is the inventory of the being (it will drop this after death presumably and may be used by the player)
#define CLASS_UNIT \
 int id; \
 refer_t word; \
 refer_t owner; \
 refer_t bond; \
 int extant; \
 int x, y;

//#define CLASS_DROP 
// CLASS_UNIT; 
// refer_t item; 
// float xa, ya, za; 
// float xx, yy, zz; 
// int lifetime;

#define CLASS_MOB \
 CLASS_UNIT; \
 refer_t pile; \
 int health, dir, xk, yk; \
 int hurt, tick, swim, walk;

//#define CLASS_FIEND 
// CLASS_MOB; 
// int level, xa, ya;

#define CLASS_PLIANT \
 CLASS_MOB; \
 refer_t inhand, item; \
 int aim, score; \
 int attacktime, breathtime, dodgetime, stairtime; \
 int stamina, rechargedelay;

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
 CMD_INTERACT = 0x04,
 CMD_MOVEUP = 0x08,
 CMD_MOVEDOWN = 0x10,
 CMD_MOVELEFT = 0x20,
 CMD_MOVERIGHT = 0x40,
} command_e;

typedef enum {
 SUPER_NULL = 0,
 SUPER_UNIT,
 SUPER_DROP,
 SUPER_MOB,
//  SUPER_BEAST,
  SUPER_FIEND,
  SUPER_PLIANT,
 SUPER_MOVABLE,
 SUPER_TRACE,
} class_e;

//typedef struct {
// CLASS_DROP;
//} drop_t;

//typedef struct {
// CLASS_FIEND;
//} fiend_t;

typedef struct {
 CLASS_MOB;
} mob_t;

//typedef struct {
// CLASS_MOVABLE;
//} movable_t;

typedef struct {
 CLASS_PLIANT;
} pliant_t;

//typedef struct {
// CLASS_TRACE;
//} trace_t;

typedef struct {
 CLASS_UNIT;
} unit_t;

typedef union {
 int data[MAX_ELEMENTS];
 
// drop_t drop;
// fiend_t fiend;
 mob_t mob;
// movable_t movable;
 pliant_t pliant;
// trace_t trace;
 unit_t base;
} unit_u;

typedef struct {
 // shared
 int id;
 int super; // superclass
 char* word; // unitword
 int extra; // extra unit data length
 
 // unit shared "unit"
 int halfheight, halfwidth;
 
 // mob only "mob"
 attitude_e attitude;
 int maxhealth;
 
 // movable only "movable"
// char* name;
// refer_t surface;
 
 // player only "pliant"
 int maxstamina;
} unitword_t, entity_t;

refer_t unitid(const char* word);
char* unitname(refer_t word);
char* unitword(refer_t word);
char* unitwordname(const char* word);

void blankunit(unit_u* unit, refer_t word);
