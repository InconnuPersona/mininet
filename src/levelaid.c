#include "levelaid.h"

// ==================================================
// variables

level_t* source;

// ==================================================
// level functions

// Find and allocate an unit slot to accompany the worded unit;
// the index of fetched slot is returned by the function.
int begetunit(const char* word, level_t* level) {
 int unit;
 
 CHECKLEVEL(level, return INVALIDUNIT);
 
 if (!word) {
  LOGREPORT("received null unit word.");
  return INVALIDUNIT;
 }
 
 unit = emptyunit(level);
 
 if (unit == INVALIDUNIT) {
  LOGREPORT("unable to reserve unit space for '%s'.", word);
  return INVALIDUNIT;
 }
 
 createunit(&GETUNIT(unit, level), word);
 
 return unit;
}

// Cleans the level dirties to allow the next update sequence a proper update procedure.
void cleanlevel(level_t* level) {
 // TODO: implement level cleaning
}

int isdirtychunk(int x, int y, level_t* level) {
 int byte, index;
 
 CHECKLEVEL(level, return 0);
 
 CHECKTILEBOUNDS(x, y, level, return 0);
 
 index = x / CHUNKANGTH + y * level->w / (CHUNKANGTH * CHUNKANGTH);
 
 byte = index / BYTEWIDTH;
 byte = index - (byte * BYTEWIDTH);
 byte = 1 << byte;
 
 return level->dirties[index / BYTEWIDTH] & byte;
}

// Coordinates inputted are tile coordinates which are mapped to chunk coordinates.
// When in a multiplayer game, dirty chunks are submitted to each player when in range
// of the player entity alongside sounds and unit updates.
void dirtychunk(int x, int y, level_t* level) {
 int byte, index;
 
 CHECKLEVEL(level, return);
 
 CHECKTILEBOUNDS(x, y, level, return);
 
 index = x / CHUNKANGTH + y * level->w / (CHUNKANGTH * CHUNKANGTH);
 
 byte = index / BYTEWIDTH;
 byte = index - (byte * BYTEWIDTH);
 byte = 1 << byte;
 
 level->dirties[index / BYTEWIDTH] |= byte;
 
 return;
}

// Returns the first available index where a unit was successfully deallocated.
int emptyunit(level_t* level) {
 int i;
 
 CHECKLEVEL(level, return NOUNIT);
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (!level->units[i].base.word) {
   return i;
  }
 }
 
 LOGREPORT("unable to find free unit slot.");
 
 return INVALIDUNIT;
}

// Inserts a tile unit at the specified tile coordinate; tile units are sorted for rendering,
// therefore a failure to insert the tile only results in an invisible entity.
void inserttileunit(refer_t unit, int x, int y, level_t* level) {
 int i, tile;
 
 CHECKLEVEL(level, return);
 CHECKTILEBOUNDS(x, y, level, return);
 
 tile = (x + y * level->w) * MAX_TILEUNITS;
 
 for (i = tile; i < tile + MAX_TILEUNITS; i++) {
  if (!level->tileunits[i]) {
   level->tileunits[i] = unit;
   return;
  }
  else if (level->tileunits[i] == unit) {
   return;
  }
 }
 
 LOGDEBUG(1, "overflowed tile unit insert location.");
 
 return;
}

// Returns whether the unit at specified index intersects the bounding box received.
int intersect(int unit, aabb_t bb, level_t* level) {
 int x, xr, y, yr;
 
 CHECKLEVEL(level, return 0);
 
 if (unit < 0 || unit >= MAX_UNITS) {
  LOGREPORT("received out of bounds unit.");
  return 0;
 }
 
 x = GETUNIT(unit, level).x;
 y = GETUNIT(unit, level).y;
 
 xr = getunitword(GETUNIT(unit, level).word)->halfwidth;
 yr = getunitword(GETUNIT(unit, level).word)->halfheight;
 
 return !(x + xr < bb.x0 || y + yr < bb.y0 || x - xr > bb.x1 || y - yr > bb.y1);
}

// Loads the specified chunk for transmission or relocation.
int readchunk(int x, int y, chunk_t* chunk, level_t* level) {
 int xt, yt;
 
 CHECKLEVEL(level, return 0);
 CHECKTILEBOUNDS(x * CHUNKANGTH, y * CHUNKANGTH, level, return 0);
 
 if (!chunk) {
  LOGREPORT("received invalid chunk.");
  return 0;
 }
 
 chunk->x = x;
 chunk->y = y;
 
 for (yt = 0; yt < CHUNKANGTH; yt++) {
  if (yt < 0 || yt >= level->h) {
   continue;
  }
  
  for (xt = 0; xt < CHUNKANGTH; xt++) {
   if (xt < 0 || xt >= level->w) {
	continue;
   }
   
   memcpy(&chunk->tiles[xt + yt * CHUNKANGTH], &level->tiles[xt + x * CHUNKANGTH + (yt + y * CHUNKANGTH) * level->w], sizeof(tile_t));
  }
 }
 
 return 1;
}

