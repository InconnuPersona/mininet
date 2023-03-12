#include "leveldef.h"

#define CHECKTILEBOUNDS(X, Y, Level, ...) \
 if (X < 0 || Y < 0 || X >= Level->w || Y >= Level->h) { \
  __VA_ARGS__; \
 }

LevelI level;
Level* bound;

void LevelI::bind(int depth) {
 Level* l;
 
 l = getlevel(depth);
 
 if (l) {
  bound = l;

  L["level"]["data"] = l->data;
 }
}

int LevelI::seed() {
 if (valid()) {
  return bound->seed;
 }

 return 0;
}

int LevelI::w() {
 if (valid()) {
  return bound->w;
 }

 return 0;
}

int LevelI::h() {
 if (valid()) {
  return bound->h;
 }

 return 0;
}

int LevelI::depth() {
 if (valid()) {
  return bound->depth;
 }

 return 0;
}

//
void LevelI::generate(int w, int h, int seed) {
 Level* l;
 Level* p;
 
 if (w < 1 || h < 1) {
  LOGREPORT("received invalid level bounds.");
  return;
 }
 
 l = levels;
 p = NULL;
 
 while (l) {
  bound = l;
  
  bound->seed = seed;

 retry:
  emptylevel(bound, w, h);
  
  auto generate = l->data["generate"];
  auto validate = l->data["validate"];
  
  if ISLUATYPE(generate, function) {
   generate(w, h);
  }
  
  if ISLUATYPE(validate, function) {
   auto state = validate(w, h);
   
   if (!ISLUATYPE(state, boolean)) {
    LOGREPORT("validate did not return a boolean");
	exit(EXIT_FAILURE);
   }
   
   if (!state) {
    goto retry;
   }
  }
  
  if (p) {
   //matchstairs(p);
  }
  
  bound = NULL;
  
  p = l;
  l = l->next;
 }
}

int LevelI::getData(int x, int y) {
 CHECKTILEBOUNDS(x, y, bound, return 0);

 return bound->tiles[x + y * bound->w].data & BYTEMASK;
}

int LevelI::getTile(int x, int y) {
 CHECKTILEBOUNDS(x, y, bound, return NOTILE);
 
 return bound->tiles[x + y * bound->w].id;
}

void LevelI::setData(int x, int y, int data) {
 CHECKTILEBOUNDS(x, y, bound, return);
 
 bound->tiles[x + y * bound->w].data = data;
 
 //dirtychunk(x / CHUNKANGTH, y / CHUNKANGTH, level);
}

void LevelI::setTile(int x, int y, int tile, int data) {
 CHECKTILEBOUNDS(x, y, bound, return);
 
 bound->tiles[x + y * bound->w].id = tile;
 bound->tiles[x + y * bound->w].data = data;
 
 //dirtychunk(x / CHUNKANGTH, y / CHUNKANGTH, level);
}

bool LevelI::hasUnit(refer_t id) {
 for (auto unit : units) {
  if (unit.first->id == id) {
   return true;
  }
 }
 
 return false;
}

Unit* LevelI::getUnit(refer_t id) {
 for (auto unit : units) {
  if (unit.first->id == id) {
   return unit.first;
  }
 }

 LOGREPORT("unable to locate unit '%x'.", id);
 
 return NULL;
}

//void LevelI::drop();

refer_t LevelI::spawn(const char* word) {
 Unit* unit;
 unitword_s* base;
 int idx;

 if (!valid()) {
    LOGREPORT("attempted to spawn unit in invalid level.")
 }

 base = begetunit(word, &idx);
 
 if (!base) {
  LOGREPORT("unable to beget unit '%s'.", word);
  return NOUNIT;
 }

 unit = NULL;

 for (auto& type : factories) {
  if (type.first == base->type) {
   unit = type.second();
   break;
  }
 }

 if (!unit) {
  LOGREPORT("invalid unit type '%s' encountered.", base->type.c_str());

  exit(EXIT_FAILURE);
 }

 unit->base = idx;
 unit->id = createunitid();

 auto findSpawn = base->data["findSpawn"];

 if ISLUATYPE(findSpawn, function) {
  sol::tie(unit->x, unit->y) = findSpawn(unit);
 }
 else {
  unit->x = randominteger(bound->w) * TILESCALE;
  unit->y = randominteger(bound->h) * TILESCALE;
 }
 
 //inserttileunit(GETUNIT(unit, level).id, GETUNIT(unit, level).x >> 4, GETUNIT(unit,level).y >> 4, level);
 
 LOGREPORT("spawned unit '%s' with id '%x' at [%i, %i].", word, unit->id, unit->x, unit->y);
 
 units.insert({ unit, bound->depth });

 return unit->id;
}

