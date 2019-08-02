#include "main.h"

#include <time.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

// ==================================================
// random number functions

void seedrandom(int seed) {
 srand(seed);
}

void seedrandomtime() {
 seedrandom(time(NULL));
}

int randomid() {
 return rand() + 1;
}

int randominteger(int range) {
 return rand() % (range + 1);
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
 float value, other;
 
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
  
 default:
  break;
 }
 
 return 0;
}

float currenttime() {
 return SDL_GetTicks() / 1000.f;
}

void delaythread(float seconds) {
 fflush(stdout);
 
 SDL_Delay(seconds * 1000.f);
}

// ==================================================
// functions

void boundbox(aabb_t* aabb, float x0, float y0, float x1, float y1) {
 if (!aabb) {
  return;
 }
 
 aabb->x0 = x0;
 aabb->y0 = y0;
 aabb->x1 = x1;
 aabb->y1 = y1;
}

void loadlibraries() {
 int sdlflags = SDL_INIT_AUDIO | SDL_INIT_VIDEO;
 int sdlimageflags = IMG_INIT_PNG;
 int sdlmixerflags = 0; //MIX_INIT_MP3;
 
 if (SDL_Init(sdlflags) != 0) {
  LOGREPORT("could not load 'SDL2' library - %s.", SDL_GetError());
  exit(EXIT_FAILURE);
 }
 
 if (IMG_Init(sdlimageflags) != sdlimageflags) {
  LOGREPORT("could not load 'SDL2' image library - %s.", IMG_GetError());
  exit(EXIT_FAILURE);
 }
 
 if (Mix_Init(sdlmixerflags) != sdlmixerflags) {
  LOGREPORT("could not load 'SDL2' mixer library - %s.", Mix_GetError());
  // exit(EXIT_FAILURE);
 }
 
 if (SDLNet_Init() == -1) {
  LOGREPORT("could not load 'SDL2' network library - %s.", SDLNet_GetError());
  exit(EXIT_FAILURE);
 }
 
 return;
}

void closelibraries() {
 SDLNet_Quit();
 Mix_Quit();
 IMG_Quit();
 SDL_Quit();
}

void bindkeys() {
 alias(SDLK_UP, "up");
 alias(SDLK_w, "up");
 alias(SDLK_DOWN, "down");
 alias(SDLK_s, "down");
 alias(SDLK_LEFT, "left");
 alias(SDLK_a, "left");
 alias(SDLK_RIGHT, "right");
 alias(SDLK_d, "right");
 alias(SDLK_c, "attack");
 alias(SDLK_RETURN, "menu");
 alias(SDLK_x, "menu");
 alias(SDLK_ESCAPE, "menu");
}

char* reprintstring(const char* string) {
 char* copy;
 int length;
 
 if (!string) {
  return 0;
 }
 
 length = strlen(string) + 1;
 
 copy = (char*) malloc(length);
 
 memcpy(copy, string, length);
 
 return copy;
}


void renderfocusnagger(screen_t* screen, int ticks) {
 const char* msg = "Click to focus!";
 int color;
 int x, y, xx, yy, w, h;
 
 xx = (RENDERWIDTH - strlen(msg) * 8) / 2;
 yy = (RENDERHEIGHT - 8) / 2;
 w = strlen(msg);
 h = 1;
 
 color = getcolor(-1, 1, 5, 445);
 
 rendersprite(xx - 1 * 8, yy - 8, 0 + 13 * 32, color, 0, screen);
 rendersprite(xx + w * 8, yy - 8, 0 + 13 * 32, color, 1, screen);
 rendersprite(xx - 1 * 8, yy + 8, 0 + 13 * 32, color, 2, screen);
 rendersprite(xx + w * 8, yy + 8, 0 + 13 * 32, color, 3, screen);
 
 for (x = 0; x < w; x++) {
  rendersprite(xx + x * 8, yy - 8, 1 + 13 * 32, color, 0, screen);
  rendersprite(xx + x * 8, yy + 8, 1 + 13 * 32, color, 2, screen);
 }
 
 for (y = 0; y < h; y++) {
  rendersprite(xx - 1 * 8, yy + y * 8, 2 + 13 * 32, color, 0, screen);
  rendersprite(xx + w * 8, yy + y * 8, 2 + 13 * 32, color, 1, screen);
 }
 
 if ((ticks / (TICKRATE / 3)) % 2 == 0) {
  renderfont(msg, xx, yy, getcolor(5, 333, 333, 333), screen);
 }
 else {
  renderfont(msg, xx, yy, getcolor(5, 555, 555, 555), screen);
 }
}
