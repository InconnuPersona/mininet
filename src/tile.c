#include "level.h"
#include "bind.h"

#define CHECKTILEWORD(Word, ...) \
 if (Word < 0 || Word >= MAX_TILEWORDS) { \
  LOGREPORT("received out of range tile identifier."); \
  __VA_ARGS__; \
 }

#define TILEFLAG(Index, Flag) (tiles[Index].flags & Flag)

// ==================================================
// declarations

int tickcount;
tileword_t tiles[MAX_TILEWORDS] = { 0 };

// ==================================================
// functions

int adjoinsliquid(refer_t tile) {
 CHECKTILEWORD(tile, return 0);
 
 return TILEFLAG(tile, TILEFLAG_TOLAVA) | TILEFLAG(tile, TILEFLAG_TOWATER);
}

void assigntile(int index, const char* name, int flags) {
 int i;
 
 if (tiles[index].name) {
  LOGREPORT("duplicate tile IDs for index %i; assigned [%s], attempted [%s].", index, tiles[index].name, name);
  exit(EXIT_FAILURE);
 }
 
 for (i = 0; i < MAX_TILEWORDS; i++) {
  if (tiles[i].name && !strcmp(tiles[i].name, name)) {
   LOGREPORT("duplicate tile SIDs for tag '%s'.", name);
   exit(EXIT_FAILURE);
  }
 }
 
 tiles[index].name = (char*) name;
 tiles[index].flags = flags;
}

int luminance(refer_t tile) {
 CHECKTILEWORD(tile, return 0);
 
 return tiles[tile].luminance;
}

int surpassable(refer_t tile) {
 CHECKTILEWORD(tile, return 0);
 
 return TILEFLAG(tile, TILEFLAG_PASSABLE);
}

refer_t tileid(const char* string) {
 int i;
 
 if (!string || !(*string)) {
  return NOTILE;
 }
 
 for (i = 0; i < MAX_TILEWORDS; i++) {
  if (tiles[i].name && !strcmp(tiles[i].name, string)) {
   return i;
  }
 }
 
 return NOTILE;
}

const char* tilename(refer_t code) {
 int i;
 
 CHECKTILEWORD(code, return NULL);
 
 return tiles[code].name;
}

// ==================================================
// lua binding

extern lua_State* L_game;
int le_tileX, le_tileY;

void rendertile(int x, int y, level_t* level, screen_t* screen) {
 int tile;
 
 tile = level->tiles[x + y * level->w].id;
 
 if (tile < 1 || tile > MAX_TILEWORDS) {
  return;
 }
 
 le_tileX = x;
 le_tileY = y;
 
 callmethod("render", tilename(level->tiles[x + y * level->w].id), L_game);
}

NEWLUAFUNCTION(name) {
 int id;
 
 id = luaL_checknumber(L, 1);
 
 lua_pushstring(L, tilename(id));
 
 return 1;
}

NEWLUAFUNCTION(word) {
 char* word;
 
 word = (char*) luaL_checkstring(L, 1);
 
 lua_pushnumber(L, tileid(word));
 
 return 1;
}

NEWLUAFUNCTION(x) {
 lua_pushnumber(L, le_tileX);
 
 return 1;
}

NEWLUAFUNCTION(y) {
 lua_pushnumber(L, le_tileY);
 
 return 1;
}

NEWLUAFUNCTION(tograss) {
// lua_pushnumber(L, )
}

NEWLUAFUNCTION(tolava) {
 
}

NEWLUAFUNCTION(tosand) {
 
}

NEWLUAFUNCTION(towater) {
 
}

BEGINLUATABLE(tile)
 //LUAFUNCTION(luminance),
 //LUAFUNCTION(surpassable),
 LUAFUNCTION(name),
 LUAFUNCTION(tograss),
 LUAFUNCTION(tolava),
 LUAFUNCTION(tosand),
 LUAFUNCTION(towater),
 LUAFUNCTION(word),
 LUAFUNCTION(x),
 LUAFUNCTION(y),
ENDLUATABLE;
