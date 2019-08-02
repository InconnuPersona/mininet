Player = {}

Player.word = "pliant.Player"
Player.extra = 0

function Player.bioluminance()
 radius = 2
 item = unit:pliant.item()
 
 if item != nil and item:item.class == item.FURNITURE then
  rr = item:item.luminance()
  
  if rr > radius then
   radius = rr
  end
 end
 
 return radius
end

function Player.blank(unit)
 unit:unit.x(24)
 unit:unit.y(24)
 
 unit:pliant.stamina(pliant.maxstamina())
 
 --unit:mob.newpile()
 
 -- additem may take a string converted to an item ord or an item id; it may also take a number argument for multiple additions
 --unit:mob.additem("powerglove")
 --unit:mob.additem("workbench") -- assumed 1 instance without any arguments
end

function Player.buoyant()
 return true
end

-- should be hardcoded
--function Player.command(unit, value)
-- 
--end

function damageagainst(unit, other)
 damage = game.random(3) + 1
 inhand = unit:pliant.inhand()
 
 if inhand != nil then
  damage = damage + inhand:item.damage(other)
 end
 
 return damage
end

function Player.define()
 pliant.maxstamina(10)
end

function Player.render(unit)
 xt = 0
 yt = 14
 
 flip1 = unit:mob.walk() >> 3 & 1
 flip2 = unit:mob.walk() >> 3 & 1
 
 if unit:pliant.aim() == 1 then
  xt = xt + 2
 else if unit:pliant.aim() > 1 then
  flip1 = 0
  filp2 = unit:mob.walk() >> 4 & 1
  
  if unit:pliant.aim() == 2 then
   flip1 = 1;
  end
  
  xt = xt + 4 + (unit:mob.walk() >> 3 & 1) * 2
 end
 
 xo = unit:unit.x() - 8
 yo = unit:unit.y() - 11
 
 if unit:unit.swimming() then
 
 end
 
 if item.(get("activeItem"), "FurnitureItem") then
 
 end
 
 if unit:pliant.item() then
  
 end
end

function Player.spawnpoint(unit)
 
end

function Player.tick(unit)
 unit:mob.tick()
 
 dodgetime = unit:pliant.dodgetime()
 rechargedelay = unit:rechargedelay()
 
 if dodgetime > 0 then
  unit:pliant.dodgetime(dodgetime - 1)
 end
 
 if rechargedelay > 0 then
  unit:pliant.rechargedelay(rechargedelay - 1)
 end
end

function Player.touch(unit, other)
 if other:unit.class() == unit.DROP then
  
 end
end

return Player