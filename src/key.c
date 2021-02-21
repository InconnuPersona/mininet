#include <stdlib.h>
#include <string.h>

#if !defined(__ANDROID__) && !defined(_MSC_VER)
 #include <SDL2/SDL.h>
#else
 #include <SDL.h>
#endif

#define MAX_BINDS 4
#define MAX_CKEYS 128
#define MAX_VKEYS 64
#define MAX_TOUCHES 8

#define NOKEY 0

// ==================================================
// structures

typedef struct {
 SDL_FingerID finger;
 char* alias;
 float x, y;
} touch_t;

typedef struct {
 int code;
 int state, clicked;
 int presses, absorbs;
} ckey_t;

typedef struct {
 char* name;
 int keys[MAX_BINDS];
} vkey_t;

// ==================================================
// externals

extern void menuchar(int);

// ==================================================
// declarations

ckey_t keys[MAX_CKEYS] = { 0 };
touch_t touches[MAX_TOUCHES] = { 0 };
vkey_t binds[MAX_VKEYS] = { 0 };

int key_listen = 0; // boolean

// ==================================================
// functions

int addkey(int code) {
 int last = -1;
 
 for (int i = 0; i < MAX_CKEYS; i++) {
  if (keys[i].code == 0) {
   last = i;
  }
  else if (keys[i].code == code) {
   return i;
  }
 }
 
 if (last > -1) {
  keys[last].code = code;
 }
 
 return last;
}

int getckey(int key) {
 for (int i = 0; i < MAX_CKEYS; i++) {
  if (keys[i].code == key) {
   return i;
  }
 }
 
 return -1;
}

void setckey(int code, int state) {
 int i = getckey(code);
 
 if (i < 0) {
  i = addkey(code);
 }
 
 if (i > 0) {
  keys[i].state = state;
  
  if (state) {
   keys[i].presses++;
  }
 }
}

int getkeyclicked(int key) {
 int i = getckey(key);
 
 if (i > -1) {
  return keys[i].clicked;
 }
 else {
  return 0;
 }
}

int getkeydown(int key) {
 int i = getckey(key);
 
 if (i > -1) {
  return keys[i].state;
 }
 else {
  return 0;
 }
}

void clearkeys() {
 for (int i = 0; i < MAX_CKEYS; i++) {
  keys[i].state = 0;
 }
}

void setlisten(int state) {
 key_listen = state;
}

void tickkeys() {
 for (int i = 0; i < MAX_CKEYS; i++) {
  if (keys[i].absorbs < keys[i].presses) {
   keys[i].absorbs++;
   keys[i].clicked = 1;
  }
  else {
   keys[i].clicked = 0;
  }
 }
}

void alias(int key, const char* bind) {
 int i, j, last;
 
 last = -1;
 
 for (i = 0; i < MAX_VKEYS; i++) {
  if (!binds[i].name && last < 0) {
   last = i;
  }
  else if (binds[i].name && !strcmp(binds[i].name, bind)) {
   for (j = 0; j < MAX_BINDS; j++) {
	if (binds[i].keys[j] == NOKEY) {
	 binds[i].keys[j] = key;
	 return;
	}
   }
  }
 }
 
 if (last > -1) {
  binds[last].name = (char*) bind;
  binds[last].keys[0] = key;
 }
}

int getaliasclicked(const char* bind) {
 int i, j;
 
 for (i = 0; i < MAX_VKEYS; i++) {
  if (binds[i].name && !strcmp(binds[i].name, bind)) {
   for (j = 0; j < MAX_BINDS; j++) {
	if (binds[i].keys[j] && getkeyclicked(binds[i].keys[j])) {
	 return 1;
	}
   }
   
   return 0;
  }
 }
 
 return 0;
}

int getaliasdown(const char* bind) {
 int i, j;
 
 for (i = 0; i < MAX_VKEYS; i++) {
  if (binds[i].name && !strcmp(binds[i].name, bind)) {
   for (j = 0; j < MAX_BINDS; j++) {
	if (binds[i].keys[j] && getkeydown(binds[i].keys[j])) {
	 return 1;
	}
   }
   
   return 0;
  }
 }
 
 return 0;
}

void pollevents() {
 extern SDL_Window* window;
 extern int focus;
 
 SDL_Event e;
 
 while (SDL_PollEvent(&e)) {
  switch (e.type) {
  case SDL_QUIT:
   exit(0);
   break;
   
  case SDL_FINGERDOWN:
  case SDL_FINGERUP:
   break;
   
  case SDL_FINGERMOTION:
   break;
   
  case SDL_KEYDOWN:
  case SDL_KEYUP:
   if (key_listen && e.key.state) {
	menuchar(e.key.keysym.sym);
   }
   
   setckey(e.key.keysym.sym, e.key.state);
   break;
   
  default:
   break;
  }
 }
 
 focus = SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS;
}
