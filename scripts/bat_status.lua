bat_status = {}

function bat_status:_get_img (percent)
    local img_num = math.floor(5.0 / 100.0 * percent)
    return "img_bat_" .. tostring(img_num)
end

function bat_status:set_percent (percent)
    lcd.set_font(self.font.d)
    self.img.d = self:_get_img(percent)
    self.s.d = "%" .. tostring(percent)
    self.s.w = lcd.get_str_width(self.s.d)
end

function bat_status:new (font, f_h, x, y, percent)
    local o = {
        font = {
            d = font,
            h = f_h
        },
        img = {},
        s = {},
        pos = { x = x, y = y }
    }

    setmetatable(o, self)
    self.__index = self

    o:set_percent(percent)

    return o
end

function bat_status:draw ()
    lcd.set_font(self.font.d)
    lcd.draw_utf8(self.pos.x, self.pos.y + self.font.h, self.s.d)
    lcd.draw_xbm(self.pos.x + self.s.w, self.pos.y, self.img.d)
end

