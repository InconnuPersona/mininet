
top = {
 dirtColor = 322,
 grassColor = 141,
 sandColor = 550,
 
 density = 8
}

function top.generate(w, h)
 local grass = tile.id('grass')
 
 for y = 0, h do
  for x = 0, w do
   level.setTile(x, y, grass, 0)
  end
  
 end
 
--[[ local mnoise1 = level.noise(w, h, 16)
 local mnoise2 = level.noise(w, h, 16)
 local mnoise3 = level.noise(w, h, 16)
 
 local noise1 = level.noise(w, h, 32)
 local noise2 = level.noise(w, h, 32)
 
 for y = 0, y < h do
  for x = 0, x < w do
   local i = x + y * w;
   
   local value = math.abs(noise1[i] - noise2[i]) * 3 - 2;
   local mvalue = math.abs(mnoise1[i] - mnoise2[i]);
   local mvalue = math.abs(mvalue - mnoise3[i]) * 3 - 2;
   
   local xd = x / (w - 1) * 2 - 1;
   local yd = y / (h - 1) * 2 - 1;
   
   if (xd < 0) {
	xd = -xd;
   }
   
   if (yd < 0) {
	yd = -yd;
   }
   
   local distance = yd
   
   if xd >= yd then
    distance = xd
   end
   
   distance = distance * distance * distance * distance;
   distance = distance * distance * distance * distance;
   
   value = value + 1 - distance * 20;
   
   if (value < -0.5) {
	--level->tiles[i].id = tileid("water");
   }
   else if (value > 0.5 && mvalue < -1.5) {
	--level->tiles[i].id = tileid("rock");
   }
   else {
	--level.settile(i, "grass");
   }
  }
 }
 ]]--
end

function top.validate(w, h)
 return true
end

function top.tick()
end

level.define(0, top)
