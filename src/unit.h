#pragma once

#include "bind.h"

#include <vector>

#define NOUNIT 0

struct unitword_s {
 std::string type;
 std::string name;

 sol::table data;
};

// ==================================================
// declarations

struct Unit {
 int id;
 int base; // index to units[]
 int x, y;
 int halfW, halfH;
 int level;
 
 Unit() {
  id = base = 0;
  x = y = 0;
  halfW = halfH = 0;
 }

 sol::object getClass() {
  return sol::nil;
 }

 void touch(Unit* other) {
  return;
 }
 
 void test() {
  LOGREPORT("meme");
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

std::vector<unitword_s> unitdefs;
std::vector<Unit*> units;

unitword_s* begetunit(const char* word, int* idx) {
 int i;

 for (i = 0; i < unitdefs.size(); i++) {
  if (unitdefs[i].name == word) {
   *idx = i;

   return &unitdefs[i];
  }
 }

 return NULL;
}

void loadunit(const char* path, const char* string) {
char buffer[MAX_PATHLENGTH];
 char* c;
 
 c = (char*) strchr(string, '.');
 
 if (c && !strncmp(c, ".lua", 4)) {
  memset(buffer, 0, MAX_PATHLENGTH);
  
  snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, string);
  
  *c = '\0';
  
  auto data = L.create_table();

  data["name"] = string;

  L[string] = data;
  
  L.script_file(buffer);
  
  // Add table to unitwords
  auto base = data["class"];

  if ISLUATYPE(base, string) {
   unitdefs.push_back({ base, string, data });
  }
  else {
   unitdefs.push_back({ "unit", string, data });
  }

  LOGREPORT("loaded unit '%s' definition.", string);

  *c = '.';
 }
}

void enableunits() {
 L.new_usertype<Unit>("unit",
  "new", sol::no_constructor,
  
  "class", &Unit::getClass,
  
  "id", sol::readonly(&Unit::id),
  "level", sol::readonly(&Unit::level),
  
  "x", &Unit::x,
  "y", &Unit::y,
  
  "halfW", &Unit::halfW,
  "halfH", &Unit::halfH,
  
  "touch", &Unit::touch
 );

 L.new_usertype<Mob>("mob",
  sol::base_classes, sol::bases<Unit>()
 );
 
 L.new_usertype<Movable>("movable",
  sol::base_classes, sol::bases<Unit>()
 );

 L.new_usertype<Pliant>("pliant",
  sol::base_classes, sol::bases<Mob>()
 );
 
 //uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
 //uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
 //uploadsubtable(lua_unittrace, "trace", "unit", L_game);

 return;
}
