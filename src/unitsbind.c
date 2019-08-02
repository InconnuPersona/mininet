#include "main.h"
#include "bind.h"

#define LUAUNITELEMENT(Name, Type, Index)

#define LUAUNITVALUE(Name, Value)

// ==================================================
// externals

//extern level_t* le_level;

// ==================================================
// functions

void callunitmethod(const char* name, unit_u* unit, level_t* level) {
 
}

// ==================================================
// lua_drop

// ==================================================
// lua_fiend

// ==================================================
// lua_mob

LUAUNITVALUE(mob, maxhealth)

// ==================================================
// lua_movable

LUAUNITVALUE(movable, color)
LUAUNITVALUE(movable, sprite)

// ==================================================
// lua_pliant

LUAUNITVALUE(pliant, maxstamina)

// ==================================================
// lua_trace

//LUAVARIABLE(tracetime, le_unit->trace.time)

// ==================================================
// lua_unit

//LUAUNITELEMENT(unitbond, number, 3)
LUAUNITELEMENT(unitextant, boolean, 4)
//LUAUNITELEMENT(unitowner, number, 2)
LUAUNITELEMENT(unitword, number, 1)
LUAUNITELEMENT(unitx, number, 5)
LUAUNITELEMENT(unity, number, 6)

LUAUNITVALUE(unit, extra)
LUAUNITVALUE(unit, halfheight)
LUAUNITVALUE(unit, halfwidth)
LUAUNITVALUE(unit, super)

// ==================================================
// lua table declarations

BEGINLUATABLE(unit)
// LUAMETHOD(extant, unitextant),
// LUAMETHOD(x, unitx),
// LUAMETHOD(y, unity),
ENDLUATABLE;

BEGINLUATABLE(unitdrop)
ENDLUATABLE;

BEGINLUATABLE(unitfiend)
ENDLUATABLE;

BEGINLUATABLE(unitmob)
ENDLUATABLE;

BEGINLUATABLE(unitmovable)
ENDLUATABLE;

BEGINLUATABLE(unitpliant)
ENDLUATABLE;

BEGINLUATABLE(unittrace)
// LUAMETHOD(time, tracetime),
ENDLUATABLE;
