water = {}

water.id = 3

function water.define()
 
end

function water.render()
 color = screen.get(005, 005, 115, 115)
 
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 0, 15 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 8, 15 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 0, 15 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 8, 15 + 1 * 32, color, 0)
end

return water