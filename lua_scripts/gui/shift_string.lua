shift_string = {}

function shift_string:new (s, font, x, y, w, h, win_h)
	lcd.set_font(font)
	local o = {
		font = font,
		h = h,
		w = w,
		win_h = win_h,
		s = {
			data = s,
			width = lcd.get_str_width(s)
		},
		pos = {
			x = {
				cur = x,
				start = x
			},
			y = y
		},
		mode = false
	}

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

function shift_string:set_mode (mode)
	self.mode = mode
end

function shift_string:draw ()
	lcd.set_font(self.font)
	lcd.set_clip_window(self.pos.x.start, self.pos.y, self.pos.x.start + self.w, self.pos.y + self.win_h);

	if self.mode then
		lcd.draw_box(self.pos.x.start, self.pos.y, self.w, self.h)
		lcd.set_draw_color(0)
		lcd.draw_utf8(self.pos.x.cur, self.pos.y + self.h - 1, self.s.data)
		lcd.set_draw_color(1)
	else
		lcd.draw_utf8(self.pos.x.cur, self.pos.y + self.h, self.s.data)
	end
	
	lcd.set_clip_window(1, 1, 128 + 1, 64 + 1)
end