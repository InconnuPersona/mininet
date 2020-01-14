#include "main.h"

#include <errno.h>
#include <dirent.h>
#include <libgen.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifndef __ANDROID__
 #include <SDL2/SDL_image.h>
 #include <SDL2/SDL_mixer.h>
 #include <SDL2/SDL_net.h>
#else
 #include <jni.h>
 #include <SDL_image.h>
 #include <SDL_mixer.h>
 #include <SDL_net.h>
#endif

#ifdef __ANDROID__
 #define DEFAULTPATH SDL_AndroidGetInternalStoragePath()
#else
 #define DEFAULTPATH "."
#endif

#ifdef _WIN32
 #define MAKEDIR(Path) mkdir(Path)
#else
 #define MAKEDIR(Path) mkdir(Path, 0)
#endif

#define MAX_FILES 8

typedef struct {
 SDL_RWops* handle;
 refer_t id;
 char* string;
} file_t;

file_t files[MAX_FILES] = { 0 };

char pathholder[MAX_PATHLENGTH] = { 0 };

// ==================================================
// file functions

void copyfile(const char* source, const char* target) {
 refer_t id;
 
 if (!source || !target) {
  LOGREPORT("received invalid file paths.");
  return;
 }
 
 id = readfile(source);
 
 if (id == NOFILE) {
  LOGREPORT("unable to read file '%s.'", source);
  return;
 }
 
 writefile(target, (byte_t*) filetext(id), fileangth(id));
 
 freefile(id);
 
 return;
}

void freefile(refer_t file) {
 int i;
 
 for (i = 0; i < MAX_FILES; i++) {
  if (files[i].id == file) {
   break;
  }
 }
 
 if (i == MAX_FILES) {
  LOGREPORT("unable to find file slot [%i].", file);
  return;
 }
 
 if (files[i].handle) {
  SDL_RWclose(files[i].handle);
 }
 
 files[i].handle = NULL;
 files[i].id = NOFILE;
 
 if (files[i].string) {
  free(files[i].string);
 }
 
 return;
}

int fileangth(refer_t file) {
 int i;
 
 for (i = 0; i < MAX_FILES; i++) {
  if (files[i].id == file) {
   break;
  }
 }
 
 if (i == MAX_FILES) {
  LOGREPORT("unable to find file slot [%i].", file);
  return -1;
 }
 
 return SDL_RWsize(files[i].handle);;
}

int fileextant(const char* path) {
 SDL_RWops* handle;
 
 if (!path) {
  LOGREPORT("received invalid file path.");
  return 0;
 }
 
 handle = SDL_RWFromFile(path, "rb");
 
 if (!handle) {
  return 0;
 }
 
 SDL_RWclose(handle);
 
 return 1;
}

const char* filetext(refer_t file) {
 int i;
 
 for (i = 0; i < MAX_FILES; i++) {
  if (files[i].id == file) {
   break;
  }
 }
 
 if (i == MAX_FILES) {
  LOGREPORT("unable to find file slot [%i].", file);
  return NULL;
 }
 
 return files[i].string;
}

const char* getfilepath(const char* path) {
 if (!path) {
  LOGREPORT("received invalid file path.");
  return NULL;
 }
 
 if (!strcmp(path, ".")) {
  strncpy(pathholder, DEFAULTPATH, MAX_PATHLENGTH);
  
  return pathholder;
 }
 
 snprintf(pathholder, MAX_PATHLENGTH, "%s/%s", DEFAULTPATH, path);

 return pathholder;
}

int makepath(const char* given) {
 struct stat info;
 char* c;
 char* s;
 char path[MAX_PATHLENGTH];
 
 if (!given) {
  LOGREPORT("received invalid path.");
  return 0;
 }
 
 strncpy(path, given, MAX_PATHLENGTH);
 
 s = path;
 
 while ((c = strtok(s, "/"))) {
  if (c != s) {
   *(c - 1) = '/';
  }
  
  if (stat(path, &info) != EXIT_SUCCESS) {
   if (MAKEDIR(path) != EXIT_SUCCESS) {
	LOGREPORT("unable to create entry '%s", path);
	return 0;
   }
  }
  else if (!S_ISDIR(info.st_mode)) {
   LOGREPORT("unable to make path '%s'", path);
   return 0;
  }
  
  s = NULL;
 }
 
 return 0;
}

