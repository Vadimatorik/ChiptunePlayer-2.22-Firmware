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

function play_time:new (font, time_sec, x, y)
	local o = {}
	lcd.set_font(font)
	o.font = font	
	o.time = {}
	o.time.sec = time_sec
	o.pos = {}
	o.pos.x = x
	o.pos.y = y
	o.s = {}
	o.s.data = self:get_s_time(o.time.sec)
	o.s.width = lcd.get_str_width(o.s.data)
	setmetatable(o, self)
    self.__index = self; 
    return o
end

function play_time:draw ()
	lcd.set_font(self.font)
	lcd.draw_utf8(self.pos.x, self.pos.y, self.s.data)
end
