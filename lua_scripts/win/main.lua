w_main = {
    gui = {
        obj = {},
        font = {
            data = "u8g2_font_5x7_tf",
            h = 7
        }
    },
    path = "0:",
    fv_file_list_name = ".file_name_sort_list.txt",
    fv_dir_list_name = ".dir_name_sort_list.txt"
}

function w_main:draw ()
    lcd.clean()
    self.gui.obj.sb:draw()
    self.gui.obj.fv:draw()
    self.gui.obj.pb:draw()
    lcd.update()
    collectgarbage("collect")
end

function w_main:keyboard_click (key)
    if key == cmd.keyboard.num.up then
        self.gui.obj.sb:up()
        self.gui.obj.fv:up()
        self:draw()
    elseif key == cmd.keyboard.num.down then
        self.gui.obj.sb:down()
        self.gui.obj.fv:down()
        self:draw()
    elseif key == cmd.keyboard.num.enter then
        local cur_fil_name = self.gui.obj.fv:get_cur_file_name()
        if cur_fil_name == nil then
            return
        end

        aym.play(cur_fil_name)
    end
end

function w_main:start ()
    log("Start os.init()")

    if os.init() ~= true then
        log_err("Fail os.init()")
        is.exit()
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

    self.gui.obj.fv = fileviewer:new(self.gui.font.data, self.gui.font.h, 1, 11, 128, 44, self.path, self.fv_dir_list_name, self.fv_file_list_name)
    self.gui.obj.pb = play_bar:new(self.gui.font.data, self.gui.font.h, 1, 54, 128, 11)
    self.gui.obj.sb = status_bar:new(self.gui.font.data, self.gui.font.h, 1, 1, 128, 11, "stop", 100, self.path, self.fv_dir_list_name, self.fv_file_list_name)

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
        end

        log_used_ram()
        collectgarbage("collect")
    end
end