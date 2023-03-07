Player.class = "player"

--function Player:spawn()
 --item.give(self.inv, "powGlove")
 --item.give(self.inv, "workbench")
--end

--function Player:buoyant()
-- return true
--end

--function Player:die()
-- sound.play("death")
--end

function Player:findSpawn()
 local x, y
 local w, h
 
 w, h = level.size()
 
 while true do
  x = game.random(w)
  y = game.random(h)
  
  if level.getTile(x, y) == tile.id("grass") then
   x = x * 16 + 8
   y = y * 16 + 8
   
   return x, y
  end
 end
end

--function Player:light()
-- return 2
--end

--[[function Player:render()
 xt = 0
 yt = 14
 
 flip1 = bit32.band(bit32.rshift(unit.mob.walked(self), 3), 1)
 flip2 = flip1
 
 if unit.mob.dir(self) == 1 then
  xt = xt + 2
 elseif unit.mob.dir(self) > 1 then
  flip1 = 0
  flip2 = bit32.band(bit32.rshift(unit.mob.walked(self), 4), 1)
  
  if unit.mob.dir(self) == 2 then
   flip1 = 1
  end
  
  xt = xt + 4 + bit32.band(bit32.rshift(unit.mob.walked(self), 3), 1) * 2
 end
 
 xo = unit.x(self) - 8
 yo = unit.y(self) - 11
 
 if unit.inliquid(self) then
  yo = yo + 4
  watercolor = screen.get(-1, -1, 115, 335)
  
  if unit.mob.ticked(self) / 8 % 2 then
   watercolor = screen.get(-1, 335, 5, 115)
  end
  
  screen.sprite(xo + 0, yo + 3, 5 + 13 * 32, watercolor, 0)
  screen.sprite(xo + 8, yo + 3, 5 + 13 * 32, watercolor, 1)
 end
 
 color = screen.get(-1, 100, 220, 532)
 
 --if unit.pliant.hurted(self) > 0 then
 -- color = screen.get(-1, 555, 555, 555)
 --end
 
 -- render head
 screen.sprite(xo + 0 + 8 * flip1, yo + 0, xt + 0 + yt * 32, color, flip1)
 screen.sprite(xo + 8 - 8 * flip1, yo + 0, xt + 1 + yt * 32, color, flip1)
 
 -- render legs
 if not unit.inliquid(self) then
  screen.sprite(xo + 0 + 8 * flip2, yo + 8, xt + 0 + (yt + 1) * 32, color, flip2);
  screen.sprite(xo + 8 - 8 * flip2, yo + 8, xt + 1 + (yt + 1) * 32, color, flip2);
 end
 
end

--[[function Player:tick()
 local attacktime = unit.pliant.attacktime
 local commanded = unit.pliant.command
 local stamina = unit.pliant.stamina
 local tired = unit.pliant.tireddelay
 
 local xa = 0
 local ya = 0
 
 if commanded(self, unit.pliant.MOVEUP) then ya = ya - 1 end
 if commanded(self, unit.pliant.MOVEDOWN) then ya = ya + 1 end
 if commanded(self, unit.pliant.MOVELEFT) then xa = xa - 1 end
 if commanded(self, unit.pliant.MOVERIGHT) then xa = xa + 1 end
 
 if unit.inliquid(self) and unit.mob.ticked(self) % 60 == 0 then
  if stamina(self) > 0 then
   stamina(self, stamina(self) - 1)
  else
   -- self:hurt(1)
  end
 end
 
 if tired(self) % 2 == 0 then
  unit.mob.move(self, xa, ya)
 end
 
 if unit.pliant.command(self, unit.pliant.ATTACK) and stamina(self) > 0 then
  stamina(self, stamina(self) - 1)
  --unit.pliant.tireddelay(self, 0)
  
  -- attack
 end
 
 if commanded(self, unit.pliant.MENU) then
  -- if not unit.pliant.use(self, ) then
   unit.pliant.view(self, "Inventory")
  --end
 end
 
 if attacktime(self) > 0 then
  attacktime(self, attacktime(self) - 1)
 end
end]]--
