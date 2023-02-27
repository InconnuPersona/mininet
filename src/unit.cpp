#include "main.h"
#include "bind.h"

#include <vector>

struct unitword_s {
 char* name;
 
 sol::table data;
 
 
};

// ==================================================
// declarations

struct Unit {
 //int id;
 //char* name;
 
 //int halfW, halfH;
 //int x, y;
 
 Unit() {
  id = 0;
  name = (char*) "unit";
  halfW = halfH = 0;
  x = y = 0;
 }
 
 void touch(Unit* other) {
  return;
 }
 
};

struct Mob : Unit {
 int hp, maxHP;
 
 Mob() {
  hp = maxHP = 0;
 }
};

struct Movable : Unit {
 
};

struct Pliant : Mob {
 int stamina, maxStamina;
 
 Pliant() {
  stamina = maxStamina = 0;
 }
};

std::vector<unitword_s> words;



void loadunit(const char* path, const char* string) {
char buffer[MAX_PATHLENGTH];
 char* c;
 
 c = (char*) strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  *c = '\0';
  
  L[string] = L.create_table();
  L[string]["name"] = string;
  
  L.script_file(buffer);
  
  
  
  *c = '.';
 }
}

void enableunits() {
 
 L.new_usertype<Unit>("unit",
  sol::constructors<Unit()>(),
  
  //sol::meta_function::new_index,
  
  "x", &Unit::x,
  "y", &Unit::y,
  
  "halfW", &Unit::halfW,
  "halfH", &Unit::halfH,
  
  "touch", &Unit::touch
  
 );
 
 /*L.new_usertype<Mob>("unit",
 );
 L.new_usertype<Movable>("unit",
 );
 L.new_usertype<Pliant>("pliant",
 );*/
 
 
 
 //uploadtable(lua_unit, "unit", L_game);
 
  //uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
  //uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
  //uploadsubtable(lua_unitmob, "mob", "unit", L_game);
  //uploadsubtable(lua_unitmovable, "movable", "unit", L_game);
  //uploadsubtable(lua_unitpliant, "pliant", "unit", L_game);
  //uploadsubtable(lua_unittrace, "trace", "unit", L_game);

 return;
}