refer_t newfileid() {
 int i, id;
 
 do {
 newfileid_loop:
  id = randomid();
  
  for (i = 0; i < MAX_FILES; i++) {
   if (id == files[i].id) {
    goto newfileid_loop;
   }
  }
  
  return id;
 }
 while (1);
}

refer_t readfile(const char* path) {
 long length;
 int i, read;
 
 if (!path) {
  LOGREPORT("received invalid file path.");
  return NOFILE;
 }
 
 for (i = 0; i < MAX_FILES; i++) {
  if (!files[i].id) {
   break;
  }
 }
 
 if (i == MAX_FILES) {
  LOGREPORT("unable to find free slot for file '%s'.", path);
  return NOFILE;
 }
 
 files[i].handle = SDL_RWFromFile(path, "rb");
 files[i].id = newfileid();
 
 if (!files[i].handle) {
  LOGREPORT("unable to open file '%s'.", path);
  
  goto readfile_error;
 }
 
 length = fileangth(files[i].id);
 
 if (length < 0) {
  LOGREPORT("unable to tell file length of '%s'.", path);
  
  goto readfile_error;
 }
 
 files[i].string = malloc(length + 1);
 
 if (!files[i].string) {
  LOGREPORT("unable to allocate file '%s' contents (%li bytes).", path, length);
  
  goto readfile_error;
 }
 
 read = SDL_RWread(files[i].handle, files[i].string, sizeof(char), length);
 
 if (read != length) {
  LOGREPORT("unable to read file '%s'.", path);
  
  goto readfile_error;
 }
 
 files[i].string[length] = '\0';
 
 LOGREPORT("read file '%s'.", path);
 
 return files[i].id;
 
readfile_error:
 freefile(files[i].id);
 
 return NOFILE;
}

// Entering a negative recurse level count will sort through all levels under
// the file system in practicallity; since there would not be over two million
// files the program needs to handle. choosing an overly large negative number
// is unsafe though, and could shorten the limit tolerance.
void recursepath(const char* path, recurse_f function, int level) {
 struct dirent* entry;
 DIR* d;
 char buffer[MAX_PATHLENGTH];
 
 d = opendir(path);
 
 if (d) {
  while ((entry = readdir(d))) {
   if (entry->d_type == DT_REG) {
	function(path, entry->d_name);
   }
   else if (level != 0 && entry->d_type == DT_DIR) {
	if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
	 snprintf(buffer, MAX_PATHLENGTH, "%s/%s", path, entry->d_name);
	 
	 recursepath(buffer, function, level - 1);
	}
   }
  }
  
  closedir(d);
 }
 else {
  LOGREPORT("unable to open directory '%s'.", path);
 }
}

void writefile(const char* path, const byte_t* bytes, long length) {
 SDL_RWops* handle;
 int written;
 
 if (!path || !bytes || length < 0) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 handle = SDL_RWFromFile(path, "wb");
 
 if (!handle) {
  LOGREPORT("unable to open file '%s'.", path);
  
  goto writefile_end;
 }
 
 written = SDL_RWwrite(handle, bytes, sizeof(byte_t), length);
 
 if (written != length) {
  LOGREPORT("unable to write to file '%s'.", path);
  
  goto writefile_end;
 }
 
 LOGREPORT("wrote to file '%s.", path);
 
writefile_end:
 SDL_RWclose(handle);
 
 return;
}

// ==================================================
// game functions

void renderfocusnagger(screen_t* screen, int ticks) {
 const char* msg = "Click to focus!";
 int color;
 int x, y, xx, yy, w, h;
 
 xx = (RENDERWIDTH - strlen(msg) * 8) / 2;
 yy = (RENDERHEIGHT - 8) / 2;
 w = strlen(msg);
 h = 1;
 
 color = getcolor(-1, 1, 5, 445);
 
 rendersprite(xx - 1 * 8, yy - 8, 0 + 13 * 32, color, 0, screen);
 rendersprite(xx + w * 8, yy - 8, 0 + 13 * 32, color, 1, screen);
 rendersprite(xx - 1 * 8, yy + 8, 0 + 13 * 32, color, 2, screen);
 rendersprite(xx + w * 8, yy + 8, 0 + 13 * 32, color, 3, screen);
 
 for (x = 0; x < w; x++) {
  rendersprite(xx + x * 8, yy - 8, 1 + 13 * 32, color, 0, screen);
  rendersprite(xx + x * 8, yy + 8, 1 + 13 * 32, color, 2, screen);
 }
 
 for (y = 0; y < h; y++) {
  rendersprite(xx - 1 * 8, yy + y * 8, 2 + 13 * 32, color, 0, screen);
  rendersprite(xx + w * 8, yy + y * 8, 2 + 13 * 32, color, 1, screen);
 }
 
 if ((ticks / (TICKRATE / 3)) % 2 == 0) {
  renderfont(msg, xx, yy, getcolor(5, 333, 333, 333), screen);
 }
 else {
  renderfont(msg, xx, yy, getcolor(5, 555, 555, 555), screen);
 }
}

