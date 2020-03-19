Inventory = {
 type = item.STORAGEVIEW,
}

function Inventory:render()
 screen.font("Inventory", 2 * 8 + 4, 1 * 8, screen.get(0, 555, 555, 555))
end

function Inventory:tick()
 
end

return Inventory