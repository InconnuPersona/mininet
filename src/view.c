#include "main.h"
#include "bind.h"

#define INVALIDSURFACE -1

// ==================================================
// declarations

surface_t surfaces[MAX_SURFACES] = { 0 };

//==================================================
// externals

extern lua_State* L_game;

// ==================================================
// functions

int getsurface(refer_t id) {
 int i;
 
 for (i = 0; i < MAX_SURFACES; i++) {
  if (surfaces[i].id == id) {
   return i;
  }
 }
 
 return INVALIDSURFACE;
}

int newsurfaceid() {
 int id;
 
 do {
  id = randomid();
  
  if (getsurface(id) != INVALIDSURFACE) {
   continue;
  }
  
  return id;
 }
 while (1);
}

int hassurfacemethod(const char* method, refer_t view) {
 int i;
 
 if (!method || !view) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 i = getsurface(view);
 
 if (i == INVALIDSURFACE || !surfaces[i].word) {
  LOGREPORT("surface '%x' is invalidword bound.", view);
  return 0;
 }
 
 return hasluamethod(method, surfaces[i].word, L_game);
}

void rendersurface(refer_t view, refer_t pliant) {
 if (!view || !pliant) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 view = getsurface(view);
 
 if (view == INVALIDSURFACE || !surfaces[view].word) {
  LOGREPORT("surface '%x' is invalidword bound.", view);
  return;
 }
 
 if (hassurfacemethod("render", surfaces[view].id)) {
  callmethod("render", surfaces[view].word, L_game, "n", pliant);
 }
}

void ticksurface(refer_t view, refer_t pliant) {
 if (!view || !pliant) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 view = getsurface(view);
 
 if (view == INVALIDSURFACE || !surfaces[view].word) {
  LOGREPORT("surface '%x' is invalidword bound.", view);
  return;
 }
 
 if (hassurfacemethod("tick", surfaces[view].id)) {
  callmethod("tick", surfaces[view].word, L_game, "n", pliant);
 }
}

refer_t getsurfacebyword(const char* word) {
 int i;
 
 if (!word) {
  return NOSURFACE;
 }
 
 for (i = 0; i < MAX_SURFACES; i++) {
  if (surfaces[i].id && !strcmp(word, surfaces[i].word)) {
   return surfaces[i].id;
  }
 }
 
 return NOSURFACE;
}

void formsurface(const char* word, surface_e type) {
 int i;
 
 if (!word) {
  LOGREPORT("received invalid surface word '%s'.", word);
  return;
 }
 
 if (getsurfacebyword(word) != NOSURFACE) {
  LOGREPORT("surface word '%s' already exists.", word);
  return;
 }
 
 for (i = 0; i < MAX_SURFACES; i++) {
  if (!surfaces[i].id) {
   break;
  }
 }
 
 if (i == MAX_SURFACES) {
  LOGREPORT("unable to find free slot for surface '%s'.", word);
  return;
 }
 
 surfaces[i].id = newsurfaceid();
 surfaces[i].type = type;
 surfaces[i].word = reprintstring(word);
 
 LOGDEBUG("formed surface '%s' under id [%x].", word, surfaces[i].id);
 
 return;
}

