status_bar = {}

function status_bar:new (font, f_h, x, y, w, h, p_stat, percent, path_to_dir, list_dir, list_file)
    local o = {
        font = { data = font },
        pos = { x = x, y = y },
        frame = { w = w, h = h },
        ps = play_status:new(x + 2, y + 2, p_stat),
        num_str = { x = x + 2 + 7 + 2, y = y + 2 + f_h },
        bs = bat_status:new(font, f_h, 0, y + 2, percent),
        path_to_dir = path_to_dir,
        list_dir = list_dir,
        list_file = list_file,
        state = {
            num_item_dir = 0,
            num_item_file = 0,
            num_item = 0,
            cur_file = 0
        }
    }

    o.bs:set_pos(x + (w - o.bs:get_width() - 3), y + 2)

    setmetatable(o, self)
    self.__index = self

    return o
end

function status_bar:draw ()
    self.ps:draw()
    self.bs:draw()
    lcd.draw_frame(self.pos.x, self.pos.y, self.frame.w, self.frame.h)

    if self.num_item == 0 then
        return
    end

    if self.state.cur_file <= self.state.num_item_dir then
        return
    end

    local s_state = tostring(self.state.cur_file - self.state.num_item_dir) .. "/" .. tostring(self.state.num_item_file)
    lcd.set_font(self.font.data)
    lcd.draw_utf8(self.num_str.x, self.num_str.y, s_state)
end

function status_bar:_get_dir_data ()
    local list_dir_obj = fat.new_file()
    local list_file_obj = fat.new_file()
    local dl = dir_list:new(self.path_to_dir, self.list_dir, list_dir_obj)
    local fl = file_list:new(self.path_to_dir, self.list_file, list_file_obj)

    log("Start open dir list")
    local rv = dl:open()
    if rv ~= 0 then
        return rv
    end

    log("Start open file list")
    rv = fl:open()
    if rv ~= 0 then
        return rv
    end

    log("Get num dir")
    rv = dl:get_item_num()
    if rv < 0 then
        return rv
    end
    self.state.num_item_dir = rv
    log("Find dir items: " .. tonumber(self.state.num_item_dir))

    log("Get num file")
    rv = fl:get_item_num()
    if rv < 0 then
        return rv
    end
    self.state.num_item_file = rv
    log("Find file items: " .. tonumber(self.state.num_item_file))

    self.state.num_item = self.state.num_item_dir + self.state.num_item_file

    if self.state.num_item then
        self.state.cur_file = 1
    end

    return 0
end

function status_bar:init ()
    log("Start init status_bar")

    log("Start init fat for status_bar")
    local rv = self:_get_dir_data()
    if rv < 0 then
        return rv
    end

    collectgarbage("collect")
    return 0
end

function status_bar:down ()
    if self.state.cur_file < self.state.num_item then
        self.state.cur_file = self.state.cur_file + 1
    end

    collectgarbage("collect")
end

function status_bar:up ()
    if self.state.cur_file > 1 then
        self.state.cur_file = self.state.cur_file - 1
    end

    collectgarbage("collect")
end