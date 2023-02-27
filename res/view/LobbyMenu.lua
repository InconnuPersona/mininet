local selected = 0

local addr = ""
local name = ""
local port = ""

function LobbyMenu.reset()
 menu.listen(true)
 
 selected = 1
 
 addr = "127.0.0.1"
 name = "player"
 port = "12345"
end

function LobbyMenu.text(character)
 if selected == 1 then
  if (character >= string.byte(" ")) and (character <= string.byte("~") and (string.len(name) < 10)) then
   name = name .. string.char(character)
  elseif character == input.BACKSPACE then
   name = name:sub(1, -2)
  end
 elseif selected == 2 then
  if ((character >= string.byte("0")) and (character <= string.byte("9"))) or (character == string.byte(".")) then
   addr = addr .. string.char(character)
  elseif character == input.BACKSPACE then
   addr = addr:sub(1, -2)
  end
 elseif selected == 3 then
  if (character >= string.byte("0")) and (character <= string.byte("9")) then
   port = port .. string.char(character)
  elseif character == input.BACKSPACE then
   port = port:sub(1, -2)
  end
 end
end

function LobbyMenu.tick()
 if selected > 3 then
  if input.clicked("up") then
   selected = selected - 1
  end
  
  if input.clicked("down") then
   selected = selected + 1
  end
 end
 
 if selected > 7 then
  selected = 1
 elseif selected < 1 then
  selected = 7
 end
 
 if (input.clicked("attack") or input.clicked("menu")) then
  if selected < 4 then
   selected = selected + 1
  elseif selected == 4 then
   sound.play("test")
   game.start(game.HOST, name, addr, port)
   menu.change("GameMenu")
  elseif selected == 5 then
   sound.play("test")
   game.start(game.CLIENT, name, addr, port)
   menu.change("GameMenu")
  elseif selected == 6 then
   sound.play("test")
   game.start(game.PRIVATE, name, addr, port)
   menu.change("GameMenu")
  elseif selected == 7 then
   menu.change("TitleMenu")
  end
 end
end

function LobbyMenu.render()
 local options = { "Name: ", "IP: ", "Port: ", "Host game", "Join game", "Private game", "Back" }
 
 local sw, sh = screen.size()
 
 screen.clear(0)
 
 for i = 1, 7, 1 do
  message = options[i]
  color = screen.get(0, 222, 222, 222)
  
  if i == 1 then
   message = message .. name
  elseif i == 2 then
   message = message .. addr
  elseif i == 3 then
   message = message .. port
  end
  
  if i == selected then
   message = "> " .. message .. " <"
   color = screen.get(0, 555, 555, 555);
  end
  
  screen.font(message, (sw - string.len(message) * 8) / 2, (2 + i) * 8, color)
 end
end
