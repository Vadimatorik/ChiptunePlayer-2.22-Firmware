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

log_free_ram()
collectgarbage("collect")

log_free_ram()
create_dir_and_file_list("0:")
collectgarbage("collect")
log_free_ram()

create_dir_name_sort_list("0:")
collectgarbage("collect")
log_free_ram()

win = w_main