AboutMenu = {}

function AboutMenu.tick()
 if input.clicked("attack") or input.clicked("menu") then
  menu.change("TitleMenu")
 end
end

function AboutMenu.render()
 screen.clear(0)
 
 screen.font("About Minicraft", 2 * 8 + 4, 1 * 8, screen.get(0, 555, 555, 555))
 screen.font("Minicraft was made", 0 * 8 + 4, 3 * 8, screen.get(0, 333, 333, 333))
 screen.font("by Markus Persson", 0 * 8 + 4, 4 * 8, screen.get(0, 333, 333, 333))
 screen.font("For the 22'nd ludum", 0 * 8 + 4, 5 * 8, screen.get(0, 333, 333, 333))
 screen.font("dare competition in", 0 * 8 + 4, 6 * 8, screen.get(0, 333, 333, 333))
 screen.font("December 2011.", 0 * 8 + 4, 7 * 8, screen.get(0, 333, 333, 333))
 screen.font("it is dedicated to", 0 * 8 + 4, 9 * 8, screen.get(0, 333, 333, 333))
 screen.font("my father. <3", 0 * 8 + 4, 10 * 8, screen.get(0, 333, 333, 333))
end

return AboutMenu
