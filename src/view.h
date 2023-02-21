#pragma once

#include "gfx.h"

void loadviews();

int isview(const char* view);
void setview(const char* view);

//void char(int character);
void tickview();
void renderview(screen_t* screen);
