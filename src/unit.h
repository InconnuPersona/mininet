#pragma once

#include "bind.h"

#include <map>
#include <vector>

#define NOUNIT 0

// ==================================================
// declarations

typedef Unit* (*unitfactory_f)();

std::map<std::string, unitfactory_f> factories;
std::map<Unit*, int> units;


struct unitword_s {
 std::string type;
 std::string name;

 sol::table data;
 unitfactory_f create;
};

Unit::Unit() {
 id = base = 0;
 x = y = 0;
 halfW = halfH = 0;
}

sol::object Unit::getClass() {
 return sol::nil;
}

void Unit::touch(Unit* other) {
 return;
}
 
void Unit::test() {
 LOGREPORT("meme");
}

int Unit::depth() {
 for (auto& unit : units) {
  if (unit.first->id == id) {
   return unit.second;
  }
 }

 return 0;
}

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

refer_t createunitid() {
 int id;
 
 do {
  id = randomid();
 }
 while (level.hasUnit(id));
 
 return id;
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
   
   
   for (auto factory : factories) {
    if (factory.first == string) {
     create = factory.second;
     break;
    }
   }

   std::string type;

   if (!create) {
    create = []() -> Unit* {
     return new Unit();
    };

    type = "unit";
   }
   else {
    type = base;
   }

   unitdefs.push_back({ base, string, data, create });
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
