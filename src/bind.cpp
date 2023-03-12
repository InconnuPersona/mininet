#include "host.h"
#include "bind.h"

#include <stdarg.h>

// ==================================================
// lua external globals

screen_t* le_screen = 0;

// ==================================================
// declarations

sol::state L;

// ==================================================
// externals

extern void loadunit(const char*, const char*);
extern void loadview(const char*, const char*);

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
 L.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
 
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
   session.render(le_screen);
  },
  
  // Perhaps start sessions with a session table that holds its data, such as
  // level size, player name, etc.
  "start", [](int type, const char* name, const char* addr, const char* port) {
   session.start((gametype_e) type, name, addr, atoi(port));
  },
  
  "seedRandom", [](int seed) {
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
  
  "ATTACK", CMD_ATTACK,
  "MENU", CMD_MENU,
  "MOVEDOWN", CMD_MOVEDOWN,
  "MOVELEFT", CMD_MOVELEFT,
  "MOVERIGHT", CMD_MOVERIGHT,
  "MOVEUP", CMD_MOVEUP,
  
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
 
 //uploadtable(lua_item, "item", L_game);
 
 return;
}

void loadscripts() {
 extern bool leveldef;
 
 recursepath(getfilepath("res/tile"), loadscript, 0);

 leveldef = true;
 recursepath(getfilepath("res/lvl"), loadscript, 0);
 leveldef = false;

 recursepath(getfilepath("res/unit"), loadunit, 0);

 recursepath(getfilepath("res/view"), loadview, 0);
}
