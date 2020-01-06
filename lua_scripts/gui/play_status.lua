play_status = {}

function play_status:new (x, y, status)
    local o = {
        pos = { x = x, y = y },
        status = status
    }

    setmetatable(o, self)
    self.__index = self

    return o
end

function play_status:draw ()
    lcd.draw_xbm(self.pos.x, self.pos.y, "img_" .. self.status)
end

--[[
    play, pause, stop
--]]
function play_status:set_status (status)
    self.status = status
    collectgarbage("collect")
end

