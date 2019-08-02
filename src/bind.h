#pragma once

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#define NEWLUAFUNCTION(Name) int lf_##Name(lua_State* L)

#define LUAFUNCTION(Name) { #Name, lf_##Name, LUAOBJECT_FUNCTION }
#define LUAMETHOD(Name, Function) { #Name, lf_##Function, LUAOBJECT_FUNCTION }
#define LUANULL { NULL, NULL, LUAOBJECT_NULL }
#define LUANUMBER(Name, Value) { #Name, (void*) Value, LUAOBJECT_NUMBER }
#define LUASTRING(Name, String) { #Name, String, LUAOBJECT_STRING }

#define BEGINLUATABLE(Name) luatable_t lua_##Name = {
#define ENDLUATABLE LUANULL }

#define LUAVARIABLE(Name, Variable) \
 NEWLUAFUNCTION(Name) { \
  if (lua_gettop(L) > 0) { \
   Variable = luaL_checknumber(L, 1); \
  } \
  lua_pushnumber(L, Variable); \
  return 1; \
 }

typedef enum {
 LUAOBJECT_NULL = 0,
 LUAOBJECT_NUMBER,
 LUAOBJECT_STRING,
 LUAOBJECT_FUNCTION,
} luaobject_e;

typedef struct {
 char* name;
 void* data;
 luaobject_e type;
} luaobject_t;

typedef luaobject_t luatable_t[];

extern luatable_t lua_game, lua_input, lua_item, lua_level, lua_menu, lua_screen, lua_sound, lua_tile, lua_unit;

void callmethod(const char* method, const char* table, lua_State* L);
int getinternal(const char* internal, const char* table, lua_State* L);
void uploadfile(const char* file, const char* label, lua_State* L);
void uploadsubtable(luatable_t table, const char* label, const char* parent, lua_State* L);
void uploadtable(luatable_t table, const char* label, lua_State* L);
