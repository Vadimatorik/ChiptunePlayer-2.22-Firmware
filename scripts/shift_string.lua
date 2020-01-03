shift_string = {}

function shift_string:new (s, font, x, y, w, h)
	local o = {}
	lcd.set_font(font)
	o.font = font	
	o.h = h
	o.w = w
	o.s = {}
	o.s.data = s
	o.s.width = lcd.get_str_width(o.s.data)
	o.pos = {}
	o.pos.y = y
	o.pos.x = {}
	o.pos.x.cur = x
	o.pos.x.start = x
	
	if o.s.width < w then
		o.pos.x.min = o.pos.x.cur
	else
		o.pos.x.min = o.pos.x.cur - (o.s.width - w)
	end

	setmetatable(o, self)
    self.__index = self
    return o
end

function shift_string:left ()
	self.pos.x.cur = self.pos.x.cur - 1
	if self.pos.x.cur < self.pos.x.min then
		self.pos.x.cur = self.pos.x.start
	end
end

function shift_string:draw ()
	lcd.set_font(self.font)
	lcd.draw_utf8(self.pos.x.cur, self.pos.y + self.h, self.s.data)
	lcd.set_draw_color(0)
	lcd.draw_box(0, self.pos.y, self.pos.x.start, self.h + 2)
	lcd.draw_box(self.pos.x.start + self.w, self.pos.y, 128 - self.pos.x.start + self.w, self.h + 1)
	lcd.set_draw_color(1)
end