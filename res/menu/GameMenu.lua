GameMenu = {}

GameMenu.menu = 0

function GameMenu.reset()
 
end

function GameMenu.tick()
 game.tick()
end

function GameMenu.render()
 game.render()
 
 if GameMenu.menu > 0 then
  
 end
end

return GameMenu