// ==================================================
// library functions

void bindkeys() {
 alias(SDLK_UP, "up");
 alias(SDLK_w, "up");
 alias(SDLK_DOWN, "down");
 alias(SDLK_s, "down");
 alias(SDLK_LEFT, "left");
 alias(SDLK_a, "left");
 alias(SDLK_RIGHT, "right");
 alias(SDLK_d, "right");
 alias(SDLK_c, "attack");
 alias(SDLK_RETURN, "menu");
 alias(SDLK_x, "menu");
 alias(SDLK_ESCAPE, "menu");
}

void closelibraries() {
 SDLNet_Quit();
 Mix_Quit();
 IMG_Quit();
 SDL_Quit();
}

float currenttime() {
 return SDL_GetTicks() / 1000.f;
}

void delaythread(float seconds) {
 fflush(stdout);
 
 SDL_Delay(seconds * 1000.f);
}

void loadasset(const char* path, const char* file) {
 DIR* dir;
 char source[MAX_PATHLENGTH];
 char target[MAX_PATHLENGTH];
 
 snprintf(source, MAX_PATHLENGTH, "%s/%s", path, file);
 snprintf(target, MAX_PATHLENGTH, "%s", getfilepath(source));
 
 if (fileextant(target)) {
  return;
 }
 
 dir = opendir(getfilepath(path));
 
 if (dir) {
  closedir(dir);
 }
 else {
  makepath(getfilepath(path));
 }
 
// LOGDEBUG("copying file '%s' to '%s'.", source, target);

 copyfile(source, target);
 
 return;
}

void loadassets() {
#ifndef __ANDROID__
 LOGREPORT("loading assets into '%s'.", getfilepath("."));
 
 recursepath("res", loadasset, -1);
#endif
 
 return;
}

void loadlibraries() {
 int sdlflags = SDL_INIT_AUDIO | SDL_INIT_VIDEO;
 int sdlimageflags = IMG_INIT_PNG;
 int sdlmixerflags = 0; //MIX_INIT_MP3;
 
 if (SDL_Init(sdlflags) != 0) {
  LOGREPORT("could not load 'SDL2' library - %s.", SDL_GetError());
  exit(EXIT_FAILURE);
 }
 
 if (IMG_Init(sdlimageflags) != sdlimageflags) {
  LOGREPORT("could not load 'SDL2' image library - %s.", IMG_GetError());
  exit(EXIT_FAILURE);
 }
 
 if (Mix_Init(sdlmixerflags) != sdlmixerflags) {
  LOGREPORT("could not load 'SDL2' mixer library - %s.", Mix_GetError());
  // exit(EXIT_FAILURE);
 }
 
 if (SDLNet_Init() == -1) {
  LOGREPORT("could not load 'SDL2' network library - %s.", SDLNet_GetError());
  exit(EXIT_FAILURE);
 }
 
 return;
}

void logreport(const char* function, const char* format, ...) {
 va_list args;
 char buffer[256];
 
 if (!function || !function) {
  return;
 }
 
 snprintf(buffer, sizeof(buffer), "%s: %s", function, format);
 
 va_start(args, format);
 
 SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, buffer, args);
 
 va_end(args);
 
 return;
}

void printhostdata() {
 int endian = 0x01;
 
 LOGREPORT("host;");
 
 LOGREPORT(" character width: '%i'", sizeof(char));
 LOGREPORT(" endian: %s", (int) ((byte_t*) &endian)[0] ? "big" : "little");
 LOGREPORT(" integer width: '%i'", sizeof(int));
 LOGREPORT(" pointer width: '%i'", sizeof(void*));
 
 return;
}

char* reprintstring(const char* string) {
 char* copy;
 int length;
 
 if (!string) {
  return 0;
 }
 
 length = strlen(string) + 1;
 
 copy = (char*) malloc(length);
 
 memcpy(copy, string, length);
 
 return copy;
}
