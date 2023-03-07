tile.define(3, "water", {
 flags = tile.flags(tile.TOSAND, tile.TOWATER),
 
 --blocks = function(x, y, entity)
  --return not unit.buoyant(entity)
 --end,
 
 render = function(x, y)
  local seed = (game.ticks() + (x / 2 - y) * 4311) / 10 * 54687121 + x * 3271612 + y * 3412987161
  
  --game.seedRandom(seed)
  
  color = screen.get(005, 005, 115, 115)
  tcolor1 = screen.get(3, 005, level.data.dirtColor - 111, level.data.dirtColor)
  tcolor2 = screen.get(3, 005, level.data.sandColor - 110, level.data.sandColor)
  
  u = not tile.hasFlag(level.getTile(x, y - 1), tile.TOWATER)
  d = not tile.hasFlag(level.getTile(x, y + 1), tile.TOWATER)
  l = not tile.hasFlag(level.getTile(x - 1, y), tile.TOWATER)
  r = not tile.hasFlag(level.getTile(x + 1, y), tile.TOWATER)
  
  su = u and tile.hasFlag(level.getTile(x, y - 1), tile.TOSAND)
  sd = d and tile.hasFlag(level.getTile(x, y + 1), tile.TOSAND)
  sl = l and tile.hasFlag(level.getTile(x - 1, y), tile.TOSAND)
  sr = r and tile.hasFlag(level.getTile(x + 1, y), tile.TOSAND)
  
  if not u and not l then
   screen.sprite(x * 16 + 0, y * 16 + 0, game.random(4), color, game.random(4));
  else
   screen.sprite(x * 16 + 0, y * 16 + 0, (l and 14 or 15) + (u and 0 or 1) * 32, (su or sl) and tcolor2 or tcolor1, 0);
  end
  
  if not u and not r then
   screen.sprite(x * 16 + 8, y * 16 + 0, game.random(4), color, game.random(4));
  else
   screen.sprite(x * 16 + 8, y * 16 + 0, (r and 16 or 15) + (u and 0 or 1) * 32, (su or sr) and tcolor2 or tcolor1, 0);
  end
  
  if not d and not l then
   screen.sprite(x * 16 + 0, y * 16 + 8, game.random(4), color, game.random(4));
  else
   screen.sprite(x * 16 + 0, y * 16 + 8, (l and 14 or 15) + (d and 2 or 1) * 32, (sd or sl) and tcolor2 or tcolor1, 0);
  end
  
  if not d and not r then
   screen.sprite(x * 16 + 8, y * 16 + 8, game.random(4), color, game.random(4));
  else
   screen.sprite(x * 16 + 8, y * 16 + 8, (r and 16 or 15) + (d and 2 or 1) * 32, (sd or sr) and tcolor2 or tcolor1, 0);
  end
  
  game.seedrandom(nil)
 end,
 
 tick = function(x, y)
  if game.randomboolean() then
   x = x + game.random(2) * 2 - 1
  else
   y = y + game.random(2) * 2 - 1
  end
  
  if level.getTile(x, y) == tile.id("hole") then
   level.setTile(x, y, water.id)
  end
 end,
})