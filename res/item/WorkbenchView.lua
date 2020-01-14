WorkbenchView = {
 type = item.CRAFTVIEW,
 
 levelcount = 5,
 
 levelcolors = {
  screen.get(-1, 100, 321, 431),
  screen.get(-1, 100, 321, 111),
  screen.get(-1, 100, 321, 555),
  screen.get(-1, 100, 321, 550),
  screen.get(-1, 100, 321, 055),
 },
 
 levels = {
  "wood",
  "stone",
  "ironIngot",
  "goldIngot",
  "gem",
 },
 
 levelaffixes = {
  "Wood ",
  "Rock ",
  "Iron ",
  "Gold ",
  "Gem ",
 },
 
 append = function()
  --item.conceive("Axe", item.AXETYPE, levelcount, levels, levelcolors, levelaffixes);
  --item.conceive("Hoe", item.PICKAXETYPE, levelcount, levels, levelcolors, levelaffixes);
  --item.conceive("Pick", item.PICKAXETYPE, levelcount, levels, levelcolors, levelaffixes);
  --item.conceive("Shvl", item.PICKAXETYPE, levelcount, levels, levelcolors, levelaffixes);
  --item.conceive("Swrd", item.SWORDTYPE, levelcount, levels, levelcolors, levelaffixes);
 end
}

function WorkbenchView:define()
 --view.addrecipe(self, item.form or wrought(item.PLACABLE, "Lantern"), {"ironIngot", 5})
 --view.addrecipe(self, item.form or wrought(item.PLACABLE, "Anvil"), {"ironIngot", 5})
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
end

function WorkbenchView:render(other)
end

function WorkbenchView:tick(other)
end

return WorkbenchView