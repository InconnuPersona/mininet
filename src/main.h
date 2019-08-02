#pragma once

#include "gfx.h"
#include "item.h"
#include "level.h"
#include "menu.h"
#include "net.h"

#define MAX_GAMECLIENTS 16
#define MAX_LEVELS 5

#define TICKRATE 30
#define GAMERATE 20
#define GAMEVERSION 0x0000001

typedef enum {
 GAME_NONE = 0,
 GAME_CLIENT,
 GAME_HOST,
 GAME_PRIVATE,
} gametype_e;

typedef enum {
 MSG_ADDCLIENT,
 MSG_REMOVECLIENT,
 
 MSG_SYNCHRONIZE, // verifier
 MSG_CLIENT,
 MSG_PLAYSOUND,
 
 MSG_COMMAND,
 
 MSG_LEVELBITMAP, // send list of requested chunks
 
 MSG_UPDATECHUNK,
 MSG_UPDATECLIENT,
 MSG_UPDATEPILE,
 MSG_UPDATEUNIT,
 
 MSG_ITEMWORDS, // host referral array
 MSG_UNITWORDS,
} message_e;

typedef struct {
 refer_t being; // network client
 char* name;
 refer_t player;
 int level, pendinglevel;
 int deadtime, livetime;
 int finished; // won
} gameclient_t;

typedef struct {
 int open;
 int id, version;
 int ticks, type;
 gameclient_t clients[MAX_GAMECLIENTS];
 level_t levels[MAX_LEVELS];
 watch_t watch;
} game_t;

extern void clearkeys();
extern void pollevents();
extern void tickkeys();
extern void setlisten(int state);
extern int getaliasclicked(const char* name);
extern int getaliasdown(const char* name);
extern void alias(int key, const char* name);

void loadlibraries();
void closelibraries();

void enablegame();
//void endsession() or closesession()
void startsession(gametype_e type, char* name, char* address, int port);
void rendergame(screen_t* screen);
void tickgame();

void bindkeys();
void renderfocusnagger(screen_t* screen, int ticks);

void loadsounds();
void playsound(const char* sound);
void unloadsounds();
