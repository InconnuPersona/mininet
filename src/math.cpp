#include "main.h"

#include <math.h>
#include <time.h>

#define MAX_CRCVALUES 256

int crctable[MAX_CRCVALUES] = { 0 };

// ==================================================
// bounding box functions

void boundbox(aabb_t* aabb, float x0, float y0, float x1, float y1) {
 if (!aabb) {
  return;
 }
 
 aabb->x0 = x0;
 aabb->y0 = y0;
 aabb->x1 = x1;
 aabb->y1 = y1;
}

void binddomain(aabb_t* aabb, float x0, float y0, float x1, float y1) {
 if (!aabb) {
  return;
 }
 
 BOUNDVALUE(aabb->x0, x0, x1);
 BOUNDVALUE(aabb->x1, x0, x1);
 BOUNDVALUE(aabb->y0, y0, y1);
 BOUNDVALUE(aabb->y1, y0, y1);
 
 return;
}

int onbounds(int x, int y, aabb_t* aabb) {
 return (x == aabb->x0) || (x == aabb->x1) || (y == aabb->y0) || (y == aabb->y1);
}

void ensuredomain(aabb_t* aabb) {
 int medium;
 
 if (!aabb) {
  return;
 }
 
 if (aabb->x1 < aabb->x0) {
  medium = aabb->x1;
  aabb->x1 = aabb->x0;
  aabb->x0 = medium;
 }
 
 if (aabb->y1 < aabb->y0) {
  medium = aabb->y1;
  aabb->y1 = aabb->y0;
  aabb->y0 = medium;
 }
}

// ==================================================
// random number functions

void seedrandom(double seed) {
 srand(seed);
}

void seedrandomtime() {
 seedrandom(time(NULL));
}

int randomid() {
 return rand() + 1;
}

int randominteger(int range) {
 //return rand() % (range + 1);
 return rand() % range;
}

float randomfloat() {
 return (float) rand() / RAND_MAX;
}

double randomdouble() {
 return (double) rand() / RAND_MAX;
}

double gaussian() {
 static int havenext = 0;
 static double next;
 
 double s, scalar, v1, v2;
 
 if (havenext) {
  havenext = 0;
  return next;
 }
 else {
  do {
   v1 = 2 * randomdouble() - 1;
   v2 = 2 * randomdouble() - 1;
   
   s = v1 * v1 + v2 * v2;
  }
  while (s >= 1 || s == 0);
  
  scalar = sqrt(-2 * log(s));
  
  next = v2 * scalar;
  havenext = 1;
  
  return v1 * scalar;
 }
 
 return 0;
}

// ==================================================
// checksum functions

void initiatechecksum() {
 int i, j, r;
 
 for (i = 0; i < MAX_CRCVALUES; i++) {
  r = i;
  
  for (j = 0; j < 8; j++) {
   if (r & 1) {
	r >>= 1;
	r ^= 0xedb88320;
   }
   else {
	r >>= 1;
   }
  }
  
  crctable[i] = r;
 }
}

int checksum(byte_t* bytes, int length, int checksum) {
 int i;
 
 checksum = ~checksum;
 
 for (i = 0; i < length; i++) {
  checksum = (checksum >> 8) ^ crctable[(checksum & 0xff) ^ bytes[i]];
 }
 
 return ~checksum;
}

// ==================================================
// mathematical functions

float inversesquareroot(float value) {
 float x, y;
 int i;
 
 x = value * 0.5f;
 y = value;
 
 i = *((int*) &value);
 i = 0x5f3759df - (i >> 1);
 
 y = *((float*) &i);
 
 // reiterate this statement to produce higher precision results;
 // four iterations will yield adequate precision for floating points.
 y *= 1.5f - x * y * y;
 
 return y;
}

float zigzag(float value, float interval) {
 float a, b, c, fvalue, mvalue;
 
 value *= 1.f / (interval * 2.f);
 
 fvalue = floor(value);
 mvalue = fmod(fvalue, 2);
 
 a = -1 + 2 * mvalue;
 b = -mvalue;
 c = (value - fvalue) * a + b + 0.5f;
 
 return c;
}

float cutoff(float value, float interval) {
 float a, b, c, fvalue;
 
 value *= (2.f / interval);
 
 fvalue = floor(value);
 
 a = fmod(fvalue, 2);
 b = 1 + -a;
 c = (value - fvalue) * b + a;
 
 return c;
}

// ==================================================
// timer functions

void settimer(watch_t* watch, timer_e type, float start, float value) {
 watch->type = type;
 
 if (start != CURRENTTIME) {
  watch->start = watch->end = start;
 }
 else {
  watch->start = watch->end = currenttime();
 }
 
 watch->value = value;
 watch->set = 1;
 
 return;
}

float readtimer(watch_t* watch) {
 float other, value;
 
 if (!watch->set) {
  LOGREPORT("received unset timer to read.");
  return 0;
 }
 
 switch (watch->type) {
 case TIMER_FALSELAPSE:
  value = currenttime();
  
  other = value - (watch->end + watch->value);
  
  if (other >= 0) {
   watch->end = value;
  }
  
  return other;
  
 case TIMER_SIMPLELAPSE:
  value = currenttime();
  
  other = (value - watch->end) / watch->value;
  
  if (other >= 0) {
   watch->end = value;
  }
  
  return other;
  
 case TIMER_SPACEDLAPSE:
  value = currenttime();
  
  other = floor((value - watch->end) / watch->value);
  
  if (other > 0) {
   watch->end += other * watch->value;
  }
  
  return other;
  
 default:
  break;
 }
 
 return 0;
}
