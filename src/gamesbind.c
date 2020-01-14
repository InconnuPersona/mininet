#include "main.h"
#include "bind.h"

// ==================================================
// declarations

lua_State* L_game;

// ==================================================
// externals

extern void assigntile(int index, const char* name);
extern refer_t designunit(const char* word, int extra);
extern void formsurface(const char* word, surface_e type);
//extern void appenditem();

// ==================================================
// lua_game

NEWLUAFUNCTION(dumpstack) {
 dumpluastack(L);
 
 return 0;
}

NEWLUAFUNCTION(client) {
 
 return 1;
}

NEWLUAFUNCTION(player) {
 
 return 1;
}

NEWLUAFUNCTION(random) {
 int range;
 
 range = 0xffff;
 
 if (lua_gettop(L) > 0) {
  range = luaL_checknumber(L, 1);
 }
 
 lua_pushnumber(L, randominteger(range));
 
 return 1;
}

NEWLUAFUNCTION(randomboolean) {
 lua_pushboolean(L, randominteger(0xffff) % 1);
 
 return 1;
}

NEWLUAFUNCTION(render) {
 extern screen_t* le_screen;
 
 rendergame(le_screen);
 
 return 0;
}

NEWLUAFUNCTION(seedrandom) {
 if (lua_gettop(L) > 0) {
  if (lua_isnil(L, 1)) {
   seedrandomtime();
  }
  else {
   seedrandom(luaL_checknumber(L, 1));
  }
 }
 
 return 0;
}

NEWLUAFUNCTION(start) {
 char* name;
 char* addr;
 char* port;
 int type;
 
 type = luaL_checknumber(L, 1);
 name = (char*) luaL_checkstring(L, 2);
 addr = (char*) luaL_checkstring(L, 3);
 port = (char*) luaL_checkstring(L, 4);
 
 startsession(type, name, addr, atoi(port));
 
 return 0;
}

NEWLUAFUNCTION(ticks) {
 lua_pushnumber(L, session.ticks);
 
 return 1;
}

// ==================================================
// lua table declaration

BEGINLUATABLE(game)
 LUANUMBER(CLIENT, GAME_CLIENT),
 LUANUMBER(HOST, GAME_HOST),
 LUANUMBER(PRIVATE, GAME_PRIVATE),
 LUANUMBER(RATE, GAMERATE),
 
 LUAFUNCTION(dumpstack),
 LUAFUNCTION(random),
 LUAFUNCTION(randomboolean),
 LUAFUNCTION(render),
 LUAFUNCTION(seedrandom),
 LUAFUNCTION(start),
 LUAFUNCTION(ticks),
ENDLUATABLE;

// ==================================================
// functions

void loaditem(const char* path, const char* string) {
 char buffer[MAX_PATHLENGTH];
 char* c;
 char* label;
 
 c = strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  *c = '\0';
  
  label = reprintstring(string);
  
  if (strlen(label) > MAX_WORDLENGTH) {
   LOGREPORT("surface word '%s' exceeds %i character limit.", label, MAX_WORDLENGTH);
   exit(EXIT_FAILURE);
  }
  
  *c = '.';
  
  uploadfile(buffer, label, L_game);
  
  {
   if (hasluamethod("append", label, L_game)) {
	callmethod("append", label, L_game, NULL);
   }
   
   if (getinternal("type", label, L_game) != VIEW_NONE) {
	formsurface(label, getinternal("type", label, L_game));
   }
  }
 }
}

void loaditems() {
 recursepath(getfilepath("res/item"), loaditem, 0);
 
 return;
}

void loadtile(const char* path, const char* file) {
 char buffer[MAX_PATHLENGTH];
 char* c;
 char* label;
 
 c = strchr(file, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, file);
  
  *c = '\0';
  
  label = reprintstring(file);
  
  if (strlen(label) > MAX_WORDLENGTH) {
   LOGREPORT("tile word '%s' exceeds %i character limit.", label, MAX_WORDLENGTH);
   exit(EXIT_FAILURE);
  }
  
  *c = '.';
  
  uploadfile(buffer, label, L_game);
  
  {
   assigntile(getinternal("id", label, L_game), label);

   if (hasluamethod("define", label, L_game)) {
    callmethod("define", label, L_game, NULL);
   }
  }
 }
}

void loadtiles() {
 assigntile(NOTILE, "void");
 
 recursepath(getfilepath("res/tile"), loadtile, 0);
 
 return;
}

void loadunit(const char* path, const char* string) {
 char buffer[MAX_PATHLENGTH];
 char* c;
 char* label;
 
 c = strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  *c = '\0';
  
  label = reprintstring(string);
  
  if (strlen(label) > MAX_WORDLENGTH) {
   LOGREPORT("unit word '%s' exceeds %i character limit.", label, MAX_WORDLENGTH);
   exit(EXIT_FAILURE);
  }
  
  *c = '.';
  
  uploadfile(buffer, label, L_game);
  
  {
   designunit((char*) getinternal("word", label, L_game), getinternal("extra", label, L_game));
   
   if (hasluamethod("define", label, L_game)) {
    callmethod("define", label, L_game, NULL);
   }
  }
 }
}

void loadunits() {
 recursepath(getfilepath("res/unit"), loadunit, 0);
 
 return;
}

void enablegame() {
 extern luatable_t lua_unitdrop, lua_unitfiend, lua_unitmob, lua_unitmovable, lua_unitpliant, lua_unittrace;
 
 L_game = luaL_newstate();
 
 luaL_openlibs(L_game);
 
 uploadtable(lua_game, "game", L_game);
 uploadtable(lua_item, "item", L_game);
 uploadtable(lua_level, "level", L_game);
 uploadtable(lua_menu, "menu", L_game);
 uploadtable(lua_screen, "screen", L_game);
 uploadtable(lua_tile, "tile", L_game);
 uploadtable(lua_unit, "unit", L_game);
 
 {
  uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
  uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
  uploadsubtable(lua_unitmob, "mob", "unit", L_game);
  uploadsubtable(lua_unitmovable, "movable", "unit", L_game);
  uploadsubtable(lua_unitpliant, "pliant", "unit", L_game);
  uploadsubtable(lua_unittrace, "trace", "unit", L_game);
 }
 
 loaditems();
 loadtiles();
 loadunits();
 
 return;
}
