#include "bind.h"
#include "main.h"

#include <stdarg.h>

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

// ==================================================
// functions

int callmethod(const char* method, const char* table, lua_State* L, const char* format, ...) {
 va_list args;
 int value;
 
 if (!L || !method || !table) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 va_start(args, format);
 
 value = passmethod(method, table, NULL, L, format, args, NULL);
 
 va_end(args);
 
 return value;
}

void dumpluastack(lua_State* L) {
 int i, type;
 
 if (!L) {
  LOGREPORT("received null argument.");
  return;
 }
 
 LOGREPORT("dumping lua stack;");
 
 for (i = 1; i <= lua_gettop(L); i++) {
  type = lua_type(L, i);
  
  printf(" %i: %s [", i, lua_typename(L, type));
  
  switch (type) {
  case LUA_TBOOLEAN:
   printf(lua_toboolean(L, i) ? "true" : "false");
   break;
   
  case LUA_TNUMBER:
   printf("%g", lua_tonumber(L, i));
   break;
   
  case LUA_TSTRING:
   printf("%s", lua_tostring(L, i));
   break;
   
  default:
   break;
  }
  
  printf("]\n");
 }
}

long getinternal(const char* internal, const char* table, lua_State* L) {
 long value = 0;
 
 lua_getglobal(L, table);
 
 if (lua_istable(L, -1)) {
  lua_getfield(L, -1, internal);
  
  if (lua_isboolean(L, -1)) {
   value = lua_toboolean(L, -1);
  }
  if (lua_isnumber(L, -1)) {
   value = lua_tonumber(L, -1);
  }
  else if (lua_isstring(L, -1)) {
   value = (long) lua_tostring(L, -1);
  }
 }
 
 lua_settop(L, 0);
 
 return value;
}

int getinternaltype(const char* internal, const char* table, lua_State* L) {
 if (!L || !internal || !table) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 lua_getglobal(L, table);
 
 if (lua_istable(L, -1)) {
  lua_getfield(L, -1, internal);
  
  return lua_type(L, -1);
 }
 
 return 0;
}

int hasluamethod(const char* method, const char* table, lua_State* L) {
 if (!L || !method || !table) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 return getinternaltype(method, table, L) == LUA_TFUNCTION;
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

int passmethod(const char* method, const char* table, const char* subtable, lua_State* L, const char* passformat, va_list passed, const char* format, ...) {
 va_list args;
 int count, type, value;
 
 if (!L || !method || !table) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 lua_getglobal(L, table);
 
 if (subtable && lua_istable(L, -1)) {
  lua_getfield(L, -1, subtable);
 }
 
 if (!lua_istable(L, -1)) {
  LOGREPORT("unable to locate lua table '%s.%s'.", table, subtable);
  return 0;
 }
 
 lua_getfield(L, -1, method);
 
 if (!lua_isfunction(L, -1)) {
  LOGREPORT("unable to load function '%s' from table '%s.%s'.", method, table, subtable);
  return 0;
 }
 
 count = 0;
 
 if (format) {
  va_start(args, format);
  
  while (*format) {
   switch (*format) {
   case 'b':
	lua_pushboolean(L, va_arg(args, int));
	break;
	
   case 'f':
   case 'i':
   case 'n':
	lua_pushnumber(L, va_arg(args, int));
	break;
	
   case 's':
	lua_pushstring(L, va_arg(args, char*));
	break;
	
   default:
	LOGREPORT("parameter string includes inoperable character.");
	break;
   }
   
   count++;
   
   format += sizeof(char);
  }
  
  va_end(args);
 }

 if (passformat) {
  while (*passformat) {
   switch (*passformat) {
   case 'b':
	lua_pushboolean(L, va_arg(passed, int));
	break;
	
   case 'f':
   case 'i':
   case 'n':
	lua_pushnumber(L, va_arg(passed, int));
	break;
	
   case 's':
	lua_pushstring(L, va_arg(passed, char*));
	break;
	
   default:
	LOGREPORT("passed parameter string includes inoperable character.");
	break;
   }
   
   count++;
   
   passformat += sizeof(char);
  }
 }
 
 lua_call(L, count, 1);
 
 if (lua_gettop(L) > 0) {
  type = lua_type(L, -1);
  
  switch (type) {
  case LUA_TNUMBER:
   value = lua_tonumber(L, -1);
   break;
   
  case LUA_TBOOLEAN:
   value = lua_toboolean(L, -1);
   break;
   
  case LUA_TSTRING:
   value = (int) reprintstring(lua_tostring(L, -1));
   break;
   
  default:
   value = 0;
   break;
  }
 }
 else {
  value = 0;
 }
 
 lua_settop(L, 0);
 
 return value;
}

int rollmethod(const char* method, const char* table, lua_State* L, const char* format, va_list args) {
 if (!L || !method || !table) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 return passmethod(method, table, NULL, L, format, args, NULL);
}

void uploadfile(const char* path, const char* label, lua_State* L) {
 refer_t file;
 
 file = readfile(path);
 
 if (file == NOFILE) {
  LOGREPORT("unable to upload lua file '%s'.", path);
  return;
 }
 
 luaL_dostring(L, filetext(file));
 
 lua_setglobal(L, label);
 
 lua_settop(L, 0);
 
 freefile(file);
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
