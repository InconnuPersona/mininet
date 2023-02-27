#include "level.h"
#include "bind.h"

#include "tile.h"

// ==================================================
// definitions

struct level_s {
 int w, h;
 int depth;
 
 int seed;
 
 sol::table data;
 
 //int dirtcolor, sandcolor, grasscolor;
 //int density; // monster, unit, and mob density
 
 //byte_t* dirties; // chunk bitmap
 
 //chunk_t* chunks;
 


 tile_s* tiles;
 
 refer_t* tileunits;
 
 //unit_u units[MAX_UNITS];
 
 level_s* next;
};

struct noise_s {
 int w, h;
 float* values;
};

bool leveldef = false;
level_s* levels = NULL;

// ==================================================
// externals

extern level_s* level;

// ==================================================
// functions

level_s* getlevel(int depth) {
 level_s* l;
 
 l = levels;
 
 while (l) {
  if (l->depth == depth) {
   return l;
  }
  
  l = l->next;
 }
 
 return NULL;
}

//void resetlevel() {
//	
//}

void enablelevel() {
 /*L.new_usertype<noise_s>("stepnoise",
  ""
 };*/
 
 L.create_named_table("level",
  "define", [](int depth, sol::table table) {
   level_s* l;
   level_s* p;
   
   if (!leveldef) {
    LOGREPORT("attempted to define level outside of level script.");
	return;
   }
   
   p = NULL;
   
   if (!levels) {
    l = levels = new level_s();
	
    goto level_define;
   }
   
   l = levels;
   
   do {
    if (l->depth == depth) {
	 LOGREPORT("level of depth %i already defined", depth);
	 exit(EXIT_FAILURE);
	}
	
	if (l->depth < depth) {
	 break;
	}
	
	p = l;
	l = l->next;
   }
   while (l);
   
   l = new level_s();
   
  level_define:
   l->data = table;
   l->depth = depth;
   
   if (p && p->next) {
    l->next = p->next;
    p->next = l;
   }
   
   return;
  },
  
  "depth", []() {
   if VALIDLEVEL{
    return level->depth;
   }
   
   return 0;
  },
  
  "getTile", [](int x, int y) {
    if VALIDLEVEL {
	 return gettile(x, y);
	}
  },
  
  "noise", [](int w, int h, int stepsize) {
   
  },
  
  "seed", []() {
   if (level) {
    return level->seed;
   }
   
   return 0;
  },
  
  "setData", [](int x, int y, int data) {
   
  },
  
  "setTile", [](int x, int y, int id, int data) {
   if VALIDLEVEL {
    settile(x, y, id, data);
   }
  },
  
  "random", []() {
  },
  
  "size", []() {
   if VALIDLEVEL {
    return std::tuple(level->w, level->h);
   }
   
   return std::tuple(0, 0);
  }
 );
 
 L.create_named_table("tile",
  "TOGRASS", TILE_TOGRASS,
  "TOLAVA", TILE_TOGRASS,
  "TOLIQUID", TILE_TOGRASS,
  "TOSAND", TILE_TOGRASS,
  "TOWATER", TILE_TOGRASS,
  
  "define", [](int index, const char* name, sol::table data) {
   assigntile(index, name, data);
  },
  
  "hasFlag", [](int id, int flag) {
   return hasflags(flag, id);
  },
  
  "id", [](const char* name) {
   return tileid(name);
  },
  
  "scale", TILESCALE
 );

 assigntile(0, "void", sol::nil);
}