void rendertiles(int xs, int ys, screen_t* screen) {
 int tile, x, y, xo, yo, w, h;
 
 if (!screen) {
  LOGREPORT("received invalid screen.");
  return;
 }
 
 if (xs < 0 || ys < 0) {
  return;
 }
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo; y <= h + yo; y++) {
  for (x = xo; x <= w + xo; x++) {
   tile = level.getTile(x, y);
   
   if (tiles[tile].data.valid()) {
    auto render = tiles[tile].data["render"];
    
    if ISLUATYPE(render, function) {
     render(x, y);
    }
   }
  }
 }
 
 INDEBUG(2,
 for (y = yo; y <= h + yo; y++) {
  for (x = xo; x <= w + xo; x++) {
   if (x % CHUNKANGTH == 0 && y % CHUNKANGTH == 0) {
	char chars[16];
	
	sprintf(chars, "%i,%i", x / CHUNKANGTH, y / CHUNKANGTH);
	
	renderfont(chars, x * 16, y * 16, getcolor(0, 555, 555, 555), screen);
   }
  }
 });
 
 offsetscreen(screen, 0, 0);
}

#define GETTILEUNIT(I, X, Y, Level) \
 (Level->tileunits[I + (X + Y * Level->w) * MAX_TILEUNITS])

void renderlights(int xs, int ys, screen_t* screen) {
 int i, lr, x, y, xo, yo, w, h, r;
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 r = 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo - r; y <= h + yo + r; y++) {
  for (x = xo - r; x <= w + xo + r; x++) {
   CHECKTILEBOUNDS(x, y, bound, continue);
   
   /*for (i = 0; i < MAX_TILEUNITS; i++) {
	lr = getlighting(GETTILEUNIT(x, y, i, level));
	
	if (lr > 0) {
	 renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
	}
   }
   
   lr = gettileglow(x, y);
   
   if (lr > 0) {
	renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
   }*/
  }
 }
 
 offsetscreen(screen, 0, 0);
}

// Sorts through the tile units and renders them on the screen according to position.
void sortandrender(std::vector<Unit*>& units, int count, Level* level, screen_t* screen) {
 int i;
 
 if (!screen || count < 0) {
  LOGREPORT("received invalid parameters.");
  return;
 }
 
 qsort(&units[0], count, sizeof(refer_t), [](const void* v1, const void* v2) {
  Unit* u1 = (Unit*) v1;
  Unit* u2 = (Unit*) v2;
  
  if (u2->y < u1->y) {
   return 1;
  }

  if (u2->y > u1->y) {
   return -1;
  }

  return 0;
 });
 
 le_screen = screen;

 for (i = 0; i < count; i++) {
  auto render = unitdefs[units[i]->base].data["render"];

  if ISLUATYPE(render, function) {
   render(units[i]);
  }
 }
}

void rendersprites(int xs, int ys, screen_t* screen) {
 static std::vector<Unit*> samples;
 int i, j, x, y, xo, yo, w, h;
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 
 offsetscreen(screen, xs, ys);
 
 samples.clear();

 for (y = yo; y <= h + yo; y++) {
  j = 0;
  
  for (x = xo; x <= w + xo; x++) {
   CHECKTILEBOUNDS(x, y, bound, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	if (GETTILEUNIT(i, x, y, bound) != NOUNIT) {
	 samples[j] = GETTILEUNIT(i, x, y, bound);
	 j++;
	}
   }
  }
  
  if (j > 0) {
   sortandrender(samples, j, bound, screen);
  }
 }
 
 offsetscreen(screen, 0, 0);
}

void LevelI::render(int xs, int ys, screen_t* screen) {
 extern screen_t lightscreen;
 int color;
 int x, y;
 
 if (!valid()) {
  return;
 }

 if (bound->depth > 3) {
  color = getcolor(20, 20, 121, 121);
  
  for (y = 0; y < 14; y++) {
   for (x = 0; x < 24; x++) {
	rendersprite(x * 8 - ((xs / 4) & 7), y * 8 - ((ys / 4) & 7), 0, color, 0, screen);
   }
  }
 }
 
 rendertiles(xs, ys, screen);
 //rendersprites(xs, ys, screen);
 
 if (bound->depth > 3) {
  clearscreen(&lightscreen, 0);
  
  renderlights(xs, ys, &lightscreen);
  
  overlayscreens(screen, &lightscreen, xs, ys);
 }
 
 return;
}

void LevelI::getUnits(aabb_t bb, std::vector<Unit*>& units) {

}

void LevelI::tickUnits(aabb_t bb) {
 Unit* unit;
 int i, xt, yt, xto, yto;
 
 for (auto& it : units) {
  unit = it.first;

  if (!unit) {
   continue;
  }

  xto = unit->x >> 4;
  yto = unit->y >> 4;

  if (unit->exist) {
   unitdefs[unit->base].call("tick", unit);
   
   xt = unit->x >> 4;
   yt = unit->y >> 4;
   
   if (xto != xt || yto != yt) {
	//removetileunit(unit->id, xto, yto, level);
	//inserttileunit(unit->id, xt, yt, level);
   }
  }
  else {
   //efface(it);
  }
 }
}

void LevelI::tick() {
 tickUnits({});
}

bool LevelI::valid() {
 if (!bound) {
  return false;
 }

 return true;
}
