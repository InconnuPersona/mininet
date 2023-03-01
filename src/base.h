#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <string>

#define BYTEMASK 0xff
#define BYTEWIDTH 8
#define CURRENTTIME -2
#define FLOATWIDTH (sizeof(float) * BYTEWIDTH)
#define INTEGERMASK 0xffffffff
#define INTEGERWIDTH (sizeof(int) * BYTEWIDTH)
#define SHORTMASK 0xffff
#define SHORTWIDTH (sizeof(short) * BYTEWIDTH)

#define NOFILE 0

#define BOUNDVALUE(Value, Minimum, Maximum) \
 if (Value < Minimum) { \
  Value = Minimum; \
 } \
 if (Value > Maximum) { \
  Value = Maximum; \
 }

#define INDEBUG(Level, ...) \
 if (debug >= Level) { \
  __VA_ARGS__; \
 };

#define SIGNED(Type) ((Type) -1 < (Type) 0)

#define SIGNEDEXTENT(Type) (Type) (((unsigned Type) ~(unsigned Type) 0) >> 1)

#define UNSIGNEDEXTENT(Type) (Type) (~(Type) 0)

#define INTEGEREXTENT(Type) (unsigned int) (SIGNED(Type) ? SIGNEDEXTENT(Type) : UNSIGNEDEXTENT(Type))

#ifndef __ANDROID__
 #define LOGREPORT(...) \
  { printf("%s: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); }
#else
 #define LOGREPORT(...) \
  { \
   logreport(__FUNCTION__, __VA_ARGS__); \
  }
#endif

#define LOGDEBUG(Level, ...) INDEBUG(Level, LOGREPORT(__VA_ARGS__))

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

typedef void (*recurse_f)(const char* path, const char* file);

typedef unsigned char byte_t;
typedef unsigned int uint_t;
typedef long long long_t;

typedef int color_t;
typedef int refer_t;

extern int debug;

void logreport(const char* function, const char* format, ...);

float inversesquareroot(float value);
float cutoff(float value, float interval);
float zigzag(float value, float interval);

void copyfile(const char* source, const char* target);
int fileangth(refer_t file);
int fileextant(const char* path);
const char* filetext(refer_t file);
void freefile(refer_t file);
const char* getfilepath(const char* path);
int makepath(const char* path);
refer_t readfile(const char* path);
void recursepath(const char* path, recurse_f function, int level);
void writefile(const char* path, const byte_t* bytes, long length);

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
