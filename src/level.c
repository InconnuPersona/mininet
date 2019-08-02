#include "level.h"

#define CHECKBOUNDLEVEL \
 if (!level) { \
  LOGREPORT("level not bound"); \
  exit(EXIT_FAILURE); \
 }

#define CHECKTILEBOUNDS(X, Y, ...) \
 if (X < 0 || Y < 0 || X >= level->w || Y >= level->h) { \
  __VA_ARGS__; \
 }

#define CHECKUNIT(Unit, ...) \
 Unit++; \
 if (Unit < 0 || Unit >= MAX_UNITS) { \
  __VA_ARGS__; \
 }

#define GETTILEUNIT(X, Y, I) level->tileunits[X + Y * level->w + I]

// ==================================================
// externals

extern void generate(level_t* level);

// ==================================================
// variables

level_t* level;
refer_t samples[MAX_SAMPLES];

// ==================================================
// private functions

void dirtychunk(int x, int y) {
 int byte, index;
 
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 index = x / CHUNKANGTH + y * level->w / (CHUNKANGTH * CHUNKANGTH);
 
 byte = index / BYTEWIDTH;
 byte = index - (byte * BYTEWIDTH);
 byte = 1 << byte;
 
 level->dirties[index / BYTEWIDTH] |= byte;
}

refer_t emptyunit() {
 int i;
 
 CHECKBOUNDLEVEL;
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (!level->units[i].base.word) {
   return i;
  }
 }
 
 return -1;
}

void encircle(int x, int y, int tile) {
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 settile(x - 1, y, tile, 0);
 settile(x + 1, y, tile, 0);
 settile(x, y - 1, tile, 0);
 settile(x, y + 1, tile, 0);
 settile(x - 1, y - 1, tile, 0);
 settile(x - 1, y + 1, tile, 0);
 settile(x + 1, y - 1, tile, 0);
 settile(x + 1, y + 1, tile, 0);
}

void inserttileunit(refer_t unit, int x, int y) {
 int i, tile;
 
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 tile = x + y * level->w;
 
 for (i = tile; i < tile + MAX_TILEUNITS; i++) {
  if (!level->tileunits[i]) {
   level->tileunits[i] = unit;
   return;
  }
 }
}

int intersect(refer_t unit, aabb_t bb) {
 int r, x, y;
 
 CHECKUNIT(unit, return 0);
 
 x = level->units[unit - 1].base.x;
 y = level->units[unit - 1].base.y;
 
 //r = unitradius(level->units[unit - 1].base.word);
 
 return !(x + r < bb.x0 || y + r < bb.y0 || x - r > bb.x1 || y - r > bb.y1);
}

void matchstairs(level_t* parent) {
 int x, y;
 
 CHECKBOUNDLEVEL;
 
 for (x = 0; x < level->w; x++) {
  for (y = 0; y < level->h; y++) {
   if (parent->tiles[x + y * parent->w].id == tileid("stairsDown")) {
	settile(x, y, tileid("stairsUp"), 0);
	
	if (level == 0) {
	 encircle(x, y, tileid("hardRock"));
	}
	else {
	 encircle(x, y, tileid("dirt"));
	}
   }
  }
 }
}

void relayunit(refer_t unit) {
 
}

void removetileunit(refer_t unit, int x, int y) {
 int i, tile;
 
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 tile = x + y * level->w;
 
 for (i = tile; i < tile + MAX_TILEUNITS; i++) {
  if (level->tileunits[i] == unit) {
   level->tileunits[i] = NOUNIT;
   return;
  }
 }
}

int spritesort(refer_t u1, refer_t u2) {
 unit_t* unit1;
 unit_t* unit2;
 
 unit1 = &level->units[u1].base;
 unit2 = &level->units[u2].base;
 
 if (unit2->y < unit1->y) {
  return 1;
 }
 
 if (unit2->y > unit1->y) {
  return -1;
 }
 
 return 0;
}

