#include "main.h"
#include "bind.h"

// ==================================================
// externals

extern screen_t* le_screen;

// ==================================================
// declarations

lua_State* L_menu;
char* menu;

// ==================================================
// functions

void loadmenu(const char* path, const char* string) {
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
   LOGREPORT("menu label '%s' exceeds %i character limit.", label, MAX_WORDLENGTH);
   exit(EXIT_FAILURE);
  }
  
  *c = '.';
  
  uploadfile(buffer, label, L_menu);
 }
}

void enablemenu() {
 L_menu = luaL_newstate();
 
 luaL_openlibs(L_menu);
 
 uploadtable(lua_game, "game", L_menu);
 uploadtable(lua_input, "input", L_menu);
 uploadtable(lua_menu, "menu", L_menu);
 uploadtable(lua_screen, "screen", L_menu);
 uploadtable(lua_sound, "sound", L_menu);
 
 recursepath(getfilepath("res/menu"), loadmenu, 0);
 
 return;
}

void disablemenu() {
 lua_close(L_menu);
}

int ismenu(const char* other) {
 return !strcmp(menu, other);
}

void setmenu(const char* newmenu) {
 setlisten(0);
 
 menu = (char*) newmenu;
 
 if (menu) {
  callmethod("reset", menu, L_menu, NULL);
 }
}

void menuchar(int character) {
 if (menu && ((character >= ' ' && character <= '~') || character == '\b' || character == '\r' || character == '\033')) {
  callmethod("text", menu, L_menu, "n", character);
 }
}

void tickmenu() {
 if (menu && hasluamethod("tick", menu, L_menu)) {
  callmethod("tick", menu, L_menu, NULL);
 }
}

void rendermenu(screen_t* screen) {
 le_screen = screen;
 
 if (menu) {
  callmethod("render", menu, L_menu, NULL);
 }
 
 le_screen = 0;
}

// ==================================================
// lua_menu

NEWLUAFUNCTION(change) {
 char* menu;
 
 menu = (char*) luaL_checkstring(L, 1);
 
 setmenu(menu);
 
 return 0;
}

NEWLUAFUNCTION(listen) {
 int state;
 
 if (lua_isboolean(L, -1)) {
  state = lua_toboolean(L, -1);
 }
 else {
  state = luaL_checknumber(L, 1);
 }
 
 setlisten(state);
 
 return 0;
}

// ==================================================
// lua table declaration

BEGINLUATABLE(menu)
 LUAFUNCTION(change),
 LUAFUNCTION(listen),
ENDLUATABLE;
