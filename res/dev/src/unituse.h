#pragma once

#include "unit.h"

int callunitmethod(const char* method, unit_t* unit, const char* format, ...);
int hasunitmethod(const char* method, unit_t* unit);

mob_t* castmob(refer_t id);
pliant_t* castpliant(refer_t id);
unit_t* castunit(refer_t id);

void blankunit(refer_t unit);
int buoyant(refer_t unit);
int getlighting(refer_t unit);
int inliquid(refer_t unit);
int movestep(refer_t unit, int xa, int ya);
int moveunit(refer_t unit, int xa, int ya);
int passable(refer_t unit, refer_t other);
void touchunit(refer_t other, refer_t unit);

int findspawn(refer_t mob);
void freestrike(int xt, int yt, refer_t mob);
void healmob(refer_t id, int amount);
void killmob(refer_t mob);
int movemob(refer_t mob, int xa, int ya);
void strike(refer_t other, refer_t mob);
int swimming(refer_t mob);
