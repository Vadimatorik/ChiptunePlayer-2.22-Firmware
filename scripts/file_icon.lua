file_icon = {}

function file_icon:new (fil_type, x, y)
	local o = {}
	o.t = fil_type
	o.pos = {}
	o.pos.x = x
	o.pos.y = y
	o.pos.w = 7
	o.pos.h = 7

	setmetatable(o, self)
    self.__index = self
    return o
end

function file_icon:draw ()
	if self.t == "fil" then
		lcd.draw_frame(math.floor(self.pos.x + self.pos.w / 6), self.pos.y, math.floor(self.pos.w - self.pos.w / 6), self.pos.h)
	elseif self.t == "dir" then
		lcd.draw_box(math.floor(self.pos.x + self.pos.w / 5 * 3), self.pos.y, self.pos.w - math.floor(self.pos.w / 5 * 3), math.floor(self.pos.h / 6 * 2))
		lcd.draw_box(self.pos.x, math.floor(self.pos.y + self.pos.w / 5), self.pos.w, math.floor(self.pos.h / 6 * 5))
	end
end