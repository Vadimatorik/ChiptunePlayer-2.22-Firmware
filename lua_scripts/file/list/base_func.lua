function create_no_sort_dir_list_file (path_to_dir, fat_fs_file_obj)
    local rv = open_file(path_to_dir, ".dir_list", fat_fs_file_obj, "wrC")
    return rv
end

function create_no_sort_fil_list_file (path_to_dir, fat_fs_file_obj)
    local rv = open_file(path_to_dir, ".fil_list", fat_fs_file_obj, "wrC")
    return rv
end

function close_no_sort_dir_list_file (path_to_dir, fat_fs_file_obj)
    local rv = close_file(path_to_dir, ".dir_list", fat_fs_file_obj)
    return rv
end

function close_no_sort_fil_list_file (path_to_dir, fat_fs_file_obj)
    local rv = close_file(path_to_dir, ".fil_list", fat_fs_file_obj)
    return rv
end

function write_dir_list_item (path_to_dir, file_name, fat_fs_file_obj, dir_item_number, dir_name)
    local lseek_byte = dir_item_number * list_dir_item_len.sum_len
    local rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    return write_file_string(path_to_dir, file_name, fat_fs_file_obj, dir_name)
end

function write_fil_list_item (path_to_dir, file_name, fat_fs_file_obj, fil_item_number, file_data)
    -- Name
    local lseek_byte = fil_item_number * list_fil_item_len.sum_len
    local rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = write_file_string(path_to_dir, file_name, fat_fs_file_obj, file_data.name)
    if rv ~= 0 then
        return rv
    end

    -- Len
	lseek_byte = lseek_byte + list_fil_item_len.name
    rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = write_file_int(path_to_dir, file_name, fat_fs_file_obj, file_data.len)
    return rv
end

function read_dir_list_item (path_to_dir, file_name, fat_fs_file_obj, dir_item_number, ret_dir_name)
    local lseek_byte = dir_item_number * list_dir_item_len.sum_len
    local rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    return read_file_string(path_to_dir, file_name, fat_fs_file_obj, ret_dir_name)
end

function read_fil_list_item (path_to_dir, file_name, fat_fs_file_obj, fil_item_number, ret_file_data)
    -- Name
    local lseek_byte = fil_item_number * list_fil_item_len.sum_len
    local rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = read_file_string(path_to_dir, file_name, fat_fs_file_obj, ret_file_data.name)
    if rv ~= 0 then
        return rv
    end

    -- Len
	lseek_byte = lseek_byte + list_fil_item_len.name
    rv = lseek_file(path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = read_file_int(path_to_dir, file_name, fat_fs_file_obj, ret_file_data.len)
    return rv
end

function write_no_sort_dir_list_item (path_to_dir, fat_fs_file_obj, dir_item_number, dir_name)
    local rv = write_dir_list_item(path_to_dir, ".dir_list", fat_fs_file_obj, dir_item_number, dir_name)
    return rv
end

function write_no_sort_fil_list_item (path_to_dir, fat_fs_file_obj, fil_item_number, file_data)
    local rv = write_fil_list_item(path_to_dir, ".fil_list", fat_fs_file_obj, fil_item_number, file_data)
    return rv
end

function get_item_type (item)
    if item:is_dir() then
        return "dir"
    else
        return "file"
    end
end