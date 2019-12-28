#include "levelaid.h"

// ==================================================
// variables

level_t* level;
refer_t samples[MAX_SAMPLES];

// ==================================================
// private functions

// Encircles the specified location with the designated tile id.
void encircle(int x, int y, int tile) {
 CHECKTILEBOUNDS(x, y, level, return);
 
 settile(x - 1, y, tile, 0);
 settile(x + 1, y, tile, 0);
 settile(x, y - 1, tile, 0);
 settile(x, y + 1, tile, 0);
 settile(x - 1, y - 1, tile, 0);
 settile(x - 1, y + 1, tile, 0);
 settile(x + 1, y - 1, tile, 0);
 settile(x + 1, y + 1, tile, 0);
}

// Matches stairs with the parent level of the provided level and inserts proper radius
// about the tile.
void matchstairs(level_t* parent) {
 int x, y;
 
 if (!parent) {
  LOGREPORT("received null parent level.");
  return;
 }
 
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

// ==================================================
// level functions

void bindlevel(level_t* bind) {
 level = bind;
}

void createlevel(int w, int h, int depth, level_t* parent) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 if (w < 1 || h < 1) {
  LOGREPORT("received invalid level bounds.");
  return;
 }
 
 emptylevel(w, h, depth);
 
 generate(level);
 
 if (parent) {
  matchstairs(parent);
 }
 
 if (depth == 1) {
  initiate("fiend.AirWizard", w * TILESCALE, h * TILESCALE);
 }
}

// Returns an unique unit id to the level bound; two levels may have units with the same id,
// but one level may not have two units alike. In this case, one unit's id must be changed prior
// to entry and ownerships must be investigated in the next tick to remove lingering mastered
// bonds.
refer_t createunitid() {
 int id;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 do {
  id = randomid();
 }
 while (hasunit(id));
 
 return id;
}

refer_t dropitem(refer_t item, int x, int y) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 // TODO: implement drop item function.
 
 return NOUNIT;
}

// The unit is not erased until it has been handled in the updatelevel function
// a refrech will reseize its memory.
void efface(refer_t id) {
 unit_t* unit;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 unit = (unit_t*) getunit(id);
 
 if (!unit) {
  LOGREPORT("unable to conjure unit for removal.");
  return;
 }
 
 removetileunit(id, unit->x >> 4, unit->y >> 4, level);
}

void emptylevel(int w, int h, int depth) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 if (w < 1 || h < 1) {
  LOGREPORT("received invalid level bounds.");
  return;
 }
 
 level->dirtcolor = 322;
 level->grasscolor = 141;
 level->sandcolor = 550;
 
 level->depth = depth;
 level->w = w;
 level->h = h;
 
 level->density = 8;
 
 if (depth < 0) {
  level->dirtcolor = 222;
  level->density = 4;
 }
 else if (depth > 0) {
  level->dirtcolor = 444;
  level->density = 4;
 }
 
 level->dirties = calloc(w * h / (CHUNKANGTH * CHUNKANGTH) / BYTEWIDTH, sizeof(byte_t));
 level->tiles = malloc(sizeof(tile_t) * w * h);
 level->tileunits = calloc(w * h, sizeof(refer_t) * MAX_TILEUNITS);
 
 if (!level->dirties || !level->tiles || !level->tileunits) {
  LOGREPORT("unable to allocate memory for level chunk data.");
  exit(EXIT_FAILURE);
 }
}

int getdata(int x, int y) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 CHECKTILEBOUNDS(x, y, level, return 0);
 
 return level->tiles[x + y * level->w].data & BYTEMASK;
}

int gettile(int x, int y) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 CHECKTILEBOUNDS(x, y, level, return NOTILE);
 
 return level->tiles[x + y * level->w].id;
}

unit_u* getunit(refer_t id) {
 int unit;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 unit = seekunit(id, level);
 
 if (unit == INVALIDUNIT) {
  LOGREPORT("unable to locate unit '%x'.", id);
  return NULL;
 }
 
 return &level->units[unit];
}

refer_t* getunits(aabb_t aabb) {
 int xt0, yt0, xt1, yt1;
 int i, j, x, y;
 int unit;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 xt0 = (aabb.x0 >> 4) - 1;
 yt0 = (aabb.y0 >> 4) - 1;
 xt1 = (aabb.x1 >> 4) + 1;
 yt1 = (aabb.y1 >> 4) + 1;
 
 j = 0;
 
 for (y = yt0; y <= yt1; y++) {
  for (x = xt0; x <= xt1; x++) {
   CHECKTILEBOUNDS(x, y, level, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	unit = GETTILEUNIT(i, x, y, level);
	
	if (unit != NOUNIT && getunit(unit)) {
	 samples[j] = unit;
	 j++;
	}
   }
  }
 }
 
 samples[j] = NOUNIT;
 
 return samples;
}

int hasunit(refer_t id) {
 int i;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (GETUNIT(i, level).id == id) {
   return 1;
  }
 }
 
 return 0;
}

