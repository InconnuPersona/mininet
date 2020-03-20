Inventory = {
 type = item.STORAGEVIEW,
}

local choice = 1

function Inventory:open()
 choice = 1
end

function Inventory:render()
 screen.frame("Inventory", 1, 1, 12, 11)
 
 --item.render
end

function Inventory:tick()
 local commanded = unit.pliant.command
 
 if commanded(self, unit.pliant.MENU) then
  unit.pliant.view(self, nil)
 end
 
 if commanded(self, unit.pliant.MOVEUP) then
  choice = choice - 1
 end
 
 if commanded(self, unit.pliant.MOVEDOWN) then
  choice = choice + 1
 end
 
 --items = item.pilewidth(unit.mob.pile(self))
 
 --if selected > 3 then
 -- selected = 1
 --elseif selected < 1 then
 -- selected = 3
 --end
 
 if commanded(self, unit.pliant.ATTACK) then
  --item = item.take(unit.mob.pile(self), selected)
  
  --unit.pliant.inhand(self, item)
  
  unit.pliant.view(self, "");
 end
 
end

return Inventory