function open_file (path_to_dir, file_name, fat_fs_file_obj, open_flags)
    local path = path_to_dir .. file_name
    log("Start open file. Path: " .. path)
    local rv = fat_fs_file_obj:open(path, open_flags)
    if rv ~= 0 then
        log_err("Fail open file. Err: " .. get_fat_fs_err_str(rv) .. ". Path: " .. path)
    end

    return rv
end

function close_file (path_to_dir, file_name, fat_fs_file_obj)
    local path = path_to_dir .. file_name
    log("Start close file. Path: " .. path)
    local rv = fat_fs_file_obj:close()
    if rv ~= 0 then
        log_err("Fail close file. Err: " .. get_fat_fs_err_str(rv) .. ". Path: " .. path)
    end

    return rv
end

function lseek_file (path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    local path = path_to_dir .. file_name
    log("Start lseek file. Seek: " .. tostring(lseek_byte) .. " byte. Path: " .. path)
    local rv = fat_fs_file_obj:lseek(lseek_byte)
    if rv ~= 0 then
        local err_str = "Fail lseek file. Err: " .. get_fat_fs_err_str(rv) .. ". "
        err_str = err_str .. "Seek: " .. tostring(lseek_byte) .. " byte. Path: " .. path
        log_err(err_str)
    end

    return rv
end

function write_file_string (path_to_dir, file_name, fat_fs_file_obj, str)
    local path = path_to_dir .. file_name
    log("Start write string to file. Str: " .. str .. ". Path: " .. path)
    local rv = fat_fs_file_obj:write_string(str)
	if rv ~= 0 then
		local err_str = "Fail write string to file. Str: " .. str .. ". Err: " .. get_fat_fs_err_str(rv) .. ". "
		err_str = err_str .. "Path: " .. path
		log_err(err_str)
    end

    return rv
end

function write_file_int (path_to_dir, file_name, fat_fs_file_obj, var)
	local path = path_to_dir .. file_name
	log("Start write int to file. Path: " .. path)
	local rv = fat_fs_file_obj:write_int(var)
	if rv ~= 0 then
		local err_str = "Fail write int to file. Var: " .. tostring(var) .. ". Err: " .. get_fat_fs_err_str(rv) .. ". "
		err_str = err_str .. "Path: " .. path
		log_err(err_str)
	end

	return rv
end

