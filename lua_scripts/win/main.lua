w_main = {
	fv = fileviewer:new("u8g2_font_5x7_tf", 7, 1, 11, 128, 44),
	pb = play_bar:new("u8g2_font_5x7_tf", 7, 1, 54, 128, 11),
	sb = status_bar:new("u8g2_font_5x7_tf", 7, 1, 1, 128, 11, "stop", 100)
}

function w_main:draw ()
    lcd.clean()
    self.sb:draw()
    self.fv:draw()
    self.pb:draw()
    lcd.update()
end

function w_main:keyboard_click (key)
    if key == cmd.keyboard.num.up then
        self.fv:up()
        self:draw()
    elseif key == cmd.keyboard.num.down then
        self.fv:down()
        self:draw()
    end
end

function w_main:start ()
    self:draw()

    while true do
        local cmd_type, cmd_code = os.get_cmd()
        if cmd_type == cmd.keyboard.type.click then
            self:keyboard_click(cmd_code)
        end
    end
end