void ticktile(int x, int y) {
 CHECKBOUNDLEVEL;
 
 dirtychunk(x, y);
}

// ==================================================
// render functions

void renderbackground(int xs, int ys, screen_t* screen) {
 int x, y, xo, yo, w, h;
 
 CHECKBOUNDLEVEL;
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo; y <= h + yo; y++) {
  for (x = xo; x <= w + xo; x++) {
   rendertile(x, y, level, screen);
  }
 }
 
 offsetscreen(screen, 0, 0);
}

void renderlights(int xs, int ys, screen_t* screen) {
 int i, lr, x, y, xo, yo, w, h, r;
 
 CHECKBOUNDLEVEL;
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 r = 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo - r; y <= h + yo + r; y++) {
  for (x = xo - r; x <= w + xo + r; x++) {
   CHECKTILEBOUNDS(x, y, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	//lr = bioluminance(GETTILEUNIT(x, y, i));
	
	//if (lr > 0) {
	// renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
	//}
   }
   
   lr = luminance(gettile(x, y));
   
   if (lr > 0) {
	renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
   }
  }
 }
 
 offsetscreen(screen, 0, 0);
}

void sortandrender(int count, screen_t* screen) {
 int i;
 
 // TODO: sort sprites
 
 for (i = 0; i < count; i++) {
//  renderunit(&level->units[level->sprites[i] - 1], level, screen);
 }
}

void rendersprites(int xs, int ys, screen_t* screen) {
 int i, j, x, y, xo, yo, w, h;
 
 CHECKBOUNDLEVEL;
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 
 offsetscreen(screen, xs, ys);
 
// for (y = 0; y <= h + yo; y++) {
//  j = 0;
//  
//  for (x = 0; x <= w + xo; x++) {
//   CHECKTILEBOUNDS(x, y, continue);
//   
//   for (i = 0; i < MAX_TILEUNITS; i++) {
//	if (GETTILEUNIT(x, y, i)) {
//	 level->sprites[j] = GETTILEUNIT(x, y, i);
//	 j++;
//	}
//   }
//  }
//  
//  if (j > 0) {
//   sortandrender(j, screen);
//  }
// }
 
 offsetscreen(screen, 0, 0);
}

// ==================================================
// acting functions

void efface(refer_t unit) {
 unit_t* base;
 
 CHECKBOUNDLEVEL;
 CHECKUNIT(unit);
 
 base = &level->units[unit - 1].base;
 
 dirtychunk(base->x >> 4, base->y >> 4);
 
 removetileunit(unit, base->x >> 4, base->y >> 4);
 memset(base, 0, sizeof(unit_u));
 
 // TODO: send message
}

int getdata(int x, int y) {
 CHECKTILEBOUNDS(x, y, return 0);
 
 return level->tiles[x + y * level->w].data & 0xff;
}

int getdepth() {
 CHECKBOUNDLEVEL;
 
 return level->depth;
}

int gettile(int x, int y) {
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y, return 0);
 
 return level->tiles[x + y * level->w].id;
}

refer_t* getunits(aabb_t aabb) {
 int xt0, yt0, xt1, yt1;
 int i, j, x, y;
 
 xt0 = (aabb.x0 >> 4) - 1;
 xt1 = (aabb.x1 >> 4) - 1;
 yt0 = (aabb.y0 >> 4) + 1;
 yt1 = (aabb.y1 >> 4) + 1;
 
 j = 0;
 
 for (y = yt0; y <= yt1; y++) {
  for (x = xt0; x <= xt1; x++) {
   CHECKTILEBOUNDS(x, y, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	if (intersect(GETTILEUNIT(x, y, i), aabb)) {
	 samples[j] = GETTILEUNIT(x, y, i);
	 j++;
	}
   }
  }
 }
 
 samples[j] = NOUNIT;
 
 return samples;
}

