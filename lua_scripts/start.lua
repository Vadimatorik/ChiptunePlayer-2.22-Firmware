lcd.driver_init()

--[[
    Отображаем элементы главного окна.
--]]
w_main = {}
w_main.fv = fileviewer:new("u8g2_font_5x7_tf", 7, 1, 11, 128, 44, data)
w_main.pb = play_bar:new("u8g2_font_5x7_tf", 7, 1, 54, 128, 11)
w_main.sb = status_bar:new("u8g2_font_5x7_tf", 7, 1, 1, 128, 11, "stop", 100)

function w_main:draw ()
    self.sb:draw()
    self.fv:draw()
    self.pb:draw()
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

lcd.clean()
w_main:draw()
lcd.update()