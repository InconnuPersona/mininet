ViewMenu = {}

function ViewMenu.render()
 game.render()
 
 
end

function ViewMenu.tick()
 if input.clicked("menu") then
  menu.change("GameMenu")
 end
 
 if input.clicked("attack") then
  --view.
 end
 
 if input.clicked("up") then
 end
 
 if input.clicked("down") then
 end
end

return ViewMenu