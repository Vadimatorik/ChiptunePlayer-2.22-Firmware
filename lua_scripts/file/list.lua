function create_dir_list (path_to_dir)
    log("Start create sort list")
    local cur_dir = fat.new_dir()
	local cur_dir_item = fat.new_filinfo()
	local f_list_dir = fat.new_file()

	-- DIR
	local path = path_to_dir .. "/.list_dir"
	log("Start create sort list file. Path: " .. path)
	if f_list_dir:open(path, "wrC") ~= 0 then
		log("Fail create sort list file. Path: " .. path)
		return false
	end

	log("Start findfirst")
	if cur_dir:findfirst(cur_dir_item, path_to_dir, "*") ~= 0 then
		log("Fail findfirst. Path: " .. path_to_dir)
		return false
	end

	local item_name = cur_dir_item:name()

	while item_name ~= "" do
		local item_type
		if cur_dir_item:is_dir() then
			item_type = "dir"
		else
			item_type = "file"
		end

		log("Found item. Name: " .. item_name .. ". Type: " .. item_type)

		if cur_dir_item:is_dir() then
			log("Start add string to .list_dir")
			if f_list_dir:add_string(item_name .. "\n") == false then
				log("Fail add string to .list_dir. Path: " .. path)
				return false
			end
		end

		log("Start findnext")
		if cur_dir:findnext(cur_dir_item) ~= 0 then
			log("Fail findnext. Path: " .. path_to_dir)
			return false
		end

		item_name = cur_dir_item:name()
	end

	log("Start close .list_dir")
	if f_list_dir:close() ~= 0 then
		log("Fail close .list_dir. Path: " .. path)
		return false
	end

	return true
end