int getunitid(refer_t unit) {
 
}

refer_t initiate(const char* word, int x, int y) {
 refer_t unit;
 
 unit = emptyunit();
 
 if (unit < 0) {
  return NOUNIT;
 }
 
 //createunit(&level->units[unit], word);
 
 level->units[unit].base.x = x;
 level->units[unit].base.y = y;
 
 return unit;
}

refer_t place(unit_u* unit, int x, int y) {
 int i;
 
 CHECKBOUNDLEVEL;
 
 i = 0;
 
 dirtychunk(x / 8 / CHUNKANGTH, y / 8 / CHUNKANGTH);
 
 return i;
}

refer_t seekbond(int bond) {
 int i;
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (level->units[i].base.bond == bond) {
   return i + 1;
  }
 }
 
 return 0;
}

refer_t* seekclass(const char* word, aabb_t aabb) {
 
 
 return 0;
}

void setdata(int x, int y, int data) {
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 dirtychunk(x, y);
 
 level->tiles[x + y * level->w].data = data;
}

void settile(int x, int y, int tile, int data) {
 CHECKBOUNDLEVEL;
 CHECKTILEBOUNDS(x, y);
 
 dirtychunk(x, y);
 
 level->tiles[x + y * level->w].id = tile;
 level->tiles[x + y * level->w].data = data;
}

int spawn(const char* word) {
 int i;//, x, y;
 
 for (i = 0; i < MAX_UNITS; i++) {
  
 }
 
 //level->units[i].extant = 1;
 
 //inserttileunit(x >> 4, y >> 4, i);
 
 return i + 1;
}

void spawnfiends(int count) {
 int i, max, min, toughness;
 
 CHECKBOUNDLEVEL;
 
 for (i = 0; i < count; i++) {
  max = min = 1;
  
  if (level->depth < 0) {
   max = -level->depth + 1;
  }
  else if (level->depth > 0) {
   min = max = 4;
  }
  
  toughness = randominteger(max - min + 1) + min;
  
  if (randominteger(1) == 0) {
   // spawn slime of level 'toughness'
  }
  else {
   // spawn zombie of level 'toughness'
  }
  
  //level->dirties[]
 }
}

refer_t summon(unit_u* unit) {
 return 0;
}

// ==================================================
// level functions

void bindlevel(level_t* bind) {
 level = bind;
}

void createlevel(int w, int h, int depth, level_t* parent) {
 CHECKBOUNDLEVEL;
 
 emptylevel(w, h, depth);
 
 generate(level);
 
 if (parent) {
  //matchstairs(parent);
 }
 
 if (depth == 1) {
  //initiate("fiend.AirWizard", w * 8, h * 8);
 }
}

void emptylevel(int w, int h, int depth) {
 CHECKBOUNDLEVEL;
 
 level->dirtcolor = 322;
 level->grasscolor = 141;
 level->sandcolor = 550;
 
 level->depth = depth;
 level->w = w;
 level->h = h;
 
 level->monsterdensity = 8;
 
 if (depth < 0) {
  level->dirtcolor = 222;
  level->monsterdensity = 4;
 }
 else if (depth > 0) {
  level->dirtcolor = 444;
  level->monsterdensity = 4;
 }
 
 level->dirties = calloc(w * h / (CHUNKANGTH * CHUNKANGTH) / BYTEWIDTH, sizeof(byte_t));
 level->tiles = malloc(sizeof(tile_t) * w * h);
 level->tileunits = calloc(w * h, sizeof(refer_t) * MAX_TILEUNITS);
}

