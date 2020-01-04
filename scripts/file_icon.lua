file_icon = {}

function file_icon:new (fil_type, x, y, win_h)
	local o = {}
	o.t = fil_type
	o.pos = {}
	o.pos.x = x
	o.pos.y = y
	o.pos.w = 7
	o.pos.h = 7
	o.win = {}
	o.win.h = win_h

	setmetatable(o, self)
    self.__index = self
    return o
end

function file_icon:draw ()
	lcd.set_clip_window(self.pos.x, self.pos.y, self.pos.x + self.pos.w, self.pos.y + self.win.h);

	if self.t == "fil" then
		lcd.draw_frame(math.floor(self.pos.x + self.pos.w / 6), self.pos.y, math.floor(self.pos.w - self.pos.w / 6), self.pos.h)
	elseif self.t == "dir" then
		lcd.draw_box(math.floor(self.pos.x + self.pos.w / 5 * 3), self.pos.y, self.pos.w - math.floor(self.pos.w / 5 * 3), math.floor(self.pos.h / 6 * 2))
		lcd.draw_box(self.pos.x, math.floor(self.pos.y + self.pos.w / 5), self.pos.w, self.pos.h - math.floor(self.pos.h / 6))
	end

	lcd.set_clip_window(1, 1, 128 + 1, 64 + 1)
end