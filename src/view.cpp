#include "main.h"
#include "bind.h"

#define INVALIDSURFACE -1

// ==================================================
// externals

extern screen_t* le_screen;

// ==================================================
// declarations

surface_t surfaces[MAX_SURFACES] = { 0 };
sol::table view;

// ==================================================
// functions

/*int getsurface(refer_t id) {
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

void opensurface(refer_t view, refer_t pliant) {
 if (!view || !pliant) {
  return;
 }
 
 view = getsurface(view);
 
 if (view == INVALIDSURFACE || !surfaces[view].word) {
  LOGREPORT("surface '%x' is invalidword bound.", view);
  return;
 }
 
 L_game.

 if (hassurfacemethod("open", surfaces[view].id)) {
  callmethod("open", surfaces[view].word, L_game, "n", pliant);
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
 
 LOGDEBUG(2, "formed surface '%s' under id [%x].", word, surfaces[i].id);
 
 return;
}*/

int isview(const char* other) {
 if (view.valid()) {
  auto name = view["_vname"];
  
  if (name.valid() && name.get_type() == sol::type::string) {
   return name == other;
  }
 }

 return false;
}

void loadview(const char* path, const char* string) {
 char buffer[MAX_PATHLENGTH];
 char* c;
 
 c = (char*) strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  *c = '\0';

  L[string] = L.create_table();

  L.script_file(buffer);

  L[string]["_vname"] = string;

  // check if the view has define functions

  *c = '.';
 }
}

void loadviews() {
 recursepath(getfilepath("res/view"), loadview, 0);
}

void setview(const char* newview) {
 setlisten(0);
 
 view = L[newview];

 if (view.valid()) {
  auto reset = view["reset"];

  if (reset.valid()) {
   reset();
  }
 }
 else {
  LOGREPORT("unusable view '%s' set.", newview);
 }
}

void tickview() {
 if (view.valid()) {
  auto tick = view["tick"];

  if ISLUATYPE(tick, function) {
   tick();
  }
 }
}

void renderview(screen_t* screen) {
 le_screen = screen;
 
 if (view.valid()) {
  auto render = view["render"];

  if ISLUATYPE(render, function) {
   render();
  }
 }

 le_screen = 0;
}
