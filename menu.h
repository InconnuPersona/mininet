#pragma once

#include "gfx.h"

void enablemenu();
void disablemenu();

int ismenu(const char* menu);
void setmenu(const char* menu);

void menuchar(int character);
void tickmenu();
void rendermenu(screen_t* screen);
