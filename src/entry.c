#include "main.h"

#undef main

screen_t lightscreen, screen;
int debug = 0;
int ticks = 0;

void preinit() {
 loadlibraries();
 
 createview();
 
 fillpalette();
 
 initiatechecksum();
 
 seedrandomtime();
}

void init() {
 preinit();
 
 createscreen(&screen, RENDERWIDTH, RENDERHEIGHT, "res/icons.png");
 createscreen(&lightscreen, RENDERWIDTH, RENDERHEIGHT, "res/icons.png");
 
 bindkeys();
 
 enablemenu();
 enablegame();
 
 loadsounds();
 
 setmenu("TitleMenu");
}

void tick() {
 ticks++;
 
 if (!hasfocus()) {
  clearkeys();
 }
 else {
  tickkeys();
 }
 
 if (session.open) {
  tickgame();
 }
 
 if (hasfocus()) {
  tickmenu();
 }
 
 updatequeues();
}

void render() {
 rendermenu(&screen);
 
 if (!hasfocus()) {
  renderfocusnagger(&screen, ticks);
 }
 
 updatetexture(&screen);
}

void quit() {
 unloadsounds();
 
 closelibraries();
 
 LOGREPORT("successful exit.");
 
 exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
 static int r = 0, t = 0;
 watch_t timer;
 float delay = 1.f / TICKRATE;
 int shouldrender, unprocessed;
 
 LOGREPORT("compiled executable on " __DATE__ " " __TIME__ ".");
 
 init();
 
 settimer(&timer, TIMER_SPACEDLAPSE, CURRENTTIME, delay);
 
 while (1) {
  shouldrender = 0;
  unprocessed = readtimer(&timer);
  
  pollevents();
  
  handlequeues();
  
  while (unprocessed > 0) {
   tick();
   
   shouldrender = 1;
   unprocessed--;
  }
  
  if (shouldrender) {
   render();
   LOGDEBUG("renders %i.", r++);
  }
  
  LOGDEBUG("ticks %i.", t++);
  
  delaythread(delay);
 }
 
 quit();
 
 return 0;
}
