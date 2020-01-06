scroll = {}

function scroll:set_item (item)
	if item < 1 then
		item = 1
	elseif item > self.num_item then
		item = self.num_item
	end

	self.cur_item = item

	collectgarbage("collect")
end

function scroll:set_num_item (num)
	self.num_item = num
end

function scroll:new (x, y, w, h, x_space, y_space, num_item)
	local o = {
		cur_item = 1,
		num_item = num_item,
		space = {
			x = x_space,
			y = y_space
		},
		frame = {
			pos = {
				x = x,
				y = y,
			},
			w = w,
			h = h
		}
	}

	setmetatable(o, self)
    self.__index = self

    return o
end

function scroll:draw ()
	lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.w, self.frame.h)

	if self.num_item ~= 0 then
		local box_x = self.frame.pos.x + self.space.x
		local box_h = (self.frame.h - self.space.y * 2) / (self.num_item)
		local box_y = self.frame.pos.y + self.space.y + box_h * (self.cur_item - 1)
		local box_w = self.frame.w - self.space.x * 2

		if box_h < 1 then
			box_h = 1
		end

		lcd.draw_box(math.floor(box_x), math.floor(box_y), math.floor(box_w), math.floor(box_h))

		collectgarbage("collect")
	end
end



