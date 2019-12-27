WorkbenchRecipes = {
 id = 0,
 
 function define()
  --id = view.addscape("Workbench", view.CRAFT)
  
  --view.addrecipe(item.form(item.PLACABLE, "Anvil"), {"ironIngot", 5}, id)
  --view.addrecipe(item.form(item.PLACABLE, "Chest"), {"wood", 20}, id)
  --view.addrecipe(item.form(item.PLACABLE, "Furnace"), {"stone", 20}, id)
  --view.addrecipe(item.form(item.PLACABLE, "Lantern"), {"wood", 5, "slime", 10, "glass", 4}, id)
  --view.addrecipe(item.form(item.PLACABLE, "Oven"), {"stone", 15}, id)
  --view.addrecipe(item.form(item.PLACABLE, "Workbench"), {"wood", 20}, id)
  
  --addRecipe(item.TOOL, item.SWORDTYPE, "wood"):addCost("wood", 5)
  --addRecipe(item.TOOL, item.AXETYPE, "wood"):addCost("wood", 5)
  --addRecipe(item.TOOL, item.HOETYPE, "wood"):addCost("wood", 5)
  --addRecipe(item.TOOL, item.PICKAXETYPE, "wood"):addCost("wood", 5)
  --addRecipe(item.TOOL, item.SHOVELTYPE, "wood"):addCost("wood", 5)
  
  --addRecipe(item.TOOL, item.SWORDTYPE, "rock"):addCost("wood", 5):addCost("stone, 5)
  --addRecipe(item.TOOL, item.AXETYPE, "stone"):addCost("wood", 5):addCost("stone, 5)
  --addRecipe(item.TOOL, item.HOETYPE, "stone"):addCost("wood", 5):addCost("stone, 5)
  --addRecipe(item.TOOL, item.PICKAXETYPE, "stone"):addCost("wood", 5):addCost("stone, 5)
  --addRecipe(item.TOOL, item.SHOVELTYPE, "stone"):addCost("wood", 5):addCost("stone, 5)
 end,
 
 function render(other)
  
 end,
 
 function tick(other)
  
 end
}

return WorkbenchRecipes