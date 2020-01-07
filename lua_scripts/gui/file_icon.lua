file_icon = {}

function file_icon:new (fil_type, x, y, win_h)
	local o = {
		t = fil_type,
		pos = {
			x = x,
			y = y
		},
		w = 7,
		h = 7,
		win = { h = win_h }
	}

	setmetatable(o, self)
	self.__index = self
	
	return o
end

function file_icon:draw ()
	lcd.set_clip_window(self.pos.x, self.pos.y, self.pos.x + self.w, self.pos.y + self.win.h)
	lcd.draw_xbm(self.pos.x,  self.pos.y, "img_" .. self.t)
	lcd.set_clip_window(1, 1, 128 + 1, 64 + 1)
end