// Spawns an unit from the specified unit word and places it in the level at specified
// coordinates. The chunk is automatically dirtied and the unit is appropriately initialized.
refer_t initiate(const char* word, int x, int y) {
 int unit;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 unit = begetunit(word, level);
 
 if (unit == INVALIDUNIT) {
  LOGREPORT("unable to beget unit '%s'.", word);
  return NOUNIT;
 }
 
 GETUNIT(unit, level).x = x;
 GETUNIT(unit, level).y = y;
 
 inserttileunit(GETUNIT(unit, level).id, x >> 4, y >> 4, level);
 
 LOGREPORT("spawned unit '%s' with id '%x' at [%i, %i].", word, GETUNIT(unit, level).id, GETUNIT(unit, level).x, GETUNIT(unit, level).y);
 
 return GETUNIT(unit, level).id;
}

refer_t place(unit_u* unit) {
 unit_u* held;
 int slot;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 if (!unit) {
  LOGREPORT("received invalid unit.");
  return NOUNIT;
 }
 
 held = getunit(unit->base.id);
 
 if (!held) {
  slot = emptyunit(level);
  
  if (slot == INVALIDUNIT) {
   LOGREPORT("unable to find free slot for unit [%x].", unit->base.id);
   return NOUNIT;
  }
  
  held = (unit_u*) &GETUNIT(slot, level);
  
  if (!held) {
   LOGREPORT("unable to place unit [%x].", unit->base.id);
   return NOUNIT;
  }
 }
 
 removetileunit(held->base.id, held->base.x >> 4, held->base.y >> 4, level);
 
 memcpy(held, unit, sizeof(unit_u));
 
 inserttileunit(held->base.id, held->base.x >> 4, held->base.y >> 4, level);
 
 LOGDEBUG("placed unit [%x] at [%i, %i].", held->base.id, held->base.x, held->base.y);
 
 return held->base.id;
}

// The function must handle instances of duplicate ids in the target level, therefore it shall change
// unit ids appropriately and return the resulting unit's id.
refer_t relay(refer_t id, level_t* other) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 // TODO: implement relay unit function.
 
 return 0;
}

void renderbackground(int xs, int ys, screen_t* screen) {
 int x, y, xo, yo, w, h;
 
 if (!screen) {
  LOGREPORT("received invalid screen.");
  return;
 }
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
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
   rendertile(x, y, screen);
  }
 }
 
 for (y = yo; y <= h + yo; y++) {
  for (x = xo; x <= w + xo; x++) {
   if (x % CHUNKANGTH == 0 && y % CHUNKANGTH == 0) {
	char chars[16];
	
	sprintf(chars, "%i,%i", x / CHUNKANGTH, y / CHUNKANGTH);
	
	renderfont(chars, x * 16, y * 16, getcolor(0, 555, 555, 555), screen);
   }
  }
 }
  
 offsetscreen(screen, 0, 0);
}

void renderlights(int xs, int ys, screen_t* screen) {
 int i, lr, x, y, xo, yo, w, h, r;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 r = 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo - r; y <= h + yo + r; y++) {
  for (x = xo - r; x <= w + xo + r; x++) {
   CHECKTILEBOUNDS(x, y, level, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	lr = getlighting(GETTILEUNIT(x, y, i, level));
	
	if (lr > 0) {
	 renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
	}
   }
   
   lr = gettileglow(x, y);
   
   if (lr > 0) {
	renderlight(x * 16 + 8, y * 16 + 8, lr * 8, screen);
   }
  }
 }
 
 offsetscreen(screen, 0, 0);
}

void rendersprites(int xs, int ys, screen_t* screen) {
 int i, j, x, y, xo, yo, w, h;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 xo = xs >> 4;
 yo = ys >> 4;
 w = (screen->w + 15) >> 4;
 h = (screen->h + 15) >> 4;
 
 offsetscreen(screen, xs, ys);
 
 for (y = yo; y <= h + yo; y++) {
  j = 0;
  
  for (x = xo; x <= w + xo; x++) {
   CHECKTILEBOUNDS(x, y, level, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	if (GETTILEUNIT(i, x, y, level) != NOUNIT) {
	 samples[j] = GETTILEUNIT(i, x, y, level);
	 j++;
	}
   }
  }
  
  if (j > 0) {
   sortandrender(samples, j, level, screen);
  }
 }
 
 offsetscreen(screen, 0, 0);
}

