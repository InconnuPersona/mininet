SurfaceMenu = {}

function SurfaceMenu.render()
 game.render()
end

function SurfaceMenu.tick()
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

return SurfaceMenu