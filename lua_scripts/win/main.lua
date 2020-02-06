w_main = {
    gui = {
        obj = {},
        font = {
            data = "u8g2_font_5x7_tf",
            h = 7
        }
    },
    cur_path = "0:/",
    cur_file = "",
    fv_file_list_name = ".file_name_sort_list.txt",
    fv_dir_list_name = ".dir_name_sort_list.txt",
    play_state = "stop" -- Еще: play, pause
}

function w_main:draw()
    lcd.clean()
    self.gui.obj.sb:draw()
    self.gui.obj.fv:draw()
    self.gui.obj.pb:draw()
    lcd.update()
    collectgarbage("collect")
end

function w_main:play_current_track()
    local f_len = self.gui.obj.fv:get_cur_file_len_sec()
    self.gui.obj.pb:set(0, f_len)
    aym.play(self.cur_file)
    log("Play new track")
    self.play_state = "play"
    self:draw()
end

function w_main:keyboard_enter_click_file()
    if self.play_state == "stop" then
        log("Change aym state: to play")
        self.cur_file = self.gui.obj.fv:get_cur_file_name()
        self:play_current_track()
    elseif self.play_state == "play" then
        local fv_name = self.gui.obj.fv:get_cur_file_name()
        log("Current play file: " .. self.cur_file)
        log("Current menu file: " .. fv_name)

        if self.cur_file == fv_name then
            aym.pause()

            log("Change aym state: to pause")
            self.play_state = "pause"

            self:draw()
        else
            aym.stop()
            self.cur_file = fv_name
            self:play_current_track()
        end
    elseif self.play_state == "pause" then
        local fv_name = self.gui.obj.fv:get_cur_file_name()
        if self.cur_file == fv_name then
            self.play_state = "play"
            log("Change aym state: to play")
            self:draw()
            aym.pause()
        else
            aym.stop()
            self.cur_file = fv_name
            self:play_current_track()
        end
    end
end

function w_main:keyboard_enter_click_dir()
end

function w_main:keyboard_enter_click()
    local i_type = self.gui.obj.fv:get_cur_item_type()
    if (i_type == "file") then
        self:keyboard_enter_click_file()
    else
        self:keyboard_enter_click_dir()
    end
end

function w_main:keyboard_click(key)
    if key == cmd.keyboard.num.up then
        log("Event: key up")
        self.gui.obj.sb:up()
        self.gui.obj.fv:up()
        self:draw()
    elseif key == cmd.keyboard.num.down then
        log("Event: key down")
        self.gui.obj.sb:down()
        self.gui.obj.fv:down()
        self:draw()
    elseif key == cmd.keyboard.num.enter then
        log("Event: key enter")
        self:keyboard_enter_click()
    end
end

function w_main:aym_play_event(event)
    if event == cmd.aym.play.cmd.tick then
        log("Event: aym tick")
        self.gui.obj.pb:inc()
        self:draw()
    end
end

function w_main:start()
    log("Start os.init()")

    if os.init() ~= true then
        log_err("Fail os.init()")
        os.exit()
    end

    log("Start lcd driver init")
    lcd.driver_init()

    microsd = fat.new_fat()
    log("Start microsd mount")
    if microsd:mount("0") ~= 0 then
        log_err("Fail microsd mount")
        os.exit()
    end

    log_used_ram()
    collectgarbage("collect")

    log_used_ram()
    create_dir_and_file_list("0:")
    collectgarbage("collect")
    log_used_ram()

    create_dir_name_sort_list("0:")
    collectgarbage("collect")
    log_used_ram()

    create_file_name_sort_list("0:")
    collectgarbage("collect")
    log_used_ram()

    self.gui.obj.fv = fileviewer:new(self.gui.font.data, self.gui.font.h, 1, 11, 128, 44, self.cur_path, self.fv_dir_list_name, self.fv_file_list_name)
    self.gui.obj.pb = play_bar:new(self.gui.font.data, self.gui.font.h, 1, 54, 128, 11)
    self.gui.obj.sb = status_bar:new(self.gui.font.data, self.gui.font.h, 1, 1, 128, 11, self.play_state, 100, self.cur_path, self.fv_dir_list_name, self.fv_file_list_name)

    rv = self.gui.obj.fv:init()
    if rv ~= 0 then
        return rv
    end

    rv = self.gui.obj.sb:init()
    if rv ~= 0 then
        return rv
    end

    self:draw()

    while true do
        local cmd_type, cmd_data = os.get_cmd()
        if cmd_type == cmd.keyboard.type.click then
            self:keyboard_click(cmd_data)
        elseif cmd_type == cmd.aym.play.type then
            self:aym_play_event(cmd_data)
        end

        log_used_ram()
        collectgarbage("collect")
    end
end