// The function returns all entity ids within the tile coordinates of the
// bounding box; the ids are stacked and end at any NOUNIT reference met
// from the reference index onwards.
refer_t* seekunits(const char* word, aabb_t aabb) {
 unit_t* unit;
 unitword_t* unitword;
 int i, j, tileunit, x, y;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 unitword = getunitbyword(word);
 
 if (!unitword) {
  LOGREPORT("received invalid search word.");
  return NULL;
 }
 
 binddomain(&aabb, 0, 0, level->w, level->h);
 ensuredomain(&aabb);
 
 j = 0;
 
 for (y = aabb.y0; y <= aabb.y1; y++) {
  for (x = aabb.x0; x <= aabb.x1; x++) {
   CHECKTILEBOUNDS(x, y, level, continue);
   
   for (i = 0; i < MAX_TILEUNITS; i++) {
	tileunit = GETTILEUNIT(i, x, y, level);
	
	if (tileunit != NOUNIT) {
	 unit = (unit_t*) getunit(tileunit);
	 
	 if (unit && (unit->word == unitword->id)) {
	  samples[j] = unit->id;
	  
	  j++;
	 }
	}
   }
  }
 }
 
 samples[j] = NOUNIT;
 
 return samples;
}

void setdata(int x, int y, int data) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 CHECKTILEBOUNDS(x, y, level, return);
 
 level->tiles[x + y * level->w].data = data;
 
 dirtychunk(x / CHUNKANGTH, y / CHUNKANGTH, level);
}

void settile(int x, int y, int tile, int data) {
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 CHECKTILEBOUNDS(x, y, level, return);
 
 level->tiles[x + y * level->w].id = tile;
 level->tiles[x + y * level->w].data = data;
 
 dirtychunk(x / CHUNKANGTH, y / CHUNKANGTH, level);
}

// Spawns from the specified unit word and places it in the world according to its spawn;
// units not of the mob class are spawned randomly as they do not have the ability to spawn.
refer_t spawn(const char* word) {
 int unit;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 unit = begetunit(word, level);
 
 if (unit == INVALIDUNIT) {
  LOGREPORT("unable to beget unit '%s'.", word);
  return NOUNIT;
 }
 
 if (isclass(SUPER_MOB, GETUNIT(unit, level).word)) {
  findspawn(GETUNIT(unit, level).id);
 }
 else {
  GETUNIT(unit, level).x = randominteger(level->w) * TILESCALE;
  GETUNIT(unit, level).y = randominteger(level->h) * TILESCALE;
 }
 
 inserttileunit(GETUNIT(unit, level).id, GETUNIT(unit, level).x >> 4, GETUNIT(unit,level).y >> 4, level);
 
 LOGREPORT("spawned unit '%s' with id '%x' at [%i, %i].", word, GETUNIT(unit, level).id, GETUNIT(unit, level).x, GETUNIT(unit, level).y);
 
 return GETUNIT(unit, level).id;
}

// FIXME: fix code
void boundunit(refer_t id, int angth, aabb_t* aabb) {
 unit_t* unit;
 int x, y;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 if (!id || angth < 0 || !aabb) {
  LOGREPORT("received invalid arguments.");
  return;
 }
 
 unit = &getunit(id)->base;
 
 if (!unit) {
  LOGREPORT("unable to bound unit [%x].", id);
  return;
 }
 
 x = unit->x >> 4;
 y = unit->y >> 4;
 
 boundbox(aabb, x - angth, y - angth, x + angth, y + angth);
 
 binddomain(aabb, 0, 0, level->w, level->h);
 
 ensuredomain(aabb);
 
 return;
}

void ticklevel() {
 unit_t* unit;
 int i, xt, yt, xto, yto;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 // TODO: spawn level fiends
 //spawnfiends(1, level);
 
 // TODO: hasten the tick tiles function
 for (i = 0; i < level->w * level->h; i++) {
  xt = randominteger(level->w);
  yt = randominteger(level->h);
  
  //ticktile(xt, yt);
 }
 
 for (i = 0; i < MAX_UNITS; i++) {
  unit = &GETUNIT(i, level);
  
  if (unit->id) {
   xto = unit->x >> 4;
   yto = unit->y >> 4;
   
   if (unit->extant) {
	tickunit(unit->id);
	
	xt = unit->x >> 4;
	yt = unit->y >> 4;
	
	if (xto != xt || yto != yt) {
	 removetileunit(unit->id, xto, yto, level);
	 inserttileunit(unit->id, xt, yt, level);
	}
   }
   else {
	efface(unit->id);
   }
   
   dirtychunk(xto, yto, level);
  }
 }
}

void tickunits(aabb_t aabb) {
 refer_t* units;
 unit_t* unit;
 int i, xt, yt, xto, yto;
 
 CHECKLEVEL(level, exit(EXIT_FAILURE));
 
 units = getunits(aabb);
 
 for (i = 0; i < MAX_SAMPLES && units[i] != NOUNIT; i++) {
  unit = &getunit(units[i])->base;
  
  if (!unit) {
   continue;
  }
  
  xto = unit->x >> 4;
  yto = unit->y >> 4;
  
  if (GETUNIT(units[i], level).extant) {
   tickunit(unit->id);
   
   xt = unit->x >> 4;
   yt = unit->y >> 4;
   
   if (xto != xt || yto != yt) {
	removetileunit(unit->id, xto, yto, level);
	inserttileunit(unit->id, xt, yt, level);
   }
  }
  else {
   efface(unit->id);
  }
 }
}
