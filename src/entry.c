#include "main.h"

#undef main

#ifdef __ANDROID__
 #define main SDL_main
#endif

screen_t lightscreen, screen;
int debug = 0;
int renders = 0;
int ticks = 0;

void preinit() {
 loadlibraries();
 
 loadassets();
 
 createview();
 
 fillpalette();
 
 initiatechecksum();
 
 seedrandomtime();
}

void init() {
 preinit();
 
 INDEBUG(printhostdata());
 
 createscreen(&screen, RENDERWIDTH, RENDERHEIGHT, getfilepath("res/icons.png"));
 createscreen(&lightscreen, RENDERWIDTH, RENDERHEIGHT, getfilepath("res/icons.png"));
 
 bindkeys();
 
 enablemenu();
 enablegame();
 
 loadsounds();
 loadviews();
 
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
 
// updatequeues();
}

void render() {
 renders++;
 
 rendermenu(&screen);
 
 if (!hasfocus()) {
  renderfocusnagger(&screen, ticks);
 }
 
 updatetexture(&screen);
 
 return;
}

void quit() {
 unloadsounds();
 
 closelibraries();
 
 LOGREPORT("successful exit.");
 
 exit(EXIT_SUCCESS);
}

void printgameframes() {
 static int lastrender = 0, lasttick = 0;
 static float lasttime = 0;
 float current;
 
 current = currenttime();
 
 if (current - lasttime >= 1) {
  LOGDEBUG("rendered %i frames and %i ticks in %f seconds.", renders - lastrender, ticks - lasttick, current - lasttime);
  
  lastrender = renders;
  lasttick = ticks;
  lasttime = current;
 }
 
 return;
}

int main(int argc, char** argv) {
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
  
//  handlequeues();
  
  while (unprocessed > 0) {
   tick();
   
   shouldrender = 1;
   unprocessed--;
  }
  
  if (shouldrender) {
   render();
  }
  
  INDEBUG(printgameframes());
  
  delaythread(delay);
 }
 
 quit();
 
 return 0;
}
