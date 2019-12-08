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

#define BOUNDVALUE(Value, Minimum, Maximum) \
 if (Value < Minimum) { \
  Value = Minimum; \
 } \
 if (Value > Maximum) { \
  Value = Maximum; \
 }

#define INDEBUG(...) \
 if (debug) { \
  __VA_ARGS__; \
 };

#define SIGNED(Type) ((Type) -1 < (Type) 0)

#define SIGNEDEXTENT(Type) (Type) (((unsigned Type) ~(unsigned Type) 0) >> 1)

#define UNSIGNEDEXTENT(Type) (Type) (~(Type) 0)

#define INTEGEREXTENT(Type) (unsigned int) (SIGNED(Type) ? SIGNEDEXTENT(Type) : UNSIGNEDEXTENT(Type))

#define LOGREPORT(...) \
 { printf("%s: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); }

#define LOGDEBUG(...) INDEBUG(LOGREPORT(__VA_ARGS__))

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
 TIMER_SPACEDLAPSE,
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

void seedrandom(double seed);
void seedrandomtime();

int randomid();
int randominteger(int range);
float randomfloat();
double randomdouble();
double gaussian();

void binddomain(aabb_t* aabb, float x0, float y0, float x1, float y1);
void boundbox(aabb_t* aabb, float x0, float y0, float x1, float y1);
void ensuredomain(aabb_t* aabb);
int onbounds(int x, int y, aabb_t* aabb);
char* reprintstring(const char* string);
