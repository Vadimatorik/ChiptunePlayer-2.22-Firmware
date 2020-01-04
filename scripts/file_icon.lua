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
	lcd.set_clip_window(self.pos.x, self.pos.y, self.pos.x + self.pos.w, self.pos.y + self.win.h)
	lcd.draw_xbm(self.pos.x,  self.pos.y, "img_" .. self.t)
	lcd.set_clip_window(1, 1, 128 + 1, 64 + 1)
end