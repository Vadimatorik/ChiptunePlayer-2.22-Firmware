function get_item_type (item)
	if item:is_dir() then
		return "dir"
	else
		return "file"
	end
end

function create_dir_and_file_list (path_to_dir)
    log("Start create sort list")
    local cur_dir = fat.new_dir()
	local cur_dir_item = fat.new_filinfo()
	local f_list_dir = fat.new_file()
	local f_list_fil = fat.new_file()

	local path = path_to_dir .. "/.list_dir"
	log("Start create sort list dir. Path: " .. path)
	if f_list_dir:open(path, "wrC") ~= 0 then
		log_err("Fail create sort list dir. Dir path: " .. path_to_dir)
		return false
	end

	local path = path_to_dir .. "/.list_fil"
	log("Start create sort list file. Path: " .. path)
	if f_list_fil:open(path, "wrC") ~= 0 then
		log_err("Fail create sort list file. Dir path: " .. path_to_dir)
		return false
	end

	path = nil

	log("Start findfirst")
	if cur_dir:findfirst(cur_dir_item, path_to_dir, "*") ~= 0 then
		log_err("Fail findfirst. Dir path: " .. path_to_dir)
		return false
	end

	local item_name = cur_dir_item:name()

	while item_name do
		local item_type = get_item_type(cur_dir_item)

		log("Found item. Name: " .. item_name .. ". Type: " .. item_type)

		if cur_dir_item:is_dir() then
			log("Start add string to .list_dir")
			if f_list_dir:add_string(item_name .. "\n") == false then
				log_err("Fail add string to .list_dir. Dir path: " .. path_to_dir)
				return false
			end
		else
			if string.find(item_name, "[.]psg$") then
				log("Start add string to .list_fil")
				if f_list_fil:add_string(item_name .. "\n") == false then
					log_err("Fail add string to .list_fil. Dir path: " .. path_to_dir)
					return false
				end
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