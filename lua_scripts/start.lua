log("Start os.init()")

if os.init() ~= true then
	log("Fail os.init()")
	is.exit()
end

log("Start lcd driver init")
lcd.driver_init()

microsd = fat.new_fat()
log("Start microsd mount")
if microsd:mount("0") ~= 0 then
	log("Fail microsd mount")
	os.exit()
end

--local r = create_dir_list("0:")
--io.write(tostring(r))

win = w_main