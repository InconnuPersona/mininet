sand = {
 id = 7,
 
 define = function()
  tile.setflag(sand.id, tile.TOSAND)
 end,
 
 render = function(x, y)
  color = screen.get(level.sandcolor() + 2, level.sandcolor(), level.sandcolor() - 110, level.sandcolor() - 110)
  transition = screen.get(level.sandcolor() - 110, level.sandcolor(), level.sandcolor() - 110, level.dirtcolor())
  
  u = not tile.flagged(level.gettile(x, y - 1), tile.TOSAND)
  d = not tile.flagged(level.gettile(x, y + 1), tile.TOSAND)
  l = not tile.flagged(level.gettile(x - 1, y), tile.TOSAND)
  r = not tile.flagged(level.gettile(x + 1, y), tile.TOSAND)
  
  steppedOn = level.getdata(x, y) > 0
  
  if not u and not l then
   if not steppedOn then
    screen.sprite(x * 16 + 0, y * 16 + 0, 0, color, 0)
   else
    screen.sprite(x * 16 + 0, y * 16 + 0, 3 + 1 * 32, color, 0)
   end
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
   if not steppedOn then
    screen.sprite(x * 16 + 8, y * 16 + 8, 3, color, 0)
   else
    screen.sprite(x * 16 + 8, y * 16 + 8, 3 + 1 * 32, color, 0)
   end
  else
   screen.sprite(x * 16 + 8, y * 16 + 8, (r and 13 or 12) + (d and 2 or 1) * 32, transition, 0)
  end
 end
}

return sand