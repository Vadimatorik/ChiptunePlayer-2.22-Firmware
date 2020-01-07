status_bar = {}

function status_bar:new (font, f_h, x, y, w, h, p_stat, percent)
    local o = {
        pos = { x = x, y = y },
        frame = { w = w, h = h },
        ps = play_status:new(x + 2, y + 2, p_stat),
        bs = bat_status:new(font, f_h, 0, y + 2, percent)
    }

    o.bs:set_pos(x + (w - o.bs:get_width() - 3), y + 2)

    setmetatable(o, self)
    self.__index = self    
    
    return o
end

function status_bar:draw ()
    self.ps:draw()
    self.bs:draw()
    lcd.draw_frame(self.pos.x, self.pos.y, self.frame.w, self.frame.h)
end