// Removes the tile unit from the tile unit slots; there is no error if it is not found.
void removetileunit(refer_t unit, int x, int y, level_t* level) {
 int i, tile;
 
 CHECKLEVEL(level, return);
 CHECKTILEBOUNDS(x, y, level, return);
 
 tile = (x + y * level->w) * MAX_TILEUNITS;
 
 for (i = tile; i < tile + MAX_TILEUNITS; i++) {
  if (level->tileunits[i] == unit) {
   level->tileunits[i] = NOUNIT;
   return;
  }
 }
}

// Returns the unit specified by id from the level; null is returned if unfound.
int seekunit(refer_t id, level_t* level) {
 int i;
 
 CHECKLEVEL(level, return INVALIDUNIT);
 
 if (id == NOUNIT) {
  return INVALIDUNIT;
 }
 
 for (i = 0; i < MAX_UNITS; i++) {
  if (GETUNIT(i, level).id == id) {
   return i;
  }
 }
 
 return INVALIDUNIT;
}

// Updates chunk tiles to received chunk data.
void setchunk(chunk_t* chunk, level_t* level) {
 int xt, yt;
 
 CHECKLEVEL(level, return);
 CHECKTILEBOUNDS(chunk->x * CHUNKANGTH, chunk->y * CHUNKANGTH, level, return);
 
 if (!chunk) {
  LOGREPORT("received invalid chunk.");
  return;
 }
 
 for (yt = 0; yt < CHUNKANGTH; yt++) {
  if (yt < 0 || yt >= level->h) {
   continue;
  }
  
  for (xt = 0; xt < CHUNKANGTH; xt++) {
   if (xt < 0 || xt >= level->w) {
	continue;
   }
   
   memcpy(&level->tiles[xt + chunk->x * CHUNKANGTH + (yt + chunk->y * CHUNKANGTH) * level->w], &chunk->tiles[xt + yt * CHUNKANGTH], sizeof(tile_t));
  }
 }
 
 return;
}

// Sorts through the tile units and renders them on the screen according to position.
void sortandrender(refer_t* units, int count, level_t* level, screen_t* screen) {
 int i;
 
 CHECKLEVEL(level, return);
 
 if (!units || !screen || count < 0) {
  LOGREPORT("received invalid parameters.");
  return;
 }
 
 source = level;
 
 qsort(units, count, sizeof(refer_t), spritesort);
 
 for (i = 0; i < count; i++) {
  renderunit(units[i], screen);
 }
}

// Spawns counted fiends in the level according to its depth and random nature.
// Fetment is a standword for the mob's difficulty.
void spawnfiends(int count, level_t* level) {
 unit_u* unit;
 refer_t id;
 int i, max, min, fetment;
 
 CHECKLEVEL(level, return);
 
 for (i = 0; i < count; i++) {
  max = min = 1;
  
  if (level->depth < 0) {
   max = -level->depth + 1;
  }
  else if (level->depth > 0) {
   min = max = 4;
  }
  
  fetment = randominteger(max - min + 1) + min;
  
  if (randominteger(1) == 0) {
   id = spawn("fiend.Slime");
  }
  else {
   id = spawn("fiend.Zombie");
  }
  
  if (id == NOUNIT) {
   continue;
  }
  
  unit = getunit(id);
  
  if (!unit) {
   LOGREPORT("unable to set fiend fetment.");
   continue;
  }
  
  unit->fiend.level = fetment;
 }
}

// An utility function to sort through sprites and return an adjusting value
// ordering them from upmost rendered first and bottommost rendered last; this
// overlays units so that ones lower on the screen are rendered ontop of others.
int spritesort(const void* v1, const void* v2) {
 int u1, u2;
 
 u1 = seekunit((refer_t) v1, source);
 u2 = seekunit((refer_t) v2, source);
 
 if (GETUNIT(u2, source).y < GETUNIT(u1, source).y) {
  return 1;
 }
 
 if (GETUNIT(u2, source).y > GETUNIT(u1, source).y) {
  return -1;
 }
 
 return 0;
}
