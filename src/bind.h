#pragma once

#include "base.h"

#include <sol/sol.hpp>

#include <stdarg.h>

#define LUAARGUE(Function) \
 LOGREPORT("%s; received invalid number of arguments.", Function);

#define ISLUATYPE(Function, Type) \
 (Function.valid() && Function.get_type() == sol::type::Type)

extern sol::state L;
