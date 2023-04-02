#pragma once

#include "bind.h"

#include <map>
#include <vector>
#include "level.h"

#define NOUNIT 0

// ==================================================
// declarations

typedef Unit* (*unitfactory_f)();

std::map<std::string, unitfactory_f> factories;
std::map<Unit*, int> units;


template<typename T>
struct func_traits;

template<typename R, typename ... Args>
struct func_traits<std::function<R(Args...)>> {

};

Unit::Unit() {
 id = base = 0;
 x = y = 0;
 halfW = halfH = 0;
 exist = true;
}

void Unit::input(int cmds) {
 return;
}

sol::object Unit::getClass() {
 return sol::nil;
}

bool Unit::moveStep(int xa, int ya) {
 aabb_t newbox, oldbox, t, to;
 refer_t* nowinside;
 refer_t* wasinside;
 int blocked, i, j;
 int newcount, oldcount;
 int xr, xt, yr, yt;
 
 if (xa != 0 && ya != 0) {
  LOGREPORT("units may only move steps along one axis at a time.");
  return 0;
 }
 
 xr = halfW;
 yr = halfH;
 
 boundbox(&oldbox, x - xr, y - yr, x + xr, y + yr);
 boundbox(&newbox, oldbox.x0 + xa, oldbox.y0 + ya, oldbox.x1 + xa, oldbox.y1 + ya);
 
 boundbox(&to, oldbox.x0 >> 4, oldbox.y0 >> 4, oldbox.x1 >> 4, oldbox.y1 >> 4);
 boundbox(&t, newbox.x0 >> 4, newbox.y0 >> 4, newbox.x1 >> 4, newbox.y1 >> 4);
 
 blocked = 0;
 
 for (yt = t.y0; yt <= t.y1; yt++) {
  for (xt = t.x0; xt <= t.x1; xt++) {
   if (xt >= to.x0 && xt <= to.x1 && yt >= to.y0 && yt <= to.y1) {
	continue;
   }
   
   /*touchtile(xt, yt, unit);
   
   if (!surpassable(xt, yt, unit)) {
	blocked = 1;
	return 0;
   }*/
  }
 }
 
 if (blocked) {
  return 0;
 }
 
 /*oldcount = copysamples(getunits(oldbox), &wasinside);
 newcount = copysamples(getunits(newbox), &nowinside);
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == unit) {
   continue;
  }
  
  touchunit(nowinside[i], unit);
  
  for (j = 0; j < oldcount; j++) {
   if (nowinside[i] == wasinside[j]) {
	nowinside[i] = NOUNIT;
	break;
   }
  }
 }
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == NOUNIT) {
   for (j = i; j < newcount; j++) {
	if (nowinside[j] != NOUNIT) {
	 nowinside[i] = nowinside[j];
	 break;
	}
   }
  }
  
  if (nowinside[i] == NOUNIT) {
   newcount = i;
   break;
  }
 }
 
 for (i = 0; i < newcount; i++) {
  if (nowinside[i] == unit) {
   continue;
  }
  
  if (!passable(nowinside[i], unit)) {
   free(nowinside);
   free(wasinside);
   
   return 0;
  }
 }
 
 free(nowinside);
 free(wasinside);*/
 
 x += xa;
 y += ya;
 
 return true;
}

bool Unit::move(int xa, int ya) {
 int xt, yt;
 bool stopped;

 if (xa != 0 || ya != 0) {
  stopped = true;
  
  if (xa != 0 && moveStep(xa, 0)) {
   stopped = false;
  }
  
  if (ya != 0 && moveStep(0, ya)) {
   stopped = false;
  }
  
  if (!stopped) {
   xt = x >> 4;
   yt = y >> 4;
   
   //stepontile(xt, yt, id);
  }
  
  return !stopped;
 }
 
 return 1;
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
 int hurted, ticked, swam, walked;
 
 Mob() {
  hp = maxHP = 0;
  dir = xk = yk = 0;
  hurted = ticked = swam = walked = 0;
 }

 bool move(int xa, int ya) override {
  //if (inliquid(id) && mob->swum++ % 2 == 0) {
  // return true;
  //}
   
  if (xk < 0) {
   moveStep(-1, 0);
   xk++;
  }
  if (xk > 0) {
   moveStep(1, 0);
   xk--;
  }
  
  if (yk < 0) {
   moveStep(0, -1);
   yk++;
  }
  if (yk > 0) {
   moveStep(0, 1);
   yk--;
  }
  
  if (hurted > 0) {
   return true;
  }
  
  if (xa != 0 || ya != 0) {
   walked++;
   
   if (xa < 0) {
    dir = 2;
   }
   else if (xa > 0) {
    dir = 3;
   }
   
   if (ya < 0) {
    dir = 1;
   }
   else if (ya > 0) {
    dir = 0;
   }
  }
  
  return Unit::move(xa, ya);
 }
};

struct Fiend : Mob {
 int level;
};

struct Movable : Unit {
 
};

struct Player : Mob {
 int stamina, maxStamina;
 int cmds;

 Player() {
  stamina = maxStamina = 0;
 }

 void input(int cmds) override {
  this->cmds = cmds;
 }
 
 bool commanded(int cmd);
};

bool Player::commanded(int cmd)  {
  if ((cmds & cmd) == cmd) {
   return true;
  }
  
  return false;
 }

#define elif else if

struct unitword_s {
 std::string type;
 std::string name;

 sol::table data;
 unitfactory_f create;

 template<typename... Args>
 sol::object call(const std::string& name, Unit* unit, Args... args) {
  auto func = data[name];

  if (!ISLUATYPE(func, function)) {
   return sol::nil;
  }

  if (type == "unit") {
   return func(unit, args...);
  }
  elif (type == "mob") {
   return func((Mob*) unit, args...);
  }
  elif (type == "fiend") {
   return func((Fiend*) unit, args...);
  }
  elif (type == "player") {
   return func((Player*) unit, args...);
  }

  return sol::nil;
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
  std::string type;
  
  if ISLUATYPE(base, string) {
   auto it = factories.find(string);

   if (it == factories.end()) {
    create = []() -> Unit* {
     return new Unit();
    };
    
    type = "unit";
   }
   else {
    create = it->second;

    type = base;
   }

   unitdefs.push_back({ base, string, data, create });
  }

  LOGREPORT("loaded unit '%s' definition.", string);

  *c = '.';
 }
}

sol::table getunittable(const char* word) {
 for (auto& def : unitdefs) {
  if (def.name == word) {
   return def.data;
  }
 }

 return sol::nil;
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
  
  "move", &Unit::move,
  "touch", &Unit::touch,
  "test", &Unit::test
 );

 defineunit<Mob>("mob",
  sol::base_classes, sol::bases<Unit>(),
  
  "hp", &Mob::hp,
  "maxHP", &Mob::maxHP,
  "dir", &Mob::dir,
  "hurted", &Mob::hurted,
  "ticked", &Mob::ticked,
  "swam", &Mob::swam,
  "walked", &Mob::walked,
  
  "move", &Mob::move
 );
 
 defineunit<Movable>("movable",
  sol::base_classes, sol::bases<Unit>()
 );

 defineunit<Player>("player",
  sol::base_classes, sol::bases<Unit, Mob>(),
  "commanded", &Player::commanded,
  "dir", &Player::dir
 );
 
 //uploadsubtable(lua_unitdrop, "drop", "unit", L_game);
 //uploadsubtable(lua_unitfiend, "fiend", "unit", L_game);
 //uploadsubtable(lua_unittrace, "trace", "unit", L_game);

 return;
}
