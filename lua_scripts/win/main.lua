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
end

function w_main:keyboard_click (key)
    if key == cmd.keyboard.num.up then
        self.gui.obj.fv:up()
        self:draw()
    elseif key == cmd.keyboard.num.down then
        self.gui.obj.fv:down()
        self:draw()
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
    self.gui.obj.sb = status_bar:new(self.gui.font.data, self.gui.font.h, 1, 1, 128, 11, "stop", 100)

    rv = self.gui.obj.fv:init()
    if rv ~= 0 then
        return rv
    end

    self:draw()


    while true do
        local cmd_type, cmd_code = os.get_cmd()
        if cmd_type == cmd.keyboard.type.click then
            self:keyboard_click(cmd_code)
        end
    end
end