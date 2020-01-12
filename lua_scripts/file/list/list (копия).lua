function get_item_type (item)
	if item:is_dir() then
		return "dir"
	else
		return "file"
	end
end

function open_f_list_dir_to_write (f_list_dir, path_to_dir)
	local path = path_to_dir .. "/.list_dir"
	log("Start create list dir. Path: " .. path)
	if f_list_dir:open(path, "wrC") ~= 0 then
		log_err("Fail create list dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function open_f_list_dir_to_read (f_list_dir, path_to_dir)
	local path = path_to_dir .. "/.list_dir"
	log("Start open list dir. Path: " .. path)
	if f_list_dir:open(path, "r") ~= 0 then
		log_err("Fail open list dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function open_f_list_fil_to_write (f_list_fil, path_to_dir)
	local path = path_to_dir .. "/.list_fil"
	log("Start create list file. Path: " .. path)
	if f_list_fil:open(path, "wrC") ~= 0 then
		log_err("Fail create list file. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function open_f_list_fil_to_read (f_list_fil, path_to_dir)
	local path = path_to_dir .. "/.list_fil"
	log("Start open list file. Path: " .. path)
	if f_list_fil:open(path, "r") ~= 0 then
		log_err("Fail open list file. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function write_item_dir (item_number, f_list_dir, item_name, path_to_dir)
	log("Start lseek (name) in .list_dir")
	local lseek_pos = item_number * list_dir_item_len.sum_len
	if f_list_dir:lseek(lseek_pos) ~= 0 then
		log_err("Fail lseek (name) in .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	log("Start write string (name) to .list_dir")
	if f_list_dir:write_string(item_name) ~= 0 then
		log_err("Fail write string (name) to .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function write_item_fil (item_number, f_list_fil, item_name, path_to_dir)
	log("Start lseek (name) in .list_fil")
	local lseek_pos = item_number * list_fil_item_len.sum_len
	if f_list_fil:lseek(lseek_pos) ~= 0 then
		log_err("Fail lseek (name) in .list_fil. Dir path: " .. path_to_dir)
		return false
	end

	log("Start write string (name) to .list_fil")
	if f_list_fil:write_string(item_name) ~= 0 then
		log_err("Fail write string (name) to .list_fil. Dir path: " .. path_to_dir)
		return false
	end

	log("Start lseek (track_tick_len) in .list_fil")
	lseek_pos = item_number * list_fil_item_len.sum_len
	lseek_pos = lseek_pos + list_fil_item_len.name
	if f_list_fil:lseek(lseek_pos) ~= 0 then
		log_err("Fail lseek (track_tick_len) in .list_fil. Dir path: " .. path_to_dir)
		return false
	end

	log("Start write int (track_tick_len) to .list_fil")
	if f_list_fil:write_int(0) ~= 0 then
		log_err("tart write int (track_tick_len) to .list_fil. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function create_dir_and_file_list (path_to_dir)
    log("Start create dir and file list")
    local cur_dir = fat.new_dir()
	local cur_dir_item = fat.new_filinfo()
	local f_list_dir = fat.new_file()
	local f_list_fil = fat.new_file()

	if open_f_list_dir_to_write(f_list_dir, path_to_dir) ~= true then
		return false
	end

	if open_f_list_fil_to_write(f_list_fil, path_to_dir) ~= true then
		return false
	end

	log("Start findfirst")
	if cur_dir:findfirst(cur_dir_item, path_to_dir, "*") ~= 0 then
		log_err("Fail findfirst. Dir path: " .. path_to_dir)
		return false
	end

	local item_name = cur_dir_item:name()

	local dir_counter = 0
	local fil_counter = 0

	while item_name do
		local item_type = get_item_type(cur_dir_item)

		log("Found item. Name: " .. item_name .. ". Type: " .. item_type)

		if cur_dir_item:is_dir() then
			if write_item_dir(dir_counter, f_list_dir, item_name, path_to_dir) ~= true then
				return false
			end

			dir_counter = dir_counter + 1
		else
			if string.find(item_name, "[.]psg$") then
				if write_item_fil(fil_counter, f_list_fil, item_name, path_to_dir) ~= true then
					return false
				end

				fil_counter = fil_counter + 1
			end
		end

		log("Start findnext")
		if cur_dir:findnext(cur_dir_item) ~= 0 then
			log_err("Fail findnext. Dir path: " .. path_to_dir)
			return false
		end

		item_name = cur_dir_item:name()
	end

	log("Start close .list_dir")
	if f_list_dir:close() ~= 0 then
		log_err("Fail close .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	log("Start close .list_fil")
	if f_list_fil:close() ~= 0 then
		log_err("Fail close .list_fil. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function open_f_list_dir_sort_to_write (f_list_dir, path_to_dir)
	local path = path_to_dir .. "/.list_dir"
	log("Start create sort list dir. Path: " .. path)
	if f_list_dir:open(path, "wrC") ~= 0 then
		log_err("Fail create sort list dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function read_item_dir (item_number, f_list_dir, item_name, path_to_dir)
	log("Start lseek (name) in .list_dir")
	local lseek_pos = item_number * list_dir_item_len.sum_len
	if f_list_dir:lseek(lseek_pos) ~= 0 then
		log_err("Fail lseek (name) in .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	log("Start write string (name) to .list_dir")
	if f_list_dir:write_string(item_name) ~= 0 then
		log_err("Fail write string (name) to .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function create_sort_dir_list (path_to_dir)
	log("Start create sort dir list")
	local f_list_dir = fat.new_file()
	local f_list_dir_sort = fat.new_file()

	if open_f_list_dir_to_read(f_list_dir, path_to_dir) ~= true then
		return false
	end

	if open_f_list_dir_sort_to_write(f_list_dir_sort, path_to_dir) ~= true then
		return false
	end

	local dir_number = f_list_dir:get_size() / list_dir_item_len.sum_len:

	for i = 1, dir_number do

	end

	return true
end

function create_sort_name_fil_list ()

	return true
end

function create_sort_len_fil_list ()

	return true
end


log("Start close .list_dir")
	if  ~= 0 then
		log_err("Fail close .list_dir. Dir path: " .. path_to_dir)
		return false
	end



function open_f_list_dir_to_read (f_list_dir, path_to_dir)
	local path = path_to_dir .. "/.list_dir"
	log("Start open list dir. Path: " .. path)
	if f_list_dir:open(path, "r") ~= 0 then
		log_err("Fail open list dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end

function write_item_dir (item_number, f_list_dir, item_name, path_to_dir)
	log("Start lseek (name) in .list_dir")
	local lseek_pos = item_number * list_dir_item_len.sum_len
	if f_list_dir:lseek(lseek_pos) ~= 0 then
		log_err("Fail lseek (name) in .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	log("Start write string (name) to .list_dir")
	if f_list_dir:write_string(item_name) ~= 0 then
		log_err("Fail write string (name) to .list_dir. Dir path: " .. path_to_dir)
		return false
	end

	return true
end