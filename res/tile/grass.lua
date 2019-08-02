grass = {}

grass.id = 1

function grass.define()
end

-- only by "pliant" entities
function grass.interact()
 return false
end

function grass.tick()
 --if game.random(40) != 0 then
 -- return
 --end
 
 --xn = tile.x()
 --yn = tile.y()
 
 --if game.randomboolean() then
 -- xn += game.random(2) * 2 - 1
 --else
 -- yn += game.random(2) * 2 - 1
 --end
 
 --if level.gettile(xn, yn) == tile.word("dirt")
 -- level.settile(xn, yn, grass.id, 0)
 --end
end

function grass.render()
 grasscolor = level.grasscolor()
 
 color = screen.get(grasscolor, grasscolor, grasscolor + 111, grasscolor + 111)
 --transitioncolor = screen.get(grasscolor - 111, grasscolor, grasscolor + 111, level.dirtcolor())
 
 --u = not tile.tograss(level.gettile(x, y - 1))
 --d = not tile.tograss(level.gettile(x, y + 1))
 --l = not tile.tograss(level.gettile(x - 1, y))
 --r = not tile.tograss(level.gettile(x + 1, y))
 
 --if not u and not l then
 -- screen.sprite(x * 16 + 0, y * 16 + 0, 0, color, 0)
 --else
 -- screen.sprite(x * 16 + 0, y * 16 + 0, 0, color, 0)
 --end
 
 --if not u and not r then
 -- screen.sprite(x * 16 +, y * 16, 0, color, 0)
 --else
 -- screen.sprite(x * 16, y * 16, 0, color, 0)
 --end
 
 --if not d and not l then
 -- screen.sprite(x * 16, y * 16, 0, color, 0)
 --else
 -- screen.sprite(x * 16, y * 16, 0, color, 0)
 --end
 
 --if not d and not r then
 -- screen.sprite(x * 16, y * 16, 0, color, 0)
 --else
 -- screen.sprite(x * 16, y * 16, 0, color, 0)
 --end
 
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 0, 12 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 8, 12 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 0, 12 + 1 * 32, color, 0)
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 8, 12 + 1 * 32, color, 0)
end

return grass