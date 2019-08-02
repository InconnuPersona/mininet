#include "main.h"

#undef main

screen_t screen, lightscreen;
int debug = 0;
int ticks = 0;

void preinit() {
 loadlibraries();
 
 createview();
 
 fillpalette();
 
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
 watch_t timer;
 float delay = 1.f / TICKRATE;
 int shouldrender, unprocessed;
 
 LOGREPORT("compiled executable on " __DATE__ " " __TIME__ ".");
 
 init();
 
 settimer(&timer, TIMER_SIMPLELAPSE, CURRENTTIME, delay);
 
 while (1) {
  unprocessed = readtimer(&timer);
  
  pollevents();
  
  handlequeues();
  
  while (unprocessed > 0) {
   tick();
   
   unprocessed -= 1;
   shouldrender = 1;
  }
  
  if (shouldrender) {
   render();
  }
  
  delaythread(delay);
 }
 
 quit();
 
 return 0;
}
