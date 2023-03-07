
tile.define(1, "grass", {
 flags = tile.TOGRASS,
 
 render = function(x, y)
  print('meme')
  local gcol = level.data.grassColor
  local color = screen.get(gcol, gcol, gcol + 111, gcol + 111)
  local transition = screen.get(gcol - 111, gcol, gcol + 111, gcol)
  
  u = not tile.hasFlag(level.getTile(x, y - 1), tile.TOGRASS)
  d = not tile.hasFlag(level.getTile(x, y + 1), tile.TOGRASS)
  l = not tile.hasFlag(level.getTile(x - 1, y), tile.TOGRASS)
  r = not tile.hasFlag(level.getTile(x + 1, y), tile.TOGRASS)
  
  if not u and not l then
   screen.sprite(x * 16 + 0, y * 16 + 0, 0, color, 0)
  else
   screen.sprite(x * 16 + 0, y * 16 + 0, (l and 11 or 12) + (u and 0 or 1) * 32, transition, 0)
  end
  
  if not u and not r then
   screen.sprite(x * 16 + 8, y * 16 + 0, 1, color, 0)
  else
   screen.sprite(x * 16 + 8, y * 16 + 0, (r and 13 or 12) + (u and 0 or 1) * 32, transition, 0)
  end
  
  if not d and not l then
   screen.sprite(x * 16 + 0, y * 16 + 8, 2, color, 0)
  else
   screen.sprite(x * 16 + 0, y * 16 + 8, (l and 11 or 12) + (d and 2 or 1) * 32, transition, 0)
  end
  
  if not d and not r then
   screen.sprite(x * 16 + 8, y * 16 + 8, 3, color, 0)
  else
   screen.sprite(x * 16 + 8, y * 16 + 8, (r and 13 or 12) + (d and 2 or 1) * 32, transition, 0)
  end
 end
 
 --tick = function(x, y)
  --if game.random(40) ~= 0 then
  -- return
  --end
  --
  --xn = tile.x()
  --yn = tile.y()
  --
  --if game.randomboolean() then
  -- xn = xn + game.random(2) * 2 - 1
  --else
  -- yn = yn + game.random(2) * 2 - 1
  --end
  --
  --if level.gettile(xn, yn) == tile.id("dirt") then
  -- level.settile(xn, yn, grass.id)
  --end
 --end,
 --
 --interact = function(x, y, unit, item)
 --end
})
