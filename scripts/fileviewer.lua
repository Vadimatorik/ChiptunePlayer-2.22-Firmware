fileviewer = {}

function fileviewer:new (x, y, w, h, font, f_h)
	local o = {}
	o.font = {}
	o.font.data = font
	o.font.h = f_h
	o.frame = {}
	o.frame.pos = {}
	o.frame.pos.x = x
	o.frame.pos.y = y
	o.frame.pos.w = w
	o.frame.pos.h = h
	o.space = {}
	o.space.string = {}
	o.space.string.x = 1
	o.space.string.y = 1
	o.space.icon = 1
	o.space.scroll = 4
	o.line_num = math.floor(h / (o.font.h + o.space.string.y * 2 + 2))
	o.cur_pos = 1
	o.num_item = 1
	o.icon = {}
	o.icon.w = 7

	o.items = {}
	o.gui_lines = {}

	o.scroll = scroll:new(x + w - o.space.scroll, y, o.space.scroll, h, 0, 1, 0)

	setmetatable(o, self)
    self.__index = self

    return o
end


function fileviewer:_new_line (gui_line_num, item_num)
	self.gui_lines[gui_line_num] = {}
	local x_start = self.frame.pos.x + 1 + self.space.icon
	local y = self.frame.pos.y + 1 + self.space.icon + (self.font.h + self.space.icon * 2 + 2) * (gui_line_num - 1)
	self.gui_lines[gui_line_num].icon = file_icon:new(self.items[item_num].type, x_start, y)

	local time_w = 0
	x_start = x_start + self.icon.w + 1

	if self.items[item_num].type == "fil" then
		self.gui_lines[gui_line_num].time = play_time:new(self.font.data, self.font.h, self.items[item_num].time, 0, y)
		time_w = self.gui_lines[gui_line_num].time.s.width
		local x_time = self.frame.pos.x + self.frame.pos.w - self.space.scroll - time_w - 1
		self.gui_lines[gui_line_num].time:set_pos(x_time, y)
	end

	local lin_w = self.frame.pos.w - (self.space.icon + self.icon.w * 2 + 2 + self.space.scroll + time_w)
	self.gui_lines[gui_line_num].s = shift_string:new(self.items[item_num].name, self.font.data, x_start, y, lin_w,self.font.h)
end 

function fileviewer:add_item (type, name, time)
	self.items[self.num_item] = {}
	self.items[self.num_item].type = type
	self.items[self.num_item].name = name
	self.items[self.num_item].time = time

	if self.num_item <= self.line_num then
		if self.gui_lines[self.num_item] == nil then
			self:_new_line(self.num_item, self.num_item)
		end
	end

	self.num_item = self.num_item + 1
	self.scroll:set_num_item(self.num_item)
end



function fileviewer:draw ()
	for i = 1, self.line_num do		
		if self.gui_lines[i] == nil then
			break
		end

		self.gui_lines[i].s:draw()
		self.gui_lines[i].icon:draw()
		if self.gui_lines[i].time ~= nil then
			self.gui_lines[i].time:draw()
		end
	end

	for i = 1, self.line_num - 1 do
		if self.gui_lines[i] == nil then
			break
		end

		local l_x = self.frame.pos.x + 1 + self.space.icon * 2 + self.icon.w
		local l_w = self.frame.pos.w - (self.space.icon * 2 + self.icon.w) - self.space.scroll - 1

		lcd.draw_h_line(l_x, self.frame.pos.y + ((self.font.h + self.space.string.y * 2) + 2) * i, l_w)
	end

	self.scroll:draw()

	lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.pos.w, self.frame.pos.h)
end

function fileviewer:left ()
	for i = 1, self.line_num - 1 do
		if self.gui_lines[i] == nil then
			break
		end

		self.gui_lines[i].s:left()
	end
end