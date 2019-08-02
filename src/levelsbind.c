#include "level.h"
#include "bind.h"

// ==================================================
// declarations

level_t* le_level;

//int inliquid(refer_t unit, level_t* level) {
 
// bindlevel(level);
 
 //CHECKLEVELUNIT;
 
// return 0;
//}

// ==================================================
// lua binding

NEWLUAFUNCTION(dirtcolor) {
 //CHECKBOUNDLEVEL;
 
 //lua_pushnumber(L, level->dirtcolor);
 
 return 1;
}

NEWLUAFUNCTION(grasscolor) {
 //CHECKBOUNDLEVEL;
 
 //lua_pushnumber(L, level->grasscolor);
 
 return 1;
}

NEWLUAFUNCTION(sandcolor) {
 //CHECKBOUNDLEVEL;
 
 //lua_pushnumber(L, level->sandcolor);
 
 return 1;
}

NEWLUAFUNCTION(levelheight) {
 //CHECKBOUNDLEVEL;
  
 // lua_pushnumber(L, level->h);
  
  return 1;
}

NEWLUAFUNCTION(levelwidth) {
 //CHECKBOUNDLEVEL;
  
 // lua_pushnumber(L, level->w);
  
  return 1;
}

NEWLUAFUNCTION(getdata) {
 
 return 1;
}

NEWLUAFUNCTION(gettile) {
 
 return 1;
}

NEWLUAFUNCTION(getunits) {
 
 return 1;
}

NEWLUAFUNCTION(setdata) {
 
 return 0;
}

NEWLUAFUNCTION(settile) {
 
 return 0;
}

NEWLUAFUNCTION(spawn) {
 
 return 0;
}

NEWLUAFUNCTION(summon) {
 
 return 0;
}

BEGINLUATABLE(level)
 LUAFUNCTION(dirtcolor),
 LUAFUNCTION(grasscolor),
 LUAFUNCTION(sandcolor),
 
 LUAFUNCTION(gettile),
 LUAFUNCTION(getunits),
 
 LUAMETHOD(height, levelheight),
 LUAMETHOD(width, levelwidth),
 
 LUAFUNCTION(settile),
 LUAFUNCTION(summon),
 LUAFUNCTION(spawn), // initiate
ENDLUATABLE;
