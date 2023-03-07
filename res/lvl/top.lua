
top = {
 dirtColor = 322,
 grassColor = 141,
 sandColor = 550,
 
 density = 8 -- monster, unit, and mob density
}

function top.generate(w, h)
 local grass = tile.id("grass")
 local rock = tile.id("rock")
 local water = tile.id("water")
 
 local mnoise1 = level.noise(w, h, 16)
 local mnoise2 = level.noise(w, h, 16)
 local mnoise3 = level.noise(w, h, 16)
 
 local noise1 = level.noise(w, h, 32)
 local noise2 = level.noise(w, h, 32)
 
 for y = 0, h - 1 do
  for x = 0, w - 1 do
   local i = x + y * w;
   
   local value = math.abs(noise1[i] - noise2[i]) * 3 - 2;
   local mvalue = math.abs(mnoise1[i] - mnoise2[i]);
   local mvalue = math.abs(mvalue - mnoise3[i]) * 3 - 2;
   
   local xd = x / (w - 1) * 2 - 1;
   local yd = y / (h - 1) * 2 - 1;
   
   if (xd < 0) then
	xd = -xd;
   end
   
   if (yd < 0) then
	yd = -yd;
   end
   
   local distance = yd
   
   if xd >= yd then
    distance = xd
   end
   
   distance = distance * distance * distance * distance;
   distance = distance * distance * distance * distance;
   
   value = value + 1 - distance * 20;
   
   if (value < -0.5) then
	level.setTile(x, y, water, 0)
   elseif ((value > 0.5) and (mvalue < -1.5)) then
	level.setTile(x, y, rock, 0)
   else
	level.setTile(x, y, grass, 0)
   end
  end
 end
end

function top.validate(w, h)
 return true
end

function top.tick()
end

level.define(0, top)
