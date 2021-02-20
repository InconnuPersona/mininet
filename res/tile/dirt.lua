dirt = {}

dirt.id = 6

function dirt.render()
 color = screen.get(level.dirtcolor(), level.dirtcolor(), level.dirtcolor() - 111, level.dirtcolor() - 111)
 
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 0, 0, color, 0);
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 0, 1, color, 0);
 screen.sprite(tile.x() * 16 + 0, tile.y() * 16 + 8, 2, color, 0);
 screen.sprite(tile.x() * 16 + 8, tile.y() * 16 + 8, 3, color, 0);
end

function interact()
 -- TODO: implement function
 
 --if pliant.inhand() == item.word("ToolItem") then
 -- if tool.type() == tool.word("shovel") then
 --  if pliant.tire(4 - tool.level()) then
 --   level.settile(tile.x(), tile.y(), tile.word("hole"), 0)
 --   level.drop(item.word("dirt"), tile.x() * 16 + randominteger(10) + 3, tile.y() * 16 + randominteger(10) + 3)
 --   sound.play("monsterHurt")
 --   return true
 --  end
 -- elseif tool.type() == tool.word("hoe") then
 --  if pliant.tire(4 - tool.level()) then
 --   level.settile(tile.x(), tile.y(), tile.word("farmland"), 0)
 --   sound.play("monsterHurt")
 --   return true
 --  end
 -- end
 --end
 
 return false
end