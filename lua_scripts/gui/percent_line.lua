percent_line = {}

function percent_line:new (x, y, w, h, percent)
	local o = {
		percent = percent,
		frame = {
			pos = { x = x, y = y },
			w = w, h = h
		},
		box = {
			pos = { x = x + 2, y = y + 2 },
			w = w - 4, h = h - 4
		}
	}

	setmetatable(o, self)
    self.__index = self

    return o
end

function percent_line:draw ()
	lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.w, self.frame.h)

	if self.percent ~= 0 then
		lcd.draw_box(self.box.pos.x, self.box.pos.y, math.floor(self.box.w * (self.percent / 100)), self.box.h)
	end
end

function percent_line:set (percent)
	self.percent = percent
	
	collectgarbage("collect")
end

function percent_line:reset ()
	self:set(0)
end