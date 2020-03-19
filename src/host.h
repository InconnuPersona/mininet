#pragma once

#include "item.h"
#include "level.h"
#include "menu.h"
#include "net.h"

#define GAMESMARKER 0x4d494e49 // mini in hexadecimal
#define LEVELANGTH 128
#define LOCALANGTH 1
#define MAX_GAMECLIENTS 16
#define MAX_GAMESENDLENGTH 256
#define MAX_LEVELS 5
#define MAX_NAMELENGTH 16
#define LOCALCLIENT 0
#define SPAWNLEVEL 3

//typedef enum {
// CMD_COMMAND,
// CMD_ORDER,
//} command_e;

typedef enum {
 MSG_NONE = 0,
 
 // host messages
 MSG_ADDCLIENT,
 MSG_REMOVECLIENT,
 
 //MSG_GAMEDATA,
 //MSG_PLAYSOUND,
 MSG_REQUEST,
 
 // ingame messages
 MSG_COMMAND,
 MSG_POSTCHUNK,
 //MSG_POSTITEMWORD,
 //MSG_POSTPILE,
 MSG_POSTUNIT,
 MSG_POSTUNITWORD,
} message_e;

typedef struct {
 int bind;
 int id;
 int inmenu;
 int entity, level;
 
 char name[MAX_NAMELENGTH];
 
 int deadtime, livetime;
 int finished; // won game
} gameclient_t;

typedef struct {
 short length;
 short type;
 int sender;
 byte_t data[MAX_GAMESENDLENGTH];
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

int awaited();
void bindgamelevel(int level);
int ishost();

void closesession();

refer_t getboundclient(int bind);
int getgameclient(refer_t client);
refer_t getgamepliant(refer_t client);
refer_t getnamedclient(const char* name);
refer_t getplayerclient(refer_t pliant);
void kickgameclient(refer_t client);
int putgameclient(const char* name, int bind);
void spawngameclient(refer_t client, int level);

void pushcommands(int commands);
void pushdeltachunks(refer_t client);
void pushgamesend(int type, void* data, int length, refer_t client);
void pushjoin(const char* name);

void handlehost();
void updatehost();
