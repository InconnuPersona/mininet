#pragma once

//#include "item.h"
#include "level.h"
#include "view.h"
#include "net.h"

#define GAMERATE (NETWORKRATE * 2)
#define GAMEVERSION 0x0000003
#define MAX_PATHLENGTH 128
#define MAX_WORDLENGTH 32
#define TICKRATE 30

extern void clearkeys();
extern void pollevents();
extern void tickkeys();
extern void setlisten(int state);
extern int getaliasclicked(const char* name);
extern int getaliasdown(const char* name);
extern void alias(int key, const char* name);

void loadlibraries();
void closelibraries();
void printhostdata();

void enablelua();
void enableunits();

void loadassets();
void loadscripts();

void bindkeys();
void renderfocusnagger(screen_t* screen, int ticks);

void loadsounds();
void playsound(const char* sound);
void unloadsounds();
