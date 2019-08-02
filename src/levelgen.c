#include "main.h"

#include <math.h>

// ==================================================
// structures

typedef struct {
 int w, h;
 float* values;
} map_t;

// ==================================================
// map functions

float sample(map_t* map, int x, int y) {
 if (!map) {
  LOGREPORT("attempted an invalid sample.");
  return 0;
 }
 
 return map->values[(x & (map->w - 1)) + (y & (map->h - 1)) * map->w];
}

void setsample(map_t* map, int x, int y, float value) {
 if (!map) {
  LOGREPORT("attempted to set an invalid sample.");
  return;
 }
 
 map->values[(x & (map->w - 1)) + (y & (map->h - 1)) * map->w] = value;
 
 return;
}

void setupmap(map_t* map, int w, int h, int stepsize) {
 float a, b, c, d, e, f, g, H, modifier, scale;
 int halfstep, x, y;
 
 if (!map || w < 1 || h < 1) {
  LOGREPORT("attempted invalid map creation.");
  return;
 }
 
 map->w = w;
 map->h = h;
 
 map->values = malloc(sizeof(float) * w * h);
 
 for (y = 0; y < w; y += stepsize) {
  for (x = 0; x < h; x += stepsize) {
   setsample(map, x, y, randomfloat() * 2.f - 1.f);
  }
 }
 
 scale = 1.f / w;
 modifier = 1.f;
 
 do {
  halfstep = stepsize / 2.f;
  
  for (y = 0; y < h; y += stepsize) {
   for (x = 0; x < w; x += stepsize) {
	a = sample(map, x, y);
	b = sample(map, x + stepsize, y);
	c = sample(map, x, y + stepsize);
	d = sample(map, x + stepsize, y + stepsize);
	
	e = (a + b + c + d) / 4.f + (randomfloat() * 2.f - 1.f) * stepsize * scale;
	
	setsample(map, x + halfstep, y + halfstep, e);
   }
  }
  
  for (y = 0; y < h; y += stepsize) {
   for (x = 0; x < w; x += stepsize) {
	a = sample(map, x, y);
	b = sample(map, x + stepsize, y);
	c = sample(map, x, y + stepsize);
	
	d = sample(map, x + halfstep, y + halfstep);
	e = sample(map, x + halfstep, y - halfstep);
	f = sample(map, x - halfstep, y + halfstep);
	
	H = (a + b + d + e) / 4.f + (randomfloat() * 2.f - 1.f) * stepsize * scale * 0.5f;
	g = (a + c + d + f) / 4.f + (randomfloat() * 2.f - 1.f) * stepsize * scale * 0.5f;
	
	setsample(map, x + halfstep, y, H);
	setsample(map, x, y + halfstep, g);
   }
  }
  
  stepsize /= 2.f;
  scale *= (modifier + 0.8f);
  modifier *= 0.3f;
 }
 while (stepsize > 1.f);
}

void unloadmap(map_t* map) {
 if (!map) {
  LOGREPORT("received invalid argument.");
  return;
 }
 
 free(map->values);
 
 return;
}

// ==================================================
// generation functions

void generatedeserts(level_t* level) {
 int h, i, j, k, w, x, y, xo, yo, xs, ys, xx, yy;
 
 w = level->w;
 h = level->h;
 
 for (i = 0; i < w * h / 2800; i++) {
  xs = randominteger(w);
  ys = randominteger(h);
  
  for (k = 0; k < 10; k++) {
   x = xs + randominteger(21) - 10;
   y = ys + randominteger(21) - 10;
   
   for (j = 0; j < 100; j++) {
	xo = x + randominteger(5) - randominteger(5);
	yo = y + randominteger(5) - randominteger(5);
	
	for (yy = yo - 1; yy <= yo + 1; yy++) {
	 for (xx = xo - 1; xx <= xo + 1; xx++) {
	  if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
	   if (level->tiles[xx + yy * w].id == tileid("grass")) {
//		level->tiles[xx + yy * w].id = tileid("sand");
	   }
	  }
	 }
	}
   }
  }
 }
 
 for (i = 0; i < w * h / 100; i++) {
  xx = randominteger(w);
  yy = randominteger(h);
  
  if (xx >= 0 && y >= 0 && xx < w && yy < h) {
   if (level->tiles[xx + yy * w].id == tileid("sand")) {
	level->tiles[xx + yy * w].id = tileid("cactus");
   }
  }
 }
 
 return;
}

void generatefoliage(level_t* level) {
 int color, h, i, j, w, x, xx, y, yy;
 
 h = level->h;
 w = level->w;
 
 // generate forests
 for (i = 0; i < w * h / 400; i++) {
  x = randominteger(w);
  y = randominteger(h);
  
  for (j = 0; j < 200; j++) {
   xx = x + randominteger(15) - randominteger(15);
   yy = y + randominteger(15) - randominteger(15);
   
   if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
	if (level->tiles[xx + yy * w].id == tileid("grass")) {
	 level->tiles[xx + yy * w].id = tileid("tree");
	}
   }
  }
 }
 
 for (i = 0; i < w * h / 400; i++) {
  x = randominteger(w);
  y = randominteger(h);
  
  color = randominteger(4);
  
  for (j = 0; j < 30; j++) {
   xx = x + randominteger(5) - randominteger(5);
   yy = y + randominteger(5) - randominteger(5);
   
   if (xx >= 0 && yy >= 0 && xx < w && yy < h) {
	if (level->tiles[xx + yy * w].id == tileid("grass")) {
	 level->tiles[xx + yy * w].id = tileid("flower");
	 level->tiles[xx + yy * w].data = (byte_t) (color + randominteger(4) * 16);
	}
   }
  }
 }
 
 return;
}

void generatetopmap(level_t* level) {
 map_t mnoise1, mnoise2, mnoise3, noise1, noise2;
 int count, h, i, w, x, xx, y, yy;
 float value, mvalue, xd, yd, distance;
 
 if (!level) {
  LOGREPORT("received invalid level.");
  return;
 }
 
 h = level->h;
 w = level->w;
 
 setupmap(&mnoise1, w, h, 16);
 setupmap(&mnoise2, w, h, 16);
 setupmap(&mnoise3, w, h, 16);
 
 setupmap(&noise1, w, h, 32);
 setupmap(&noise2, w, h, 32);
 
 for (y = 0; y < h; y++) {
  for (x = 0; x < w; x++) {
   i = x + y * w;
   
   value = fabs(noise1.values[i] - noise2.values[i]) * 3.f - 2.f;
   mvalue = fabs(mnoise1.values[i] - mnoise2.values[i]);
   mvalue = fabs(mvalue - mnoise3.values[i]) * 3.f - 2.f;
   
   xd = x / (w - 1.f) * 2.f - 1.f;
   yd = y / (h - 1.f) * 2.f - 1.f;
   
   if (xd < 0) {
	xd = -xd;
   }
   
   if (yd < 0) {
	yd = -yd;
   }
   
   distance = (xd >= yd) ? xd : yd;
   
   distance = distance * distance * distance * distance;
   distance = distance * distance * distance * distance;
   
   value = value + 1.f - distance * 20.f;
   
   if (value < -0.5f) {
	level->tiles[i].id = tileid("water");
   }
   else if (value > 0.5f && mvalue < -1.5f) {
	level->tiles[i].id = tileid("rock");
   }
   else {
	level->tiles[i].id = tileid("grass");
   }
  }
 }
 
 generatedeserts(level);
 
 generatefoliage(level);
 
 {
  count = 0;
  
 generatetopmap_stairloop:
  for (i = 0; i < w * h / 100; i++) {
   x = randominteger(w - 2) + 1;
   y = randominteger(h - 2) + 1;
   
   for (yy = y - 1; yy <= y + 1; yy++) {
	for (xx = x - 1; xx <= x + 1; xx++) {
	 if (level->tiles[xx + yy * w].id != tileid("rock")) {
	  goto generatetopmap_stairloop;
	 }
	}
   }
   
   level->tiles[x + y * w].id = tileid("stairsDown");
   
   count++;
   
   if (count == 4) {
    break;
   }
  }
 }
 
 unloadmap(&mnoise1);
 unloadmap(&mnoise2);
 unloadmap(&mnoise3);
 
 unloadmap(&noise1);
 unloadmap(&noise2);
 
 return;
}

void generateskymap(level_t* level) {
 int i;
 
 for (i = 0; i < level->w * level->h; i++) {
  level->tiles[i].id = tileid("dirt");
 }
}

void generateunderground(level_t* level) {

 int i;
 
 for (i = 0; i < level->w * level->h; i++) {
  level->tiles[i].id = tileid("dirt");
 }
 /*
 map_t mnoise1, mnoise2, mnoise3;
 map_t nnoise1, nnoise2, nnoise3;
 map_t wnoise1, wnoise2, wnoise3;
 map_t noise1, noise2;
 tile_t* tiles;
 double distance, value, mvalue, nvalue, wvalue, xd, yd;
 int count, h, i, w, x, y, xx, yy;
 
 if (!level) {
  LOGREPORT("attempted to generate an unbound level.");
  return;
 }
 
 h = level->h;
 tiles = level->tiles;
 w = level->w;
 
 setupmap(&mnoise1, w, h, 16);
 setupmap(&mnoise2, w, h, 16);
 setupmap(&mnoise3, w, h, 16);
 
 setupmap(&nnoise1, w, h, 16);
 setupmap(&nnoise2, w, h, 16);
 setupmap(&nnoise3, w, h, 16);
 
 setupmap(&wnoise1, w, h, 16);
 setupmap(&wnoise2, w, h, 16);
 setupmap(&wnoise3, w, h, 16);
 
 setupmap(&noise1, w, h, 32);
 setupmap(&noise2, w, h, 32);
 
 for (y = 0; y < h; y++) {
  for (x = 0; x < w; x++) {
   i = x + y * w;
   
   
   value = abs(noise1.values[i] - noise2.values[i]) * 3 - 2;
   
   mvalue = abs(mnoise1.values[i] - mnoise2.values[i]);
   mvalue = abs(mvalue - mnoise3.values[i]) * 3 - 2;
   
   nvalue = abs(nnoise1.values[i] - nnoise2.values[i]);
   nvalue = abs(nvalue - nnoise3.values[i]) * 3 - 2;
   
   wvalue = abs(wnoise1.values[i] - nnoise2.values[i]);
   wvalue = abs(wvalue - wnoise3.values[i]) * 3 - 2;
   
   xd = x / (w - 1.0) * 2 - 1;
   yd = y / (h - 1.0) * 2 - 1;
   
   if (xd < 0) {
	xd = -xd;
   }
   
   if (yd < 0) {
	yd = -yd;
   }
   
   distance = xd >= yd ? xd : yd;
   
   distance = distance * distance * distance * distance;
   distance = distance * distance * distance * distance;
   
   value = value + 1 - distance * 20;
   
   if (value > -2 && wvalue < -2.0 + level->depth / 2 * 3) {
	
   }
  }
 }
 
 if (level->depth < 3) {
  count = 0;
  
 stairloop:
  for (i = 0; i < w * h / 100; i++) {
   x = randominteger(w - 20) + 10;
   y = randominteger(w - 20) + 10;
   
   for (yy = y - 1; yy <= y + 1; yy++) {
	
   }
  }
 }*/
}

int validatetopmap(level_t* level) {
 int count[MAX_TILEWORDS], i;
 
 if (!level) {
  LOGREPORT("attempted invalid map validation.");
  return 0;
 }
 
 for (i = 0; i < level->w * level->h; i++) {
  count[level->tiles[i].id & 0xff]++;
 }
 
 if (count[tileid("rock") & BYTEMASK] < 100) return 0;
 //if (count[tileid("sand") & BYTEMASK] < 100) return 0;
 if (count[tileid("grass") & BYTEMASK] < 100) return 0;
 //if (count[tileid("tree") & BYTEMASK] < 100) return 0;
 //if (count[tileid("stairsDown") & BYTEMASK] < 2) return 0;
 
 return 1;
}

int validateskymap(level_t* level) {
 int count[MAX_TILEWORDS], i;
 
 for (i = 0; i < level->w * level->h; i++) {
  count[level->tiles[i].id & 0xff]++;
 }
 
 //if (count[tileid("cloud") & BYTEMASK] < 100) return 0;
 //if (count[tileid("stairsDown") & BYTEMASK] < 2) return 0;
 
 return 1;
}

int validateunderground(level_t* level) {
 int count[MAX_TILEWORDS], i;
 
 for (i = 0; i < level->w * level->h; i++) {
  count[level->tiles[i].id & 0xff]++;
 }
 
 //if (count[tileid("rock") & BYTEMASK] < 100) return 0;
 //if (count[tileid("dirt") & BYTEMASK] < 100) return 0;
 //if (count[(tileid("ironOre") & BYTEMASK) + level->depth - 1] < 20) return 0;
 //if (level->depth < 3 && count[tileid("stairsDown") & BYTEMASK] < 2) return 0;
 
 return 1;
}

void generate(level_t* level) {
 int attempt;
 
 if (!level) {
  LOGREPORT("received invalid argument.");
  return;
 }
 
 attempt = 0;
 
 do {
  attempt++;
  
  if (attempt > 100) {
   LOGREPORT("failed to generate a valid map after 100 tries.");
   return;
  }
  
  if (level->depth == 0) {
   generatetopmap(level);
   
   if (!validatetopmap(level)) {
	continue;
   }
   
   return;
  }
  else if (level->depth == -1) {
   generateskymap(level);
   
   if (!validateskymap(level)) {
	continue;
   }
   
   return;
  }
  else {
   generateunderground(level);
   
   if (!validateunderground(level)) {
	continue;
   }
   
   return;
  }
 }
 while (1);
}
