fileviewer = {}

function fileviewer:new (x, y, w, h, font, f_h)
    local o = {
        font = { d = font, h = f_h },
        frame = {
            pos = { x = x, y = y },
            w = w, h = h
        },
        space = {
            frame = 1,
            str = { x = { left = 1, right = 1, time = 4 }, y = 1 },
            icon = {
                x = { left = 1, w = 7, right = 1 },
                y = { up = 1, h = 7, down = 1 }
            },
            scroll = 4
        },
        state = {
            cur_pos = 1,
            num_item = 1,
            items = {},
            gui_lines = {}
        }
    }

    o.line_num = math.ceil(h / (f_h + o.space.str.y * 2 + o.space.str.y * 2))
    o.scroll = scroll:new(x + w - o.space.scroll, y, o.space.scroll, h, 0, 1, 0)

    setmetatable(o, self)
    self.__index = self

    return o
end

function fileviewer:_new_line (gui_line_num, item_num)
    local x_start = self.frame.pos.x + self.space.frame + self.space.icon.x.left

    local y_start = self.frame.pos.y + self.space.frame + self.space.str.y
    local y = self.font.h + self.space.str.y + self.space.frame + self.space.str.y
    y = y * (gui_line_num - 1)
    y = y_start + y

    local i_type = self.state.items[item_num].type

	local str_win_h = self.space.icon.y.h
	if gui_line_num == self.line_num then
        str_win_h = (self.frame.pos.y + self.frame.h) - y - self.space.frame - self.space.str.y
	end

    self.state.gui_lines[gui_line_num] = {}
    self.state.gui_lines[gui_line_num].icon = file_icon:new(i_type, x_start, y, str_win_h)

    local time_w = 0
    if i_type == "fil" then
        self.state.gui_lines[gui_line_num].time = play_time:new(self.font.d, self.font.h, self.state.items[item_num].time, 0, y, str_win_h)
        time_w = self.state.gui_lines[gui_line_num].time.s.width
        local x_time = self.frame.pos.x + self.frame.w - self.space.scroll - time_w - self.space.frame
        self.state.gui_lines[gui_line_num].time:set_pos(x_time, y)
    end

    local lin_w = self.frame.w - self.space.frame
    lin_w = lin_w - self.space.icon.x.left - self.space.icon.x.w - self.space.icon.x.right
    lin_w = lin_w - time_w - self.space.scroll

    if time_w ~= 0 then
        lin_w = lin_w - self.space.str.x.time
    end

    lin_w = lin_w - self.space.frame

    local i_name = self.state.items[item_num].name
    x_start = x_start + self.space.icon.x.w + self.space.icon.x.right

	self.state.gui_lines[gui_line_num].s = shift_string:new(i_name, self.font.d, x_start, y, lin_w, self.font.h, str_win_h)

    if self.state.cur_pos == gui_line_num then
        self.state.gui_lines[gui_line_num].s:set_mode(true)
    end
end

function fileviewer:add_item (type, name, time)
    self.state.items[self.state.num_item] = {}
    self.state.items[self.state.num_item].type = type
    self.state.items[self.state.num_item].name = name
    self.state.items[self.state.num_item].time = time

    if self.state.num_item <= self.line_num then
        if self.state.gui_lines[self.state.num_item] == nil then
            self:_new_line(self.state.num_item, self.state.num_item)
        end
    end

    self.state.num_item = self.state.num_item + 1
    self.scroll:set_num_item(self.state.num_item)
end

function fileviewer:draw ()
    lcd.draw_frame(self.frame.pos.x, self.frame.pos.y, self.frame.w, self.frame.h)
    self.scroll:draw()

    for i = 1, self.line_num do
        if self.state.gui_lines[i] == nil then
            break
        end

        self.state.gui_lines[i].s:draw()
        self.state.gui_lines[i].icon:draw()
        if self.state.gui_lines[i].time ~= nil then
            self.state.gui_lines[i].time:draw()
        end
    end

    for i = 1, self.line_num - 1 do
        if self.state.gui_lines[i] == nil then
            break
        end

        local l_x = self.frame.pos.x + self.space.frame
        l_x = l_x + self.space.icon.x.left + self.space.icon.x.w + self.space.icon.x.right

        local l_y = self.frame.pos.y
        l_y = l_y + (self.font.h + self.space.str.y + self.space.frame + self.space.str.y) * i

        local l_w = self.frame.w - self.space.frame
        l_w = l_w - self.space.icon.x.left - self.space.icon.x.w - self.space.icon.x.right
        l_w = l_w - self.space.scroll

        l_w = l_w - self.space.frame

        lcd.draw_h_line(l_x, l_y, l_w)
    end
end

function fileviewer:left ()
    for i = 1, self.line_num do
        if self.state.gui_lines[i] == nil then
            break
        end

        self.state.gui_lines[i].s:left()
    end
end