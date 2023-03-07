#pragma once

#include "bind.h"

#include <vector>

#define NOUNIT 0

typedef Unit* (*unitfactory_f)();

#include <map>

std::map<std::string, unitfactory_f> factories;

struct unitword_s {
 std::string type;
 std::string name;

 sol::table data;
 unitfactory_f create;
};

// ==================================================
// declarations

struct Unit {
 int id;
 int base; // index to units[]
 int x, y;
 int halfW, halfH;
 
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
 int dir, xk, yk;
 bool hurted, ticked, swam, walked;
 
 Mob() {
  hp = maxHP = 0;
 }
};

struct Fiend : Mob {
 int level;
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

  unitfactory_f create = NULL;

  if ISLUATYPE(base, string) {
   
      
   unitdefs.push_back({ base, string, data });

   for (auto type : factories) {
    if (type.first == base->type) {
     unit = type.second();
     break;
    }
   }

  }

  if ()
  else {
   unitdefs.push_back({ "unit", string, data });
  }

  LOGREPORT("loaded unit '%s' definition.", string);

  *c = '.';
 }
}

template <typename Class, typename... Args>
void defineunit(std::string name, Args&&... args) {
 L.new_usertype<Class>(name, std::forward<Args>(args)...);

 factories.insert({ name, []() -> Unit* {
  return new Class();
 }});
}

void enableunits() {
 defineunit<Unit>("unit",
  "new", sol::no_constructor,
  
  "class", &Unit::getClass,
  
  "id", sol::readonly(&Unit::id),
  //"level", sol::readonly(&Unit::level),
  
  "x", &Unit::x,
  "y", &Unit::y,
  
  "halfW", &Unit::halfW,
  "halfH", &Unit::halfH,
  
  "touch", &Unit::touch
 );

 defineunit<Mob>("mob",
  sol::base_classes, sol::bases<Unit>()
 );
 
 defineunit<Movable>("movable",
  sol::base_classes, sol::bases<Unit>()
 );

 defineunit<Pliant>("player",
  sol::base_classes, sol::bases<Mob>()
 );
 
 //uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
 //uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
 //uploadsubtable(lua_unittrace, "trace", "unit", L_game);

 return;
}
