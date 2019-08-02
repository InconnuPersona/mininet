cactus = {}

cactus.id = 8

function cactus.define()
end

function cactus.render()
 color = screen.get(20, 40, 50, level.sandcolor)
 
 screen.sprite(tile.x() * 16 + 0, tile.y * 16 + 0, 8 + 2 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y * 16 + 0, 9 + 2 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 0, tile.y * 16 + 8, 8 + 3 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y * 16 + 8, 9 + 3 * 32, color, 0)
end

return cactus