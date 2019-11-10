Spark = {}

Spark.word = "trace.Spark"
Spark.extra = 0

function Spark.define()
end

function Spark.blank()
 trace.lifeTime(60 * 10 + game.random(30))
end

function transient(unit)
 return false
end

function Spark.render()
 if trace.time() >= trace.lifeTime - 6 * 20 then
  return
 end
 
 screen.sprite(unit.x() - 4, unit.y() - 4 - 2, 8 + 13 * 32, screen.get(-1, 555, 555, 555), game.random(4))
 screen.sprite(unit.x() - 4, unit.y() - 4 + 2, 8 + 13 * 32, screen.get(-1, 000, 000, 000), game.random(4))
end

return Spark