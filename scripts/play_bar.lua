play_bar = {}

function play_bar:new ()
	local o = {}
	o.font = "u8g2_font_5x7_tf"
	o.time = {}
	o.time.cur = 0
	o.time.ed = 0
	o.cur_time = play_time:new(o.font, o.time.cur, 0, 63 + 1)
	o.end_time = play_time:new(o.font, o.time.ed, 127 - o.cur_time.s.width, 63 + 1)
	o.percent_line = percent_line:new(o.cur_time.pos.x + 1 + o.end_time.s.width, 63 - 6 + 1, 127 - o.end_time.s.width - 1 - (o.cur_time.pos.x + 1 + o.end_time.s.width), 6, 0)
	setmetatable(o, self)
    self.__index = self
    return o
end

function play_bar:draw ()
	self.cur_time:draw()
	self.percent_line:draw()
	self.end_time:draw()
end

function play_bar:inc ()
	if self.time.ed == 0 then
		return
	end

	if self.time.ed == self.time.cur then
		return
	end

	self.time.cur =  self.time.cur + 1
	self.cur_time:inc()
	self.percent_line:set(100 / self.time.ed * self.time.cur)
end

function play_bar:set (cur_t, end_t)
	self.time.cur = cur_t
	self.time.ed = end_t

	self.cur_time:set(self.time.cur)	
	self.end_time:set(self.time.ed)

	self.percent_line:reset()
end

function play_bar:reset ()
	self:set(0, 0)
end

