#include "main.h"

#if !defined(__ANDROID__) && !defined(_MSC_VER)
 #include <dirent.h>
 #include <SDL2/SDL_mixer.h>
#else
 #include "dirent.h"
 
 #include <SDL2/SDL_mixer.h>
#endif

#define AUDIORATE 22050

#define MAX_AUDIOBUFFER 1024
#define MAX_SOUNDS 32

// ==================================================
// structures

typedef struct {
 char* name;
 Mix_Chunk* chunk;
} sound_t;

// ==================================================
// declarations

sound_t sounds[MAX_SOUNDS] = { 0 };

// ==================================================
// functions

void loadsound(const char* string) {
 Mix_Chunk* chunk;
 char buffer[64];
 char* c;
 int i;
 
 if (!string) {
  LOGREPORT("received null argument.");
  return;
 }
 
 c = (char*) strchr(string, '.');
 
 if (!c || strncmp(c, ".wav", 4) != 0) {
  return;
 }
 
 for (i = 0; i < MAX_SOUNDS; i++) {
  if (!sounds[i].name) {
   memset(buffer, 0, 64);
   
   sprintf(buffer, "res/snd/%s", string);
   
   chunk = Mix_LoadWAV(buffer);
   
   if (!chunk) {
	LOGREPORT("could not load sound file '%s' - %s.", buffer, Mix_GetError());
	return;
   }
   
   *c = '\0';
   
   sounds[i].chunk = chunk;
   sounds[i].name = reprintstring(string);
   
   *c = '.';
   
   return;
  }
 }
 
 LOGREPORT("unable to load '%s'.", string);
 
 return;
}

void loadsounds() {
 DIR* d;
 struct dirent* dir;
 
 if (Mix_OpenAudio(AUDIORATE, AUDIO_S16SYS, 2, MAX_AUDIOBUFFER) != 0) {
  LOGREPORT("could not open 'SDL2' mixer audio - %s.", Mix_GetError());
  exit(EXIT_FAILURE);
 }
 
 d = opendir("res/snd");
 
 if (d) {
  while ((dir = readdir(d))) {
   loadsound(dir->d_name);
  }
  
  closedir(d);
 }
}

void playsound(const char* sound) {
 int i;
 
 for (i = 0; i < MAX_SOUNDS; i++) {
  if (sounds[i].name && !strcmp(sounds[i].name, sound)) {
   //Mix_PlayChannel(-1, sounds[i].chunk, 0);
   return;
  }
 }
 
 LOGREPORT("could not play sound '%s'.", sound);
 
 return;
}

void unloadsounds() {
 int i;
 
 for (i = 0; i < MAX_SOUNDS; i++) {
  if (sounds[i].chunk) {
   Mix_FreeChunk(sounds[i].chunk);
  }
 }
 
 memset(sounds, 0, sizeof(sounds));
 
 Mix_CloseAudio();
 
 return;
}
