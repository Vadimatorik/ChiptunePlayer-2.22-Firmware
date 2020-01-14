function get_item_type (item)
    if item:is_dir() then
        return "dir"
    else
        return "file"
    end
end

function create_dir_and_file_list (path_to_dir)
    log("Start create dir and file list")
    local cur_dir = fat.new_dir()
    local cur_dir_item = fat.new_filinfo()
    local fat_dir_list_obj = fat.new_file()
    local fat_file_list_obj = fat.new_file()

    dl = dir_list:new(path_to_dir, ".dir_no_sort_list.txt", fat_dir_list_obj)
    fl = file_list:new(path_to_dir, ".fil_no_sort_list.txt", fat_file_list_obj)

    local rv = dl:create()

    if rv ~= 0 then
        return rv
    end

    rv = fl:create()

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

    local dir_counter = 1
    local fil_counter = 1

    while item_name do
        local item_type = get_item_type(cur_dir_item)

        if cur_dir_item:is_dir() then
            log("Found item. Name: " .. item_name .. ". Type: dir")

            rv = dl:write_item(dir_counter, item_name)
            if rv ~= 0 then
                return rv
            end

            dir_counter = dir_counter + 1
        else
            local item_format = item_name:sub(-4, -1)
            item_format = item_format:lower()

            log("Found item. Name: " .. item_name .. ". Type: fil. Format: " .. item_format)

            if item_format == ".psg" then
                local file_data = {}
                file_data.name = item_name
                file_data.len = 0
                rv = fl:write_item(fil_counter, file_data)
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

        collectgarbage("collect")
    end

	rv = dl:close()
	if rv ~= 0 then
		return rv
	end

	rv = fl:close()
	return rv
end
