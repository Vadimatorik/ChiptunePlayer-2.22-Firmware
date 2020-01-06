os.init()
lcd.driver_init()

cmd = {
    keyboard = {
        type = {
            press = 0,
            click = 1,
            long_press = 2,
            long_click = 3
        },
        num = {
            up = 0,
            down = 1,
            left = 2,
            right = 3,
            enter = 4,
            ret = 5,
            menu = 6
        }
    }
}

--[[
    Отображаем элементы главного окна.
--]]
w_main = {}
w_main.fv = fileviewer:new("u8g2_font_5x7_tf", 7, 1, 11, 128, 44)
w_main.pb = play_bar:new("u8g2_font_5x7_tf", 7, 1, 54, 128, 11)
w_main.sb = status_bar:new("u8g2_font_5x7_tf", 7, 1, 1, 128, 11, "stop", 100)

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

--[[
    Открываем флешку и ищем первый элемент в корне
--]]
microsd = fat.new_fat()
microsd:mount("0")
cur_dir = fat.new_dir()
f_info = cur_dir:findfirst("0:/", "*")

--[[
    Читаем все элементы текущей директории и валидным
     по двум спискам: папкам и валидным файлам.
--]]
local dir_list = {}
local fil_list = {}
local item_name = f_info:name()
while item_name ~= "" do
    if f_info:is_dir() then
        table.insert(dir_list, item_name)
    else
        local fil_info = {}
        fil_info.name = item_name
        fil_info.time = 0
        table.insert(fil_list, fil_info)
    end

    f_info = cur_dir:findnext()
    item_name = f_info:name()
end

-- Сортируем папки и выводим их на главный экран.
table.sort(dir_list)
for _, dir_name in ipairs(dir_list) do
    w_main.fv:add_item("dir", dir_name);
end
dir_list = nil

table.sort(fil_list, function(a, b)
    return a.name < b.name
end)
for _, fil_info in ipairs(fil_list) do
    w_main.fv:add_item("file", fil_info.name, fil_info.time)
end
fil_list = nil

w_main:start()