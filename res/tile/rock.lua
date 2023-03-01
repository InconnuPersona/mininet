tile.define(2, "rock", {
 blocks = function(x, y)
  return true
 end,
 
 render = function(x, y)
  color = screen.get(444, 444, 333, 333)
  transition = screen.get(111, 444, 555, level.data.dirtColor)
  
  rock = tile.id('rock')
  
  u = level.getTile(x, y - 1) ~= rock
  d = level.getTile(x, y + 1) ~= rock
  l = level.getTile(x - 1, y) ~= rock
  r = level.getTile(x + 1, y) ~= rock
  
  ul = level.getTile(x - 1, y - 1) ~= rock
  dl = level.getTile(x - 1, y + 1) ~= rock
  ur = level.getTile(x + 1, y - 1) ~= rock
  dr = level.getTile(x + 1, y + 1) ~= rock
  
  if not u and not l then
   if not ul then
    screen.sprite(x * 16 + 0, y * 16 + 0, 0, color, 0)
   else
    screen.sprite(x * 16 + 0, y * 16 + 0, 7 + 0 * 32, transition, 3)
   end
  else
   screen.sprite(x * 16 + 0, y * 16 + 0, (l and 6 or 5) + (u and 2 or 1) * 32, transition, 3)
  end
  
  if not u and not r then
   if not ur then
    screen.sprite(x * 16 + 8, y * 16 + 0, 1, color, 0)
   else
    screen.sprite(x * 16 + 8, y * 16 + 0, 8 + 0 * 32, transition, 3)
   end
  else
   screen.sprite(x * 16 + 8, y * 16 + 0, (r and 4 or 5) + (u and 2 or 1) * 32, transition, 3)
  end
  
  if not d and not l then
   if not dl then
    screen.sprite(x * 16 + 0, y * 16 + 8, 2, color, 0)
   else
    screen.sprite(x * 16 + 0, y * 16 + 8, 7 + 1 * 32, transition, 3)
   end
  else
   screen.sprite(x * 16 + 0, y * 16 + 8, (l and 6 or 5) + (d and 0 or 1) * 32, transition, 3)
  end
  
  if not d and not r then
   if not dr then
    screen.sprite(x * 16 + 8, y * 16 + 8, 3, color, 0)
   else
    screen.sprite(x * 16 + 8, y * 16 + 8, 8 + 1 * 32, transition, 3)
   end
  else
   screen.sprite(x * 16 + 8, y * 16 + 8, (r and 4 or 5) + (d and 0 or 1) * 32, transition, 3)
  end
 end,
 
 tick = function(x, y)
  damage = level.getData(x, y)
  
  if damage > 0 then
   level.setData(x, y, damage - 1)
  end
 end,
})
