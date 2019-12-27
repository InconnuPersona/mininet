InventoryMenu = {}

local pliant = 0
local selected = 0

function InventoryMenu.reset()
 --pliant = game.entity()
 
 --game.inmenu(true)
 
 game.view("inventory", player)
 
 view.open("Inventory")
 
 --if unit.pliant.inhand(player)
end

function InventoryMenu.render()
 game.render()
 
 screen.frame("Inventory", 1, 1, 12, 11)
 
 
end

function InventoryMenu.tick()
 -- if the item is crafted, item.craft() is called to confirm the creation.
 -- if it is on server side, the item is crafted, otherwise it is sent to the host to be crafted and updated.
 
 if input.clicked("menu") then
  menu.change("GameMenu")
 end
 
 if input.clicked("up") then
  selected = selected - 1
 end
 
 if input.clicked("down") then
  selected = selected + 1
 end
 
 --game.push(send.CRAFT,)
 
end

return InventoryMenu
