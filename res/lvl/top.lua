

level.depth = 0


function generatetopmap(self)
 level.bind(self)
 
 w = level.width()
 h = level.height()
 
 mnoise1 = level.noise(w, h, 16)
 mnoise2 = level.noise(w, h, 16)
 mnoise3 = level.noise(w, h, 16)
 
 noise1 = level.noise(w, h, 32)
 noise2 = level.noise(w, h, 32)
 
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
	level->tiles[i].id = tile.id("water");
   }
   else if (value > 0.5f && mvalue < -1.5f) {
	level->tiles[i].id = tile.id("rock");
   }
   else {
	level->tiles[i].id = tile.id("grass");
   }
  }
 }
 
 generatedeserts(self)
 
 generatefoliage(self)
 
 level.unload()
end


