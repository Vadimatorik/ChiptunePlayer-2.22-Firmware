play_bar = {}

function play_bar:new (font, f_h, x, y, w, h)
    local o = {
        frame = {
            pos = { x = x, y = y },
            w = w, h = h
        },
        time = { cur = 0, ed = 0 },
        space = {
            frame = 1,
            obj = 1
        }
    }

	local y_in = y + o.space.frame + o.space.obj
    o.cur_time = play_time:new(font, f_h, o.time.cur, x + o.space.frame, y_in , f_h)
    o.end_time = play_time:new(font, f_h, o.time.ed, 0, y_in, f_h)
    o.end_time:set_pos(x + w - o.end_time.s.width - o.space.frame - o.space.obj, y_in)

    local pl_x = x + o.space.frame + o.cur_time.s.width + 1
    local pl_w = w - o.cur_time.s.width - o.end_time.s.width - o.space.frame * 2 - o.space.obj * 2
    o.percent_line = percent_line:new(pl_x, y_in, pl_w, f_h, 0)

    collectgarbage("collect")

    setmetatable(o, self)
    self.__index = self

    return o
end

function play_bar:draw ()
	lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.w, self.frame.h)
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

    self.time.cur = self.time.cur + 1
    self.cur_time:inc()
    self.percent_line:set(100 / self.time.ed * self.time.cur)

    collectgarbage("collect")
end

function play_bar:set (cur_t, end_t)
    self.time.cur = cur_t
    self.time.ed = end_t

    self.cur_time:set(self.time.cur)
    self.end_time:set(self.time.ed)

    self.percent_line:reset()

    collectgarbage("collect")
end

function play_bar:reset ()
    self:set(0, 0)
end

