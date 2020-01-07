play_time = {}

function play_time:get_s_time (time_sec)
	local m = time_sec / 60
	m = m - m % 1 -- Нужно только целое число минут.
	local s = time_sec % 60

	--[[
		Стилистики ради нужно, чтобы количество цифр на экране 
		всегда было по 2 (2 знака минут 2 секунд)
	--]]
	local s_m = ""
	if m < 10 then
		s_m = "0" .. tostring(m) 
	else
		s_m = tostring(m)
	end

	local s_s = ""
	if s < 10 then
		s_s = "0" .. tostring(s)
	else
		s_s = tostring(s)
	end

	--[[ 
		Убираем дополнительный ".0" с конца, добавляемый из-за того, 
		что используется при компилляции lua не double, а float.
	--]]
	if s_m:sub(-2) == ".0" then
    	s_m = s_m:sub(1,-3)
	end

	if s_s:sub(-2) == ".0" then
    	s_s = s_s:sub(1,-3)
	end

	local s_time = s_m .. ":" .. s_s
	return s_time
end

function play_time:new (font, h, time_sec, x, y, win_h)
	lcd.set_font(font)

	local o = {
		font = {
			data = font,
			h = h
		},
		time = {
			sec = time_sec
		},
		pos = { x = x, y = y },
		win = { h = win_h },
		s = {}
	}

	setmetatable(o, self)
    self.__index = self

	o.s.data = o:get_s_time(time_sec)
	o.s.width = lcd.get_str_width(o.s.data)

	collectgarbage("collect")	

    return o
end

function play_time:set_pos (x, y)
	self.pos.x = x
	self.pos.y = y

	collectgarbage("collect")
end

function play_time:draw ()
	lcd.set_clip_window(self.pos.x, self.pos.y, self.pos.x + self.s.width, self.pos.y + self.win.h);
	lcd.set_font(self.font.data)
	lcd.draw_utf8(self.pos.x, self.pos.y + self.font.h, self.s.data)
	lcd.set_clip_window(1, 1, 128 + 1, 64 + 1)
end

function play_time:inc ()
	self.time.sec = self.time.sec + 1
	self.s.data = self:get_s_time(self.time.sec)
	self.s.width = lcd.get_str_width(self.s.data)

	collectgarbage("collect")
end

function play_time:set (time)
	self.time.sec = time
	self.s.data = self:get_s_time(self.time.sec)
	self.s.width = lcd.get_str_width(self.s.data)

	collectgarbage("collect")
end

function play_time:reset ()
	self.time.sec = 0
	self.s.data = self:get_s_time(self.time.sec)
	self.s.width = lcd.get_str_width(self.s.data)

	collectgarbage("collect")
end