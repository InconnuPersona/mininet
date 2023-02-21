#include "main.h"
#include "bind.h"
#include "unituse.h"

/*#define LUAUNITELEMENT(Super, Name, Type) \
 NEWLUAFUNCTION(Super##Name) { \
  unit_u* unit; \
  int offset = offsetof(Super##_t, Name) / sizeof(int); \
  if (lua_gettop(L) >= 1) { \
   unit = getunit(luaL_checknumber(L, 1)); \
   if (unit) { \
	if (lua_gettop(L) >= 2) { \
	 unit->data[offset] = lua_tonumber(L, 2); \
	} \
    lua_push##Type(L, unit->data[offset]); \
    return 1; \
   } \
  } \
  else { \
   LUAARGUE(#Super"."#Name); \
  } \
  lua_push##Type(L, 0); \
  return 1; \
 }

#define LUAUNITMETHOD(Super, Name) \
 LUAMETHOD(Name, Super##Name)

#define LUAUNITVALUE(Super, Name) \
 NEWLUAFUNCTION(Super##Name) { \
  unitword_t* word = getunitbyword(#Super); \
  int value; \
  if (word) { \
   value = word->Name; \
  } \
  else { \
   LOGREPORT("unable to bind unitword '%s'.", #Super); \
   value = 0; \
  } \
  lua_pushnumber(L, value); \
  return 1; \
 }

#define NEWLUAUNITMETHOD(Super, Name, Type) \
 NEWLUAFUNCTION(Super##Name) { \
 if (lua_gettop(L) > 0) { \
  lua_push##Type(L, Name(luaL_checknumber(L, 1))); \
 } \
 else { \
  lua_push##Type(L, 0); \
 } \
  return 1; \
 }

//==================================================
// externals

extern void viewsurface(const char* word, refer_t client);

extern lua_State* L_game;

// ==================================================
// functions

int callunitmethod(const char* method, unit_t* unit, const char* format, ...) {
 unitword_t* word;
 va_list args;
 int value;
 
 if (!method) {
  LOGREPORT("received invalid method.");
  return 0;
 }
 
 if (!unit || unit->id == NOUNIT || !unit->extant) {
  LOGREPORT("received invalid unit.");
  return 0;
 }
 
 word = getunitword(unit->word);
 
 if (!word || !word->name) {
  LOGREPORT("unit '%x' is invalidword bound.", unit->id);
  return 0;
 }
 
 va_start(args, format);
 
 value = passmethod(method, word->name, NULL, L_game, format, args, "n", unit->id);
 
 va_end(args);
 
 return value;
}

int hasunitmethod(const char* method, unit_t* unit) {
 unitword_t* word;
 
 if (!method || !unit) {
  LOGREPORT("received invalid arguments.");
  return 0;
 }
 
 word = getunitword(unit->word);
 
 if (!word || !word->name) {
  LOGREPORT("unit '%x' is invalidword bound.", unit->id);
  return 0;
 }
 
 return hasluamethod(method, word->name, L_game);
}

// ==================================================
// lua_drop

// ==================================================
// lua_fiend

// ==================================================
// lua_mob

LUAUNITVALUE(mob, maxhealth)

//LUAUNITELEMENT(mob, pile, number)
//LUAUNITELEMENT(mob, health, number)
//LUAUNITELEMENT(mob, dir, number)
//LUAUNITELEMENT(mob, xk, number)
//LUAUNITELEMENT(mob, yk, number)
LUAUNITELEMENT(mob, hurted, number)
LUAUNITELEMENT(mob, ticked, number)
LUAUNITELEMENT(mob, swum, number)
LUAUNITELEMENT(mob, walked, number)

NEWLUAFUNCTION(mobgive) {
 mob_t* mob;
// int count;
 
 if (lua_gettop(L) >= 2) {
  mob = castmob(luaL_checknumber(L, 1));
  
  if (!mob) {
   LOGREPORT("received unbindable unit.");
   return 0;
  }
  
  if (mob->pile == NOPILE) {
   mob->pile = newpile();
  }
  
  if (lua_gettop(L) >= 3) {
//   count = luaL_checknumber(L, 3);
  }
  else {
//   count = 1;
  }
  
//  additem(luaL_checknumber(L, 2), count, mob->pile);
 }
 else {
  LUAARGUE("mob.additem");
 }
 
 return 0;
}

NEWLUAFUNCTION(mobheal) {
 if (lua_gettop(L) >= 2) {
  healmob(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
 }
 else {
  LUAARGUE("mob.heal");
 }
 
 return 0;
}

NEWLUAFUNCTION(mobhurt) {
 
 return 0;
}

NEWLUAFUNCTION(mobmove) {
 int result;
 
 if (lua_gettop(L) >= 3) {
  result = movemob(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
 }
 else {
  LUAARGUE("mob.move");
  
  result = 0;
 }
 
 lua_pushboolean(L, result);
 
 return 1;
}

// ==================================================
// lua_movable

// ==================================================
// lua_pliant

LUAUNITVALUE(pliant, maxstamina)

LUAUNITELEMENT(pliant, inhand, number)
LUAUNITELEMENT(pliant, item, number)
LUAUNITELEMENT(pliant, aim, number)
LUAUNITELEMENT(pliant, score, number)
LUAUNITELEMENT(pliant, attacktime, number)
LUAUNITELEMENT(pliant, breathtime, number)
LUAUNITELEMENT(pliant, dodgetime, number)
LUAUNITELEMENT(pliant, stairtime, number)
LUAUNITELEMENT(pliant, stamina, number)
LUAUNITELEMENT(pliant, tireddelay, number)

NEWLUAFUNCTION(pliantcommand) {
 pliant_t* pliant;
 int held, i, out, value;
 
 held = out = 0;
 
 if (lua_gettop(L) > 0) {
  pliant = (pliant_t*) getunit(luaL_checknumber(L, 1));
  
  if (!pliant) {
   LOGREPORT("could not retreive pliant unit.");
   
   goto lf_pliantcommand_exit;
  }
  
  for (i = 2; i <= lua_gettop(L); i++) {
   value = luaL_checknumber(L, i);
   
   held |= value;
   
   out |= pliant->commands & value;
  }
  
  if (held != out) {
   out = 0;
  }
 }
 else {
  LUAARGUE("unit.pliant.command");
 }
 
lf_pliantcommand_exit:
 lua_pushboolean(L, out);
 
 return 1;
}

NEWLUAFUNCTION(pliantview) {
 refer_t client;
 
 if (lua_gettop(L) > 1) {
  client = getplayerclient(luaL_checknumber(L, 1));
  
  if (client == INVALIDCLIENT) {
   LOGREPORT("received unplayed unit.");
   return 0;
  }
  
  if (lua_isstring(L, 2)) {
   viewsurface(luaL_checkstring(L, 2), client);
  }
  else {
   viewsurface(NOSURFACE, client);
  }
 }
 
 return 0;
}

// ==================================================
// lua_trace

// ==================================================
// lua_unit

LUAUNITELEMENT(unit, extant, boolean)
LUAUNITELEMENT(unit, word, number)
LUAUNITELEMENT(unit, owner, number)
LUAUNITELEMENT(unit, x, number)
LUAUNITELEMENT(unit, y, number)

LUAUNITVALUE(unit, extra)
LUAUNITVALUE(unit, halfheight)
LUAUNITVALUE(unit, halfwidth)
LUAUNITVALUE(unit, super)

NEWLUAUNITMETHOD(unit, buoyant, boolean);
NEWLUAUNITMETHOD(unit, inliquid, boolean);

NEWLUAFUNCTION(unitisclass) {
 unit_t* unit;
 int result;
 
 result = 0;
 
 if (lua_gettop(L) >= 2) {
  unit = (unit_t*) getunit(luaL_checknumber(L, 1));
  
  if (unit) {
   result = isclass(luaL_checknumber(L, 2), unit->word);
  }
  else {
   LOGREPORT("received invalid unit.");
  }
 }
 else {
  LUAARGUE("unit.isclass");
 }
 
 lua_pushboolean(L, result);
 
 return 1;
}

// ==================================================
// lua table declarations

BEGINLUATABLE(unit)
 LUAUNITMETHOD(unit, buoyant),
 LUAUNITMETHOD(unit, inliquid), 
 LUAUNITMETHOD(unit, isclass),
 
 LUAUNITMETHOD(unit, extant),
 LUAUNITMETHOD(unit, owner),
 LUAUNITMETHOD(unit, word),
 LUAUNITMETHOD(unit, x),
 LUAUNITMETHOD(unit, y),
ENDLUATABLE;

BEGINLUATABLE(unitdrop)
ENDLUATABLE;

BEGINLUATABLE(unitfiend)
ENDLUATABLE;

BEGINLUATABLE(unitmob)
// LUAUNITMETHOD(mob, give),
 LUAUNITMETHOD(mob, heal),
 LUAUNITMETHOD(mob, hurt),
 LUAUNITMETHOD(mob, move),
 
 //LUAUNITMETHOD(mob, dir),
 //LUAUNITMETHOD(mob, health),
 LUAUNITMETHOD(mob, hurted),
 //LUAUNITMETHOD(mob, pile),
 LUAUNITMETHOD(mob, swum),
 LUAUNITMETHOD(mob, ticked),
 LUAUNITMETHOD(mob, walked),
 //LUAUNITMETHOD(mob, xk),
 //LUAUNITMETHOD(mob, yk),
ENDLUATABLE;

BEGINLUATABLE(unitmovable)
ENDLUATABLE;

BEGINLUATABLE(unitpliant)
 LUANUMBER(ATTACK, CMD_ATTACK),
 LUANUMBER(CRAFT, CMD_CRAFT),
 LUANUMBER(MENU, CMD_MENU),
 LUANUMBER(MOVEDOWN, CMD_MOVEDOWN),
 LUANUMBER(MOVELEFT, CMD_MOVELEFT),
 LUANUMBER(MOVERIGHT, CMD_MOVERIGHT),
 LUANUMBER(MOVEUP, CMD_MOVEUP),
 
 LUAUNITMETHOD(pliant, maxstamina),
 LUAUNITMETHOD(pliant, view),
 
 LUAUNITMETHOD(pliant, command),
 LUAUNITMETHOD(pliant, inhand),
 LUAUNITMETHOD(pliant, item),
 LUAUNITMETHOD(pliant, aim),
 LUAUNITMETHOD(pliant, score),
 LUAUNITMETHOD(pliant, attacktime),
 LUAUNITMETHOD(pliant, breathtime),
 LUAUNITMETHOD(pliant, dodgetime),
 LUAUNITMETHOD(pliant, stairtime),
 LUAUNITMETHOD(pliant, stamina),
 LUAUNITMETHOD(pliant, tireddelay),
ENDLUATABLE;

BEGINLUATABLE(unittrace)
ENDLUATABLE;*/
