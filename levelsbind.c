#include "level.h"
#include "bind.h"

#define LUALEVELINTERNAL(Name) \
 NEWLUAFUNCTION(level##Name) { \
  lua_pushnumber(L, level->Name); \
  \
  return 1; \
 }

// ==================================================
// externals

extern level_t* level;

// ==================================================
// functions

LUALEVELINTERNAL(dirtcolor);
LUALEVELINTERNAL(grasscolor);
LUALEVELINTERNAL(h);
LUALEVELINTERNAL(sandcolor);
LUALEVELINTERNAL(w);

NEWLUAFUNCTION(closest) {
 // TODO: implement level.closest()
 // accepts unit word string and returns the closest unit of that type; nounit if invalid or not found
 return 1;
}

NEWLUAFUNCTION(drop) {
 // TODO: implement level item drops
 // accepts item word and position
 return 1;
}

NEWLUAFUNCTION(getdata) {
 if (lua_gettop(L) >= 2) {
  lua_pushnumber(L, getdata(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
 }
 else {
  LUAARGUE("level.getdata");
  
  lua_pushnumber(L, NOTILE);
 }
 
 return 1;
}

NEWLUAFUNCTION(gettile) {
 if (lua_gettop(L) >= 2) {
  lua_pushnumber(L, gettile(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
 }
 else {
  LUAARGUE("level.gettile");
  
  lua_pushnumber(L, NOTILE);
 }
 
 return 1;
}

NEWLUAFUNCTION(setdata) {
 if (lua_gettop(L) >= 3) {
  setdata(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
 }
 else {
  LUAARGUE("level.setdata");
 }
 
 return 0;
}

NEWLUAFUNCTION(settile) {
 if (lua_gettop(L) >= 4) {
  settile(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
 }
 else if (lua_gettop(L) >= 3) {
  settile(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), 0);
 }
 else {
  LUAARGUE("level.settile");
 }
 
 return 0;
}

NEWLUAFUNCTION(spawn) {
 refer_t unit;
 
 unit = NOUNIT;
 
 if (lua_gettop(L) >= 4) {
  unit = initiate(luaL_checkstring(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
  
  getunit(unit)->base.owner = luaL_checknumber(L, 4);
 }
 else if (lua_gettop(L) >= 3) {
  unit = initiate(luaL_checkstring(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
 }
 else if (lua_gettop(L) >= 1) {
  unit = spawn(luaL_checkstring(L, 1));
 }
 else {
  LUAARGUE("level.spawn");
 }
 
 lua_pushnumber(L, unit);
 
 return 1;
}

NEWLUAFUNCTION(summon) {
 if (lua_gettop(L) >= 4) {
  lf_spawn(L);
 }
 else {
  LUAARGUE("level.summon");
  
  lua_pushnumber(L, NOUNIT);
 }
 
 return 1;
}

BEGINLUATABLE(level)
 LUAMETHOD(dirtcolor, leveldirtcolor),
 LUAMETHOD(grasscolor, levelgrasscolor),
 LUAMETHOD(height, levelh),
 LUAMETHOD(sandcolor, levelsandcolor),
 LUAMETHOD(width, levelw),
 
 LUAFUNCTION(closest),
 LUAFUNCTION(getdata),
 LUAFUNCTION(gettile),
 
 LUAFUNCTION(setdata),
 LUAFUNCTION(settile),
 LUAFUNCTION(summon), // calls spawn, but forces the argument count to be at least four
 LUAFUNCTION(spawn),
ENDLUATABLE;
