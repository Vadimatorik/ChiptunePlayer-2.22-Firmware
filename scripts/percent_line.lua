percent_line = {}

function percent_line:new (x, y, w, h, percent)
	local o = {}
	o.frame = {}
	o.frame.pos = {}
	o.frame.pos.x = x
	o.frame.pos.y = y
	o.frame.pos.w = w
	o.frame.pos.h = h
	o.box = {}
	o.box.pos = {}
	o.box.pos.x = x + 2
	o.box.pos.y = y + 2
	o.box.pos.w = w - 4
	o.box.pos.h = h - 4
	setmetatable(o, self)
    self.__index = self; 
    return o
end

function percent_line:draw ()
	lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.pos.w, self.frame.pos.h)
	lcd.draw_box(self.box.pos.x, self.box.pos.y, self.box.pos.w, self.box.pos.h)
end