#pragma once

#include "item.h"
#include "level.h"
#include "menu.h"
#include "net.h"

#define GAMESMARKER 0x4d494e49 // mini in hexadecimal
#define LEVELANGTH 128
#define MAX_GAMECLIENTS 16
#define MAX_GAMESENDLENGTH 128
#define MAX_LEVELS 5
#define MAX_NAMELENGTH 16
#define LOCALCLIENT 0
#define SPAWNLEVEL 3

//typedef enum {
// CMD_COMMAND,
// CMD_ORDER,
//} command_e;

typedef enum {
 MSG_NONE,
 
 // host messages
 
 MSG_ADDCLIENT,
 MSG_REMOVECLIENT,
 
 MSG_GAMEDATA,
 MSG_SYNCHRONIZE,
 MSG_PLAYSOUND,
 MSG_POSTCLIENTS,
 
 // ingame messages
 
 MSG_COMMAND,
 MSG_POSTCHUNK,
 MSG_POSTITEMWORDS,
 MSG_POSTPILE,
 MSG_POSTUNIT
} message_e;

typedef struct {
 int bind;
 int id;
 int entity, level;
 
 char name[MAX_NAMELENGTH];
 
 int deadtime, livetime;
 int inmenu,  finished; // won game
} gameclient_t;

typedef struct {
 short length;
 short type;
 int sender;
 byte_t data[];
} gamesend_t;

typedef struct {
 int open;
 int marker, checksum;
 int id, version;
 int ticks, type;
 int self;
 
 gameclient_t clients[MAX_GAMECLIENTS];
 level_t levels[MAX_LEVELS];
 watch_t timer;
} game_t;

extern game_t session;

refer_t getboundclient(int binde);
int getgameclient(refer_t client);
void kickgameclient(refer_t client);
int putgameclient(const char* name, int binde);
void spawngameclient(refer_t client, int level);

void handlehost();
void updatehost();