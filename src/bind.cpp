#include "bind.h"
#include "main.h"

#include <stdarg.h>

// ==================================================
// lua external globals

screen_t* le_screen = 0;

// ==================================================
// declarations

sol::state L;

// ==================================================
// externals

extern void assigntile(int index, const char* name);
extern refer_t designunit(const char* word, int extra);
extern void formsurface(const char* word, surface_e type);

// ==================================================
// functions

void loadscript(const char* path, const char* string) {
 char buffer[MAX_PATHLENGTH];
 char* c;
 
 c = (char*) strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  L.script_file(buffer);
 }
}

void enablelua() {
 L.open_libraries(sol::lib::base, sol::lib::string);
 
 L.create_named_table("game",
  "CLIENT", GAME_CLIENT,
  "HOST", GAME_HOST,
  "PRIVATE", GAME_PRIVATE,
  "RATE", GAMERATE,
  
  "random", [](int range = 0xffff) {
   return randominteger(range);
  },
  
  "randomboolean", []() {
   return randominteger(0xffff) % 1;
  },
  
  "render", []() {
   extern screen_t* le_screen;
   
   //rendergame(le_screen);
  },
  
  "start", [](int type, const char* name, const char* addr, const char* port) {
   //startsession(type, name, addr, atoi(port));
  },
  
  "seedrandom", [](int seed = -1) {
   if (seed == -1) {
    seedrandomtime();
   }
   else {
    seedrandom(seed);
   }
  },
  
  "ticks", []() {
   return session.ticks;
  }
 );
 
 L.create_named_table("input",
  "BACKSPACE", '\b',
  "ENTER", '\r',
  "ESCAPE", '\033',
  
  "clicked", [](const char* name) {
   return (bool) getaliasclicked(name);
  },
  
  "down", [](const char* name) {
   return (bool) getaliasdown(name);
  }
 );
 
 L.create_named_table("menu",
  "change", [](const char* view) {
   setview(view);
  },
  
  "listen", [](bool state) {
   setlisten(state);
  }
 );

 L.create_named_table("screen",
  "MIRROR_X", MIRROR_X,
  "MIRROR_Y", MIRROR_Y,
  
  "clear", [](int color) {
   if (le_screen) {
    clearscreen(le_screen, color);
   }
  },
  
  "font", [](const char* msg, float x, float y, int col) {
   if (le_screen) {
    renderfont(msg, (int) x, (int) y, col, le_screen);
   }
  },
  
  "frame", [](const char* title, int x0, int y0, int x1, int y1) {
   if (le_screen) {
    renderframe(title, x0, y0, x1, y1, le_screen);
   }
  },
  
  "get", [](int a, int b, int c, int d) {
   return getcolor(a, b, c, d);
  },
  
  "light", [](float x, float y, int r) {
   if (le_screen) {
    renderlight((int) x, (int) y, r, le_screen);
   }
  },
  
  "size", []() {
   if (le_screen) {
    return std::tuple(le_screen->w, le_screen->h);
   }
   else {
    return std::tuple(0, 0);
   }
  },
  
  "sprite", [](float x, float y, int spr, int col, int flg) {
   if (le_screen) {
    rendersprite((int) x, (int) y, spr, col, flg, le_screen);
   }
  }
 );
 
 L.create_named_table("sound",
  "play", [](const char* name) {
   playsound(name);
  }
 );

 /*L.create_named_table("tile",

 );*/

 //uploadtable(lua_item, "item", L_game);
 //uploadtable(lua_level, "level", L_game);
 //uploadtable(lua_tile, "tile", L_game);
 //uploadtable(lua_unit, "unit", L_game);
 
 {
  //uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
  //uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
  //uploadsubtable(lua_unitmob, "mob", "unit", L_game);
  //uploadsubtable(lua_unitmovable, "movable", "unit", L_game);
  //uploadsubtable(lua_unitpliant, "pliant", "unit", L_game);
  //uploadsubtable(lua_unittrace, "trace", "unit", L_game);
 }
 
 return;
}

void loadscripts() {
 //recursepath(getfilepath("res/item"), loadscript, 0);
 
 //assigntile(NOTILE, "void");
 
 //recursepath(getfilepath("res/tile"), loadscript, 0);
 //recursepath(getfilepath("res/unit"), loadscript, 0);
}
