tree = {
 id = 5,
 
 blocks = function(x, y, entity)
  return true
 end,
 
 define = function()
  tile.setflag(tree.id, tile.TOGRASS)
 end,
 
 render = function(x, y)
  color = screen.get(10, 30, 151, level.grasscolor())
  bark1 = screen.get(10, 30, 430, level.grasscolor())
  bark2 = screen.get(10, 30, 430, level.grasscolor())
  
  u = level.gettile(x, y - 1) == tree.id
  l = level.gettile(x - 1, y) == tree.id
  r = level.gettile(x + 1, y) == tree.id
  d = level.gettile(x, y + 1) == tree.id
  ul = level.gettile(x - 1, y - 1) == tree.id
  ur = level.gettile(x + 1, y - 1) == tree.id
  dl = level.gettile(x - 1, y + 1) == tree.id
  dr = level.gettile(x + 1, y + 1) == tree.id
  
  if u and ul and l then
   screen.sprite(x * 16 + 0, y * 16 + 0, 10 + 1 * 32, color, 0)
  else
   screen.sprite(x * 16 + 0, y * 16 + 0, 9 + 0 * 32, color, 0)
  end
  
  if u and ur and r then
   screen.sprite(x * 16 + 8, y * 16 + 0, 10 + 2 * 32, bark2, 0)
  else
   screen.sprite(x * 16 + 8, y * 16 + 0, 10 + 0 * 32, color, 0)
  end
  
  if d and dl and l then
   screen.sprite(x * 16 + 0, y * 16 + 8, 10 + 2 * 32, bark2, 0)
  else
   screen.sprite(x * 16 + 0, y * 16 + 8, 9 + 1 * 32, bark1, 0)
  end
  
  if d and dr and r then
   screen.sprite(x * 16 + 8, y * 16 + 8, 10 + 1 * 32, color, 0)
  else
   screen.sprite(x * 16 + 8, y * 16 + 8, 10 + 3 * 32, bark2, 0)
  end
 end,
 
 tick = function(x, y)
  damage = level.getdata(x, y)
  
  if damage > 0 then
   level.setdata(x, y, damage - 1)
  end
 end,
}

return tree