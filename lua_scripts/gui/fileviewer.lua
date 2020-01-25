fileviewer = {}

function fileviewer:new (font, f_h, x, y, w, h, path_to_dir, list_dir, list_file)
    local o = {
        font = { d = font, h = f_h },
        frame = {
            pos = { x = x, y = y },
            w = w, h = h
        },
        space = {
            frame = 1,
            str = { x = { left = 1, right = 1, time = 2 }, y = 1 },
            icon = {
                x = { left = 1, w = 7, right = 1 },
                y = { up = 1, h = 7, down = 1 }
            },
            scroll = 4
        },
        state = {
            cur_item = 1,
            num_item = 0,
            num_item_dir = 0,
            num_item_file = 0,
            cur_gui_pos = 1,
            gui_lines = {}
        },
        view_mode_down = false,
        path_to_dir = path_to_dir,
        list_dir = list_dir,
        list_file = list_file
    }

    o.line_num = math.ceil(h / (f_h + o.space.str.y * 2 + o.space.str.y * 2))
    o.scroll = scroll:new(x + w - o.space.scroll, y, o.space.scroll, h, 0, 1, 0)

    setmetatable(o, self)
    self.__index = self

    return o
end

function fileviewer:_get_fat_item (item_num)
    log("Start get fat item for fileviewer. Number: ", tostring(item_num))

    local item = {}

    if item_num <= self.state.num_item_dir then
        local rv = self.dl:read_item(item_num)
        if type(rv) == "number" then
            return rv
        end
        local dl_item = rv

        item.type = "dir"
        item.name = dl_item.name
    else
        local rv = self.fl:read_item(item_num - self.state.num_item_dir)
        if type(rv) == "number" then
            return rv
        end
        local fl_item = rv

        item.type = "file"
        item.name = fl_item.name
        item.len = fl_item.len
    end

    return item
end

function fileviewer:_new_line (gui_line_num, item_num)
    local rv = self:_get_fat_item(item_num)
    if type(rv) == "number" then
        return rv
    end

    local item = rv
    log("Received fileviewer item. Type: " .. item.type .. ". Name: " .. item.name .. ". Len: " .. tostring(item.len))

    local x_start = self.frame.pos.x + self.space.frame + self.space.icon.x.left

    local y_start = self.frame.pos.y + self.space.frame + self.space.str.y
    local y = self.font.h + self.space.str.y + self.space.frame + self.space.str.y
    y = y_start + y * (gui_line_num - 1)

    local i_type = item.type

    local str_win_h = self.space.icon.y.h
    if gui_line_num == self.line_num then
        str_win_h = (self.frame.pos.y + self.frame.h) - y - self.space.frame - self.space.str.y
    end

    self.state.gui_lines[gui_line_num] = {}
    self.state.gui_lines[gui_line_num].icon = file_icon:new(i_type, x_start, y, str_win_h)

    local time_w = 0
    if i_type == "file" then
        self.state.gui_lines[gui_line_num].time = play_time:new(self.font.d, self.font.h, item.len, 0, y, str_win_h)
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

    x_start = x_start + self.space.icon.x.w + self.space.icon.x.right

    self.state.gui_lines[gui_line_num].s = shift_string:new(item.name, self.font.d, x_start, y, lin_w, self.font.h, str_win_h)

    if self.state.cur_gui_pos == gui_line_num then
        self.state.gui_lines[gui_line_num].s:set_mode(true)
    end

    collectgarbage("collect")
    return rv
end

function fileviewer:_init_fat ()
    self.list_dir_obj = fat.new_file()
    self.list_file_obj = fat.new_file()
    self.dl = dir_list:new(self.path_to_dir, self.list_dir, self.list_dir_obj)
    self.fl = file_list:new(self.path_to_dir, self.list_file, self.list_file_obj)

    log("Start open dir list")
    local rv = self.dl:open()
    if rv ~= 0 then
        return rv
    end

    log("Start open file list")
    rv = self.fl:open()
    if rv ~= 0 then
        return rv
    end

    log("Get num dir")
    rv = self.dl:get_item_num()
    if rv < 0 then
        return rv
    end
    self.state.num_item_dir = rv
    log("Find dir items: " .. tonumber(self.state.num_item_dir))

    log("Get num file")
    rv = self.fl:get_item_num()
    if rv < 0 then
        return rv
    end
    self.state.num_item_file = rv
    log("Find file items: " .. tonumber(self.state.num_item_file))

    self.state.num_item = self.state.num_item_dir + self.state.num_item_file

    return 0
end

function fileviewer:init ()
    log("Start init fileviewer")

    log("Start init fat for fileviewer")
    local rv = self:_init_fat()
    if rv < 0 then
        return rv
    end

    log("Start init gui line for fileviewer")
    gui_line = 1
    if self.state.num_item < self.line_num then
        gui_line = self.state.num_item
    else
        gui_line = self.line_num
    end

    for i = 1, gui_line do
        self:_new_line(i, i)
    end

    self.scroll:set_num_item(self.state.num_item)
    collectgarbage("collect")

    return 0
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
        if self.state.gui_lines[i + 1] == nil then
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

    collectgarbage("collect")
end

function fileviewer:left_active_line ()
    if self.state.gui_lines[self.cur_gui_pos] == nil then
        return
    end

    self.state.gui_lines[self.cur_gui_pos].s:left()
end

function fileviewer:get_cur_file_name ()
    if self.state.cur_item <= self.state.num_item_dir then
        return nil
    end

    local item = self:_get_fat_item(self.state.cur_item)
    return item.name
end

function fileviewer:down ()
    if self.state.cur_item >= self.state.num_item then
        return
    end

    self.state.cur_item = self.state.cur_item + 1

    self.scroll:set_item(self.state.cur_item)

    if self.state.cur_gui_pos < self.line_num - 1 then
        self.state.gui_lines[self.state.cur_gui_pos].s:set_mode(false)
        self.state.cur_gui_pos = self.state.cur_gui_pos + 1
        self.state.gui_lines[self.state.cur_gui_pos].s:set_mode(true)
        collectgarbage("collect")
        return
    end

    if self.state.cur_item == self.state.num_item then
        self.view_mode_down = true
    end

    if self.view_mode_down ~= true then
        for i = 1, self.line_num do
            self:_new_line(i, self.state.cur_item - (self.line_num - 2) + i - 1)
        end
    else
        for i = 1, self.line_num - 1 do
            self:_new_line(i, self.state.cur_item - (self.line_num - 2) + i - 1)
        end

        self.state.gui_lines[self.line_num] = nil
    end

    collectgarbage("collect")
end

function fileviewer:up ()
    if self.state.cur_item == 1 then
        return
    end

    self.state.cur_item = self.state.cur_item - 1

    self.scroll:set_item(self.state.cur_item)

    if self.state.cur_gui_pos > 1 then
        self.state.gui_lines[self.state.cur_gui_pos].s:set_mode(false)
        self.state.cur_gui_pos = self.state.cur_gui_pos - 1
        self.state.gui_lines[self.state.cur_gui_pos].s:set_mode(true)
        collectgarbage("collect")
        return
    end

    self.view_mode_down = false

    for i = 1, self.line_num do
        self:_new_line(i, self.state.cur_item + i - 1)
    end

    collectgarbage("collect")
end