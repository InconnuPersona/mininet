
function HowToMenu.tick()
 if input.clicked("attack") or input.clicked("menu") then
  menu.change("TitleMenu")
 end
end

function HowToMenu.render()
 screen.clear(0)
 
 screen.font("How to play", 4 * 8 + 4, 1 * 8, screen.get(0, 555, 555, 555))
 screen.font("Move your character", 0 * 8 + 4, 3 * 8, screen.get(0, 333, 333, 333))
 screen.font("with the arrow keys", 0 * 8 + 4, 4 * 8, screen.get(0, 333, 333, 333))
 screen.font("press C to attack", 0 * 8 + 4, 5 * 8, screen.get(0, 333, 333, 333))
 screen.font("and X to open the", 0 * 8 + 4, 6 * 8, screen.get(0, 333, 333, 333))
 screen.font("inventory and to", 0 * 8 + 4, 7 * 8, screen.get(0, 333, 333, 333))
 screen.font("use items.", 0 * 8 + 4, 8 * 8, screen.get(0, 333, 333, 333))
 screen.font("Select an item in", 0 * 8 + 4, 9 * 8, screen.get(0, 333, 333, 333))
 screen.font("the inventory to", 0 * 8 + 4, 10 * 8, screen.get(0, 333, 333, 333))
 screen.font("equip it.", 0 * 8 + 4, 11 * 8, screen.get(0, 333, 333, 333))
 screen.font("Kill the air wizard", 0 * 8 + 4, 12 * 8, screen.get(0, 333, 333, 333))
 screen.font("to win the game!", 0 * 8 + 4, 13 * 8, screen.get(0, 333, 333, 333))
end
