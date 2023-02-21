
tile.define(8, "cactus", tile.TOSAND, {
 blocks = function(x, y, ent)
  return true
 end,
 
 render = function(x, y)
  local color = screen.get(20, 40, 50, level.sandcolor())
  
  screen.sprite(x * 16 + 0, y * 16 + 0, 8 + 2 * 32, color, 0)
  screen.sprite(x * 16 + 8, y * 16 + 0, 9 + 2 * 32, color, 0)
  screen.sprite(x * 16 + 0, y * 16 + 8, 8 + 3 * 32, color, 0)
  screen.sprite(x * 16 + 8, y * 16 + 8, 9 + 3 * 32, color, 0)
 end
})
