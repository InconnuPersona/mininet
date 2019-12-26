Smash = {
 extra = 0,
 word = "trace.Smash",
}

function Smash:blank()
 --sound.play("monsterHurt")
end

function Smash.render()
 color = screen.get(-1, 555, 555, 555)
 x = unit.x()
 y = unit.y()
 
 screen.sprite(x - 8, y - 8, 5 + 12 * 32, color, 2)
 screen.sprite(x - 0, y - 8, 5 + 12 * 32, color, 3)
 screen.sprite(x - 8, y - 0, 5 + 12 * 32, color, 0)
 screen.sprite(x - 0, y - 0, 5 + 12 * 32, color, 1)
end

function etick()
 trace.time(trace.time() + 1)
 
 if (trace.time() > 10) {
  unit.remove()
 }
end

return Smash