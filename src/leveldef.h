#include "main.h"
#include "bind.h"

#include "tile.h"
#include "unit.h"

// ==================================================
// definitions

struct Level {
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
 
 Level* next;
};

struct MapNoise {
 int w, h;
 float* values;

 MapNoise(int w, int h, int stepSize) {
  float a, b, c, d, e, f, g, H, modifier, scale;
  int halfstep, x, y;
  
  if (w < 1 || h < 1) {
   LOGREPORT("attempted invalid map creation.");
   return;
  }
  
  this->w = w;
  this->h = h;
  
  values = new float[w * h];
  
  for (y = 0; y < w; y += stepSize) {
   for (x = 0; x < h; x += stepSize) {
    set(x, y, randomfloat() * 2.f - 1.f);
   }
  }
 
 scale = 1.f / w;
 modifier = 1.f;
 
 do {
  halfstep = stepSize / 2.f;
  
  for (y = 0; y < h; y += stepSize) {
   for (x = 0; x < w; x += stepSize) {
	a = sample(x, y);
	b = sample(x + stepSize, y);
	c = sample(x, y + stepSize);
	d = sample(x + stepSize, y + stepSize);
	
	e = (a + b + c + d) / 4.f + (randomfloat() * 2.f - 1.f) * stepSize * scale;
	
	set(x + halfstep, y + halfstep, e);
   }
  }
  
  for (y = 0; y < h; y += stepSize) {
   for (x = 0; x < w; x += stepSize) {
	a = sample(x, y);
	b = sample(x + stepSize, y);
	c = sample(x, y + stepSize);
	
 	d = sample(x + halfstep, y + halfstep);
 	e = sample(x + halfstep, y - halfstep);
 	f = sample(x - halfstep, y + halfstep);
 	
 	H = (a + b + d + e) / 4.f + (randomfloat() * 2.f - 1.f) * stepSize * scale * 0.5f;
 	g = (a + c + d + f) / 4.f + (randomfloat() * 2.f - 1.f) * stepSize * scale * 0.5f;
 	
 	set(x + halfstep, y, H);
 	set(x, y + halfstep, g);
    }
   }
   
   stepSize /= 2.f;
   scale *= (modifier + 0.8f);
   modifier *= 0.3f;
  }
  while (stepSize > 1.f);
 }

 float get(int i, sol::this_state L) {

	 printf("mememememem\n");
  return values[i];
 }

 float sample(int x, int y) {
  return values[(x & (w - 1)) + (y & (h - 1)) * w];
 }

 void set(int x, int y, float value) {
  values[(x & (w - 1)) + (y & (h - 1)) * w] = value;
 }
};

bool leveldef = false;
Level* levels = NULL;

// ==================================================
// externals

extern Level* level;

// ==================================================
// functions

Level* getlevel(int depth) {
 Level* l;
 
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
 L.new_usertype<MapNoise>("stepnoise",
  "new", sol::no_constructor,
  
  "at", &MapNoise::get
 );
 
 L.create_named_table("level",
  "define", [](int depth, sol::table table) {
   Level* l;
   Level* p;
   
   if (!leveldef) {
    LOGREPORT("attempted to define level outside of level script.");
	return;
   }
   
   p = NULL;
   
   if (!levels) {
    l = levels = new Level();
	
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
   
   l = new Level();
   
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
  
  "noise", [](int w, int h, int stepSize) {
   return MapNoise(w, h, stepSize);
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
  "TOLAVA", TILE_TOLAVA,
  "TOLIQUID", TILE_TOLIQUID,
  "TOSAND", TILE_TOSAND,
  "TOWATER", TILE_TOWATER,
  
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


