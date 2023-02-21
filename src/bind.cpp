#include "bind.h"
#include "main.h"

#include <stdarg.h>

// ==================================================
// lua external globals

screen_t* le_screen = 0;

// ==================================================
// lua_input

/*NEWLUAFUNCTION(clicked) {
 char* name;
 
 name = (char*) luaL_checkstring(L, 1);
 
 lua_pushboolean(L, getaliasclicked(name));
 
 return 1;
}

NEWLUAFUNCTION(down) {
 char* name;
 
 name = (char*) luaL_checkstring(L, 1);
 
 lua_pushboolean(L, getaliasdown(name));
 
 return 1;
}

// ==================================================
// lua_screen

NEWLUAFUNCTION(clear) {
 int color;
 
 color = luaL_checknumber(L, 1);
 
 if (le_screen) {
  clearscreen(le_screen, color);
 }
 
 return 0;
}

NEWLUAFUNCTION(font) {
 char* msg;
 int x, y, col;
 
 msg = (char*) luaL_checkstring(L, 1);
 x = luaL_checknumber(L, 2);
 y = luaL_checknumber(L, 3);
 col = luaL_checknumber(L, 4);
 
 if (le_screen) {
  renderfont(msg, x, y, col, le_screen);
 }
 
 return 0;
}

NEWLUAFUNCTION(frame) {
 char* title;
 int x0, y0, x1, y1;
 
 title = (char*) luaL_checkstring(L, 1);
 x0 = luaL_checknumber(L, 2);
 y0 = luaL_checknumber(L, 3);
 x1 = luaL_checknumber(L, 4);
 y1 = luaL_checknumber(L, 5);
 
 if (le_screen) {
  renderframe(title, x0, y0, x1, y1, le_screen);
 }
 
 return 0;
}

NEWLUAFUNCTION(get) {
 int a, b, c, d;
 
 a = luaL_checknumber(L, 1);
 b = luaL_checknumber(L, 2);
 c = luaL_checknumber(L, 3);
 d = luaL_checknumber(L, 4);
 
 lua_pushnumber(L, getcolor(a, b, c, d));
 
 return 1;
}

NEWLUAFUNCTION(height) {
 if (le_screen) {
  lua_pushnumber(L, le_screen->h);
 }
 else {
  lua_pushnumber(L, 0);
 }
 
 return 1;
}

NEWLUAFUNCTION(light) {
 int x, y, r;
 
 x = luaL_checknumber(L, 1);
 y = luaL_checknumber(L, 2);
 r = luaL_checknumber(L, 3);
 
 if (le_screen) {
  renderlight(x, y, r, le_screen);
 }
 
 return 0;
}

NEWLUAFUNCTION(sprite) {
 int x, y, spr, col, flg;
 
 x = luaL_checknumber(L, 1);
 y = luaL_checknumber(L, 2);
 spr = luaL_checknumber(L, 3);
 col = luaL_checknumber(L, 4);
 flg = luaL_checknumber(L, 5);
 
 if (le_screen) {
  rendersprite(x, y, spr, col, flg, le_screen);
 }
 
 return 0;
}

NEWLUAFUNCTION(width) {
 if (le_screen) {
  lua_pushnumber(L, le_screen->w);
 }
 else {
  lua_pushnumber(L, 0);
 }
 
 return 1;
}

// ==================================================
// lua_sound

NEWLUAFUNCTION(play) {
 char* sound;
 
 sound = (char*) luaL_checkstring(L, 1);
 
 if (sound) {
  playsound(sound);
 }
 
 return 0;
}

// ==================================================
// lua table declarations

BEGINLUATABLE(input)
 LUANUMBER(BACKSPACE, '\b'),
 LUANUMBER(ENTER, '\r'),
 LUANUMBER(ESCAPE, '\033'),
 
 LUAFUNCTION(clicked),
 LUAFUNCTION(down),
ENDLUATABLE;

BEGINLUATABLE(screen)
 LUANUMBER(MIRROR_X, MIRROR_X),
 LUANUMBER(MIRROR_Y, MIRROR_Y),
 
 LUAFUNCTION(clear),
 LUAFUNCTION(font),
 LUAFUNCTION(frame),
 LUAFUNCTION(get),
 LUAFUNCTION(height),
 LUAFUNCTION(light),
 LUAFUNCTION(sprite),
 LUAFUNCTION(width),
ENDLUATABLE;

BEGINLUATABLE(sound)
 LUAFUNCTION(play),
ENDLUATABLE;
*/

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
 //extern luatable_t lua_unitdrop, lua_unitfiend, lua_unitmob, lua_unitmovable, lua_unitpliant, lua_unittrace;
 
 L.open_libraries(sol::lib::base);
 
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
 
 L.create_named_table("menu",
  "change", 1,
  "listen", 1
 );
 
 L.create_named_table("menu",
  "change", [](std::string view) {
   setview(view.c_str());
  },
  
  "listen", [](bool state) {
   setlisten(state);
  }
 );

 /*L.create_named_table("tile",

 );*/

 //uploadtable(lua_input, "input", L_menu);
 //uploadtable(lua_item, "item", L_game);
 //uploadtable(lua_level, "level", L_game);
 //uploadtable(lua_menu, "menu", L_game);
 //uploadtable(lua_screen, "screen", L_game);
 //uploadtable(lua_sound, "sound", L_menu);
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
