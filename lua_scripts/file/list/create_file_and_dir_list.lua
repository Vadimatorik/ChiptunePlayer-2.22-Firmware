function create_dir_and_file_list (path_to_dir)
    log("Start create dir and file list")
    local cur_dir = fat.new_dir()
    local cur_dir_item = fat.new_filinfo()
    local dir_list = fat.new_file()
    local file_list = fat.new_file()

    local rv = create_no_sort_dir_list_file(path_to_dir, dir_list)

    if rv ~= 0 then
        return rv
    end

    rv = create_no_sort_fil_list_file(path_to_dir, file_list)

    if rv ~= 0 then
        return rv
    end

    log("Start findfirst")
    rv = cur_dir:findfirst(cur_dir_item, path_to_dir, "*")
    if rv ~= 0 then
        log_err("Fail findfirst. Err: " .. get_fat_fs_err_str(rv) .. ". Dir path: " .. path_to_dir)
        return rv
    end

    local item_name = cur_dir_item:name()

    local dir_counter = 0
    local fil_counter = 0

    while item_name do
        local item_type = get_item_type(cur_dir_item)

        log("Found item. Name: " .. item_name .. ". Type: " .. item_type)

        if cur_dir_item:is_dir() then
            rv = write_no_sort_dir_list_item(path_to_dir, dir_list, dir_counter, item_name)
            if rv ~= 0 then
                return rv
            end

            dir_counter = dir_counter + 1
        else
            if string.find(item_name, "[.]psg$") then
                local file_data = {}
                file_data.name = item_name
                file_data.len = 0
                rv = write_no_sort_fil_list_item(path_to_dir, file_list, fil_counter, file_data)
                if rv ~= 0 then
                    return rv
                end

                fil_counter = fil_counter + 1
            end
        end

        log("Start findnext")
        rv = cur_dir:findnext(cur_dir_item)
        if rv ~= 0 then
            log_err("Fail findnext. Err: " .. get_fat_fs_err_str(rv) .. ". Dir path: " .. path_to_dir)
            return rv
        end

        item_name = cur_dir_item:name()
    end

	rv = close_no_sort_dir_list_file(path_to_dir, file_list)
	if rv ~= 0 then
		return rv
	end

	rv = close_no_sort_fil_list_file(path_to_dir, dir_list)
	return rv
end