void ticklevel() {
 unit_t* unit;
 int i, xt, yt, xto, yto;
 
 CHECKBOUNDLEVEL;
 
 spawnfiends(1);
 
 for (i = 0; i < level->w * level->h; i++) {
  xt = randominteger(level->w);
  yt = randominteger(level->h);
  
  ticktile(xt, yt);
 }
 
 for (i = 0; i < MAX_UNITS; i++) {
  unit = &level->units[i].base;
  
  xto = unit->x >> 4;
  yto = unit->y >> 4;
  
  tickunit(&level->units[i], level);
  
  if (!unit->extant) {
   efface(i + 1);
  }
  else {
   xt = unit->x >> 4;
   yt = unit->y >> 4;
   
   if (xto != xt || yto != yt) {
    removetileunit(xto, yto, i);
    inserttileunit(xt, yt, i);
   }
  }
  
  dirtychunk(xto, yto);
 }
 
 //if (hoststate() == HOST_LOCAL) {
  // TODO: sort and send dirty chunks
  // units must be messaged separately from chunks
 //}
}

int movestep(unit_t* unit, int xa, int ya) {
 //unit_t* other;
 int blocked, i;
 aabb_t to, t;
 
 int xt, yt;
 int xr, yr;
 
 if (xa != 0 && ya != 0) {
  LOGREPORT("units may only move along one axis at a time.");
  return 0;
 }
 
 //xr = unithalfwidth(unit->id);
 //yr = unithalfheight(unit->id);
 
 boundbox(&to, (unit->x - xr) >> 4, (unit->y - yr) >> 4, (unit->x + xr) >> 4, (unit->y + yr) >> 4);
 boundbox(&t, (unit->x + xa - xr) >> 4, (unit->y + ya - yr) >> 4, (unit->x + xa + xr) >> 4, (unit->y + ya + yr) >> 4);
 
 blocked = 0;
 
 for (yt = t.y0; yt <= t.y1; yt++) {
  for (xt = t.x0; xt <= t.x1; xt++) {
   if (xt >= to.x0 && xt <= to.x1 && yt >= to.y0 && yt <= to.y1) {
	continue;
   }
   
   //gettile(xt, yt);
   
   if (!surpassable(gettile(xt, yt))) {
	blocked = 1;
	return 0;
   }
  }
 }
 
 if (blocked) {
  return 0;
 }
 
 for (i = 0; i < 0; i++) {
  
  
 }
 
 unit->x += xa;
 unit->y += ya;
 
 return 1;
}

int moveunit(unit_t* unit, int xa, int ya) {
 int stopped, xt, yt;
 
 if (xa != 0 || ya != 0) {
  stopped = 1;
  
  if (xa != 0 && movestep(unit, xa, 0)) {
   stopped = 0;
  }
  
  if (ya != 0 && movestep(unit, 0, ya)) {
   stopped = 0;
  }
  
  if (!stopped) {
   xt = unit->x >> 4;
   yt = unit->y >> 4;
   
   //tilestep(xt, yt, unit->id);
  }
  
  return !stopped;
 }
 
 return 1;
}

int movemob(mob_t* mob, int xa, int ya) {
// if (inliquid((unit_u*) mob) && mob->swim++ % 2 == 0) {
//  return 1;
// }
 
 if (mob->xk < 0) {
  movestep((unit_t*) mob, -1, 0);
  mob->xk++;
 }
 if (mob->xk > 0) {
  movestep((unit_t*) mob, 1, 0);
  mob->xk--;
 }
 
 if (mob->yk < 0) {
  movestep((unit_t*) mob, 0, -1);
  mob->yk++;
 }
 if (mob->yk > 0) {
  movestep((unit_t*) mob, 0, 1);
  mob->yk--;
 }
 
 if (mob->hurt > 0) {
  return 1;
 }
 
 if (xa != 0 || ya != 0) {
  mob->walk++;
  
  if (xa < 0) {
   mob->dir = 2;
  }
  else if (xa > 0) {
   mob->dir = 3;
  }
  
  if (ya < 0) {
   mob->dir = 1;
  }
  else if (ya > 0) {
   mob->dir = 0;
  }
 }
 
 return moveunit((unit_t*) mob, xa, ya);
}
