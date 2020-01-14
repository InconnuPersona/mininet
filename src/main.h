#pragma once

#include "host.h"

#define GAMERATE (NETWORKRATE * 1)
#define GAMEVERSION 0x0000002
#define MAX_PATHLENGTH 128
#define MAX_WORDLENGTH 32
#define TICKRATE 30

typedef enum {
 GAME_NONE = 0,
 GAME_CLIENT,
 GAME_HOST,
 GAME_PRIVATE,
} gametype_e;

extern void clearkeys();
extern void pollevents();
extern void tickkeys();
extern void setlisten(int state);
extern int getaliasclicked(const char* name);
extern int getaliasdown(const char* name);
extern void alias(int key, const char* name);

void initiatechecksum();
int checksum(byte_t* bytes, int length, int checksum);

void loadlibraries();
void closelibraries();
void printhostdata();

void enablegame();
//void endsession() or closesession()
void startsession(gametype_e type, char* name, char* address, int port);
void rendergame(screen_t* screen);
void tickgame();

void bindkeys();
void renderfocusnagger(screen_t* screen, int ticks);

void loadassets();

void loadsounds();
void playsound(const char* sound);
void unloadsounds();

void loadviews();
