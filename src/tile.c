#include "levelaid.h"
#include "bind.h"

#define NEWTILEMETHOD(Name) NEWLUAFUNCTION(tile##Name)
#define LUATILEMETHOD(Name) LUAMETHOD(Name, tile##Name)

#define CHECKTILEWORD(Word, ...) \
 if (Word < 0 || Word >= MAX_TILEWORDS || !tilewords[Word].word) { \
  LOGREPORT("received unbound tile identifier '%i'.", Word); \
  __VA_ARGS__; \
 }

// ==================================================
// declarations

tileword_t tilewords[MAX_TILEWORDS] = { 0 };

// ==================================================
// externals

extern lua_State* L_game;
extern screen_t* le_screen;

// ==================================================
// functions

int hastilemethod(const char* method, refer_t tile) {
 if (!method || tile == NOTILE) {
  LOGDEBUG("received invalid arguments.");
  return 0;
 }
 
 return hasluamethod(method, tilename(tile), L_game);
}

int calltilemethod(const char* method, int x, int y, const char* format, ...) {
 va_list args;
 int tile, value;
 
 if (!method) {
  LOGREPORT("received invalid method.");
  return 0;
 }
 
 tile = gettile(x, y);
 
 if (tile == NOTILE || !hastilemethod(method, tile)) {
  return 0;
 }
 
 va_start(args, format);
 
 value = passmethod(method, tilename(tile), NULL, L_game, format, args, "nn", x, y);
 
 va_end(args);
 
 return value;
}

void assigntile(int index, const char* name) {
 int i;
 
 if (index < 0) {
  LOGDEBUG("received false tile '%s'", name);
  return;
 }
 
 if (tilewords[index].word) {
  LOGREPORT("duplicate tile IDs for index %i; assigned '%s', attempted '%s'.", index, tilewords[index].word, name);
  exit(EXIT_FAILURE);
 }
 
 for (i = 0; i < MAX_TILEWORDS; i++) {
  if (tilewords[i].word && !strcmp(tilewords[i].word, name)) {
   LOGREPORT("duplicate tile words for tag '%s'.", name);
   exit(EXIT_FAILURE);
  }
 }
 
 tilewords[index].word = (char*) name;
 tilewords[index].flags = 0;
 
 return;
}

int actontile(int x, int y, refer_t unit, refer_t item) {
 //CHECKTILE(x, y);
 
 //calltilemethod("interact", gettile(x, y), x, y, "n", unit);
 
 return 0;
}

int flagtile(refer_t tile, int flags) {
 CHECKTILEWORD(tile, return 0);
 
 tilewords[tile].flags |= flags;
 
 return tilewords[tile].flags;
}

int gettileglow(int x, int y) {
 //return calltilemethod("light", gettile(x, y), x, y, NULL);
 return 0;
}

int hasflags(int flags, refer_t tile) {
 CHECKTILEWORD(tile, return 0);
 
 return tilewords[tile].flags & flags;
}

int surpassable(int x, int y, refer_t unit) {
 if (hastilemethod("blocks", gettile(x, y))) {
  return !calltilemethod("blocks", x, y, "n", unit);
 }
 else {
  return 1;
 }
}

void stepontile(int x, int y, refer_t unit) {
 //CHECKTILE(x, y);
 
 //calltilemethod("step", gettile(x, y), x, y, "n", unit);
}

void striketile(int x, int y, refer_t unit, int damage) {
// calltilemethod("hurt" or "struck");
}

refer_t tileid(const char* string) {
 int i;
 
 if (!string || !(*string)) {
  return NOTILE;
 }
 
 for (i = 0; i < MAX_TILEWORDS; i++) {
  if (tilewords[i].word && !strcmp(tilewords[i].word, string)) {
   return i;
  }
 }
 
 return NOTILE;
}

const char* tilename(refer_t word) {
 CHECKTILEWORD(word, return NULL);
 
 return tilewords[word].word;
}

void touchtile(int x, int y, refer_t unit) {
}


void rendertile(int x, int y, screen_t* screen) {
 if (!screen) {
  LOGREPORT("received unusable screen.");
  return;
 }
 
 if (x < 0 || y < 0) {
  LOGREPORT("received out of bounds tile coordinates.");
  return;
 }
 
 le_screen = screen;
 
 calltilemethod("render", x, y, NULL);
}

void ticktile(int x, int y) {
 if (x < 0 || y < 0) {
  LOGREPORT("received out of bounds tile coordinates.");
  return;
 }
 
 calltilemethod("tick", x, y, NULL);
}

NEWTILEMETHOD(deign) {
 return 1;
}

NEWTILEMETHOD(flagged) {
 int held, i, out, value, word;
 
 held = out = 0;
 
 if (lua_gettop(L) > 0) {
  word = luaL_checknumber(L, 1);
  
  CHECKTILEWORD(word, LOGREPORT("received out of bounds tile word."); goto lf_flagged_exit);
  
  for (i = 2; i <= lua_gettop(L); i++) {
   value = luaL_checknumber(L, i);
   
   held |= value;
   
   out |= tilewords[word].flags & value;
  }
  
  if (held != out) {
   out = 0;
  }
 }
 else {
  LUAARGUE("tile.flagged");
 }
 
lf_flagged_exit:
 lua_pushboolean(L, out);
 
 return 1;
}

NEWTILEMETHOD(id) {
 if (lua_gettop(L) > 0) {
  lua_pushnumber(L, tileid(luaL_checkstring(L, 1)));
 }
 else {
  LUAARGUE("tile.id");
  
  lua_pushnumber(L, NOTILE);
 }
 
 return 1;
}

NEWTILEMETHOD(interact) {
 return 0;
}

NEWTILEMETHOD(luminance) {
 return 0;
}

NEWTILEMETHOD(name) {
 int word;
 
 if (lua_gettop(L) > 0) {
  word = luaL_checknumber(L, 1);
 }
 else {
 // word = vtile.word;
 }
 
 CHECKTILEWORD(word, lua_pushstring(L, "")) else {
  lua_pushstring(L, tilewords[word].word);
 }
 
 return 1;
}

NEWTILEMETHOD(blocks) {
 return 0;
}

NEWTILEMETHOD(setflag) {
 int i, word;
 
 if (lua_gettop(L) > 0) {
  word = luaL_checknumber(L, 1);
  
  CHECKTILEWORD(word, return 0);
  
  for (i = 2; i <= lua_gettop(L); i++) {
   tilewords[word].flags |= (int) luaL_checknumber(L, i);
  }
 }
 else {
  LUAARGUE("tile.setflag");
 }
 
 return 0;
}

NEWTILEMETHOD(step) {
 return 0;
}

NEWTILEMETHOD(strike) {
 return 0;
}

NEWTILEMETHOD(touch) {
 return 0;
}

BEGINLUATABLE(tile)
 LUANUMBER(ALIAS, -1),
 LUANUMBER(TOGRASS, TILE_TOGRASS),
 LUANUMBER(TOLAVA, TILE_TOLAVA),
 LUANUMBER(TOSAND, TILE_TOSAND),
 LUANUMBER(TOWATER, TILE_TOWATER),
 LUANUMBER(TOLIQUID, TILE_TOLIQUID),
 
 LUATILEMETHOD(deign),
 LUATILEMETHOD(flagged),
 LUATILEMETHOD(blocks),
 LUATILEMETHOD(id),
 LUATILEMETHOD(interact),
 LUATILEMETHOD(luminance),
 LUATILEMETHOD(setflag),
 LUATILEMETHOD(step),
 LUATILEMETHOD(strike),
 LUATILEMETHOD(touch),
ENDLUATABLE;
