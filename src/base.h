#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTEMASK 0xff
#define BYTEWIDTH 8
#define CURRENTTIME -2
#define FLOATWIDTH (sizeof(float) * BYTEWIDTH)
#define INTEGERMASK 0xffffffff
#define INTEGERWIDTH (sizeof(int) * BYTEWIDTH)
#define SHORTMASK 0xffff
#define SHORTWIDTH (sizeof(short) * BYTEWIDTH)

#define LOGREPORT(...) \
 { printf("%s: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); }

#define LOGDEBUG(...) \
 if (debug) { \
  LOGREPORT(__VA_ARGS__); \
 }

#define MILLISECONDS(Time) (int) ((Time) * 1000)

#define NULLPRONE(Element, Message) \
 if (!Element) { \
  LOGREPORT("%s.", Message); \
  exit(EXIT_FAILURE); \
 }

#define SDLNULLPRONE(Element, Message) \
 if (!Element) { \
  LOGREPORT("%s - %s.", Message, SDL_GetError()); \
  exit(EXIT_FAILURE); \
 }

#define SDLNEGATIVEPRONE(Element, Message) \
 if (Element < 0) { \
  LOGREPORT("%s - %s.", Message, SDL_GetError()); \
  exit(EXIT_FAILURE); \
 }

#define TOSTRING(String) #String

typedef enum {
 TIMER_STEPPED,
 TIMER_FALSELAPSE,
 TIMER_SIMPLELAPSE,
 TIMER_PATTERN,
} timer_e;

typedef struct {
 int x0, y0, x1, y1;
} aabb_t;

typedef struct {
 timer_e type;
 int set; // bool
 float start, end;
 float value;
} watch_t;

typedef unsigned char byte_t;
typedef unsigned int uint_t;

typedef int color_t;
typedef int refer_t;

extern int debug;

float inversesquareroot(float value);
float cutoff(float value, float interval);
float zigzag(float value, float interval);

float currenttime();
void delaythread(float seconds);
float readtimer(watch_t* watch);
void settimer(watch_t* watch, timer_e type, float start, float value);

void seedrandom(int seed);
void seedrandomtime();

int randomid();
int randominteger(int range);
float randomfloat();
double randomdouble();
double gaussian();

void boundbox(aabb_t* aabb, float x0, float y0, float x1, float y1);
char* reprintstring(const char* string);
