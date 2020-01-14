os.dofile("lua_scripts/data.lua")
os.dofile("lua_scripts/log.lua")

os.dofile("lua_scripts/file/err.lua")
os.dofile("lua_scripts/file/io.lua")

os.dofile("lua_scripts/file/list/dir_list.lua")
os.dofile("lua_scripts/file/list/file_list.lua")

os.dofile("lua_scripts/file/list/create_file_and_dir_list.lua")
os.dofile("lua_scripts/file/list/create_dir_name_sort_list.lua")
os.dofile("lua_scripts/file/list/create_file_name_sort_list.lua")

os.dofile("lua_scripts/gui/percent_line.lua")
os.dofile("lua_scripts/gui/play_time.lua")
os.dofile("lua_scripts/gui/play_bar.lua")
os.dofile("lua_scripts/gui/shift_string.lua")
os.dofile("lua_scripts/gui/file_icon.lua")
os.dofile("lua_scripts/gui/scroll.lua")
os.dofile("lua_scripts/gui/fileviewer.lua")
os.dofile("lua_scripts/gui/play_status.lua")
os.dofile("lua_scripts/gui/bat_status.lua")
os.dofile("lua_scripts/gui/status_bar.lua")

os.dofile("lua_scripts/win/main.lua")

os.dofile("lua_scripts/start.lua")

collectgarbage("collect")