TitleMenu = {}

local selected = 0

function TitleMenu.reset()
 selected = 1
end

function TitleMenu.tick()
 if input.clicked("up") then
  selected = selected - 1
 end
 
 if input.clicked("down") then
  selected = selected + 1
 end
 
 if selected > 3 then
  selected = 1
 elseif selected < 1 then
  selected = 3
 end
 
 if (input.clicked("attack") or input.clicked("menu")) then
  if selected == 1 then
   menu.change("LobbyMenu")
  elseif selected == 2 then
   menu.change("HowToMenu")
  elseif selected == 3 then
   menu.change("AboutMenu")
  end
 end
end

function TitleMenu.render()
 options = { "Start game", "How to play", "About" }
 
 w = 13
 h = 2
 color = screen.get(0, 010, 131, 551)
 xo = (screen.width() - w * 8) / 2
 yo = 24
 
 screen.clear(0)
 
 for y = 0, h - 1, 1 do
  for x = 0, w - 1, 1 do
   screen.sprite(xo + x * 8, yo + y * 8, x + (y + 6) * 32, color, 0)
  end
 end
 
 for i = 1, 3, 1 do
  message = options[i]
  color = screen.get(0, 222, 222, 222)
  
  if i == selected then
   message = "> " .. message .. " <"
   color = screen.get(0, 555, 555, 555);
  end
  
  screen.font(message, (screen.width() - string.len(message) * 8) / 2, (7 + i) * 8, color)
 end
 
 screen.font("(Arrow keys,X and C)", 0, screen.height() - 8, screen.get(0, 111, 111, 111))
end

return TitleMenu
