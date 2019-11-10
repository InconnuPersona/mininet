-- using flowers recovers stamina

flower = {
 id = 4,
 
 define = function()
  tile.setflag(flower.id, tile.TOGRASS)
 end,
 
 render = function(x, y)
  grass.render(x, y)
  
  data = level.getdata(x, y)
  shape = (data / 16) % 2
  color = screen.get(10, level.grasscolor(), 555, 440)
  
  if shape == 0 then
   screen.sprite(x * 16 + 0, y * 16 + 0, 1 + 1 * 32, color, 0)
   screen.sprite(x * 16 + 8, y * 16 + 8, 1 + 1 * 32, color, 0)
  elseif shape == 1 then
   screen.sprite(x * 16 + 0, y * 16 + 0, 1 + 1 * 32, color, 0)
   screen.sprite(x * 16 + 8, y * 16 + 8, 1 + 1 * 32, color, 0)
  end
 end,
}

return flower