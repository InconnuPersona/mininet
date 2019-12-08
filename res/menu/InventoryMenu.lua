InventoryMenu = {}

function InventoryMenu.reset()
end

function InventoryMenu.render()
 game.render()
 
 screen.font("Inventory", 0, screen.height() - 8, screen.get(1, 1, 12, 11))
end

function InventoryMenu.tick()
 
end

return InventoryMenu
