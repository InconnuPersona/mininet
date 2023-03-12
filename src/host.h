#pragma once

#include "main.h"

#define GAMESMARKER 0x4d494e49 // mini in hexadecimal
#define LEVELANGTH 128
#define LOCALANGTH 1
#define MAX_CLIENTS 16
#define MAX_GAMESENDLENGTH 256
#define MAX_NAMELENGTH 16
#define LOCALCLIENT 0
#define SPAWNLEVEL 0

typedef enum {
 MSG_NONE = 0,
 
 // host messages
 MSG_ADDCLIENT,
 MSG_REMOVECLIENT,
 
 //MSG_GAMEDATA,
 //MSG_PLAYSOUND,
 //MSG_REQUEST,
 
 // ingame messages
 //MSG_COMMAND,
 //MSG_POSTCHUNK,
 //MSG_POSTITEMWORD,
 //MSG_POSTPILE,
 //MSG_POSTUNIT,
 //MSG_POSTUNITWORD,
} message_e;

/*typedef struct {
 short length;
 short type;
 int sender;
 byte_t data[MAX_GAMESENDLENGTH];
} gamesend_t;*/

enum gametype_e {
 GAME_NONE = 0,
 GAME_CLIENT,
 GAME_HOST,
 GAME_PRIVATE,
};

struct Game {
 
 struct Client {
  int id;
  int netid;
  //int inmenu;
  int entity, level;
  
  char name[MAX_NAMELENGTH];
  
  int old_cmds;
  int deadtime, livetime;
  int finished; // won game
 };

	
 bool open;
 int marker;
 int id, version;
 int ticks, type;
 int self;
 
 Client clients[MAX_CLIENTS];
 watch_t timer;

 int get_client(refer_t id);

 int new_clientid();
 refer_t new_client(const char* name, refer_t netid);
 void spawn_client(refer_t client, int depth);
 void tick_clients();

 int getInput();

 void start(gametype_e type, const char* name, const char* address, int port);
 void render(screen_t* screen);
 void tick();

 void tick_client(refer_t id);
};

extern Game session;

//void endsession() or closesession()
//void startgame(gametype_e type, const char* name, const char* address, int port);
//void rendergame(screen_t* screen);
//void tickgame();

//int awaited();
//void bindgamelevel(int level);
//int ishost();

//void closesession();

//void lostclient();

//refer_t getboundclient(int bind);
//int getgameclient(refer_t client);
//refer_t getgamepliant(refer_t client);
//refer_t getnamedclient(const char* name);
//refer_t getplayerclient(refer_t pliant);
//void kickgameclient(refer_t client);
//int newgameclient(const char* name, int bind);
//void spawngameclient(refer_t client, int level);

//void pushcommands(int commands);
//void pushdeltachunks(refer_t client);
//void pushgamesend(int type, void* data, int length, refer_t client);
//void pushjoin(const char* name);

//void handlehost();
//void updatehost();
