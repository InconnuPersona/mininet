#include "bind.h"
#include "main.h"

// ==================================================
// lua external globals

screen_t* le_screen = 0;

// ==================================================
// lua_input

NEWLUAFUNCTION(clicked) {
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

// ==================================================
// functions

void callmethod(const char* method, const char* table, lua_State* L) {
 lua_getglobal(L, table);
 
 if (lua_istable(L, -1)) {
  lua_getfield(L, -1, method);
  
  if (lua_isfunction(L, -1)) {
   lua_call(L, 0, 0);
  }
 }
 
 lua_settop(L, 0);
 
 return;
}

int getinternal(const char* internal, const char* table, lua_State* L) {
 int value = 0;
 
 lua_getglobal(L, table);
 
 if (lua_istable(L, -1)) {
  lua_getfield(L, -1, internal);
  
  if (lua_isnumber(L, -1)) {
   value = lua_tonumber(L, -1);
  }
  else if (lua_isstring(L, -1)) {
   value = (int) lua_tostring(L, -1);
  }
 }
 
 lua_settop(L, 0);
 
 return value;
}

void pushtable(luatable_t table, lua_State* L) {
 int index, length;
 
 for (length = 0; table[length].type != LUAOBJECT_NULL; length++);
 
 lua_createtable(L, 0, length);
 
 for (index = 0; index < length; index++) {
  switch (table[index].type) {
  case LUAOBJECT_NUMBER:
   lua_pushnumber(L, (int) table[index].data);
   break;
   
  case LUAOBJECT_STRING:
   lua_pushstring(L, table[index].data);
   break;
   
  case LUAOBJECT_FUNCTION:
   lua_pushcfunction(L, table[index].data);
   break;
   
  case LUAOBJECT_NULL:
   return;
   
  default:
   break;
  }
  
  lua_setfield(L, -2, table[index].name);
 }
}

void uploadfile(const char* file, const char* label, lua_State* L) {
 luaL_dofile(L, file);
 
 lua_setglobal(L, label);
 
 lua_settop(L, 0);
}

void uploadsubtable(luatable_t table, const char* label, const char* parent, lua_State* L) {
 if (!table || !label || !parent || !L) {
  LOGREPORT("received null arguments.");
  return;
 }
 
 lua_getglobal(L, parent);
 
 lua_pushstring(L, label);
 
 pushtable(table, L);
 
 lua_settable(L, -3);
 
 lua_setglobal(L, parent);
 
 return;
}

void uploadtable(luaobject_t* table, const char* label, lua_State* L) {
 if (!table || !label || !L) {
  LOGREPORT("received null arguments.");
  return;
 }
 
 pushtable(table, L);
 
 lua_setglobal(L, label);
 
 return;
}
