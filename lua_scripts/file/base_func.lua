function open_file (path_to_dir, file_name, fat_fs_file_obj, open_flags)
    local path = path_to_dir .. file_name
    log("Start open file. Path file: " .. path)
    local rv = fat_fs_file_obj:open(path, open_flags)
    if rv ~= 0 then
        log_err("Fail open file. Err: " .. get_fat_fs_err_str(rv) .. ". Path file: " .. path)
    end

    return rv
end

function close_file (path_to_dir, file_name, fat_fs_file_obj)
    local path = path_to_dir .. file_name
    log("Start close file. Path file: " .. path)
    local rv = fat_fs_file_obj:close()
    if rv ~= 0 then
        log_err("Fail close file. Err: " .. get_fat_fs_err_str(rv) .. ". Path file: " .. path)
    end

    return rv
end

function lseek_file (path_to_dir, file_name, fat_fs_file_obj, lseek_byte)
    local path = path_to_dir .. file_name
    log("Start lseek file. Seek: " .. tostring(lseek_byte) .. " byte. Path file: " .. path)
    local rv = fat_fs_file_obj:lseek(lseek_byte)
    if rv ~= 0 then
        local err_str = "Fail lseek file. Err: " .. get_fat_fs_err_str(rv) .. ". "
        err_str = err_str .. "Seek: " .. tostring(lseek_byte) .. " byte. Path file: " .. path
        log_err(err_str)
    end

    return rv
end

function write_file_string (path_to_dir, file_name, fat_fs_file_obj, str)
    local path = path_to_dir .. file_name
    log("Start write string to file. Str: " .. str .. ". Path file: " .. path)
    local rv = fat_fs_file_obj:write_string(str)
	if rv ~= 0 then
		local err_str = "Fail write string to file. Str: " .. str .. ". Err: " .. get_fat_fs_err_str(rv) .. ". "
		err_str = err_str .. "Path file: " .. path
		log_err(err_str)
    end

    return rv
end

function write_file_int (path_to_dir, file_name, fat_fs_file_obj, var)
	local path = path_to_dir .. file_name
	log("Start write int to file. Path file: " .. path)
	local rv = fat_fs_file_obj:write_int(var)
	if rv ~= 0 then
		local err_str = "Fail write int to file. Var: " .. tostring(var) .. ". Err: " .. get_fat_fs_err_str(rv) .. ". "
		err_str = err_str .. "Path file: " .. path
		log_err(err_str)
	end

	return rv
end

function read_file_string (path_to_dir, file_name, fat_fs_file_obj, ret_str)
    local path = path_to_dir .. file_name
    log("Start read string from file. Path file: " .. path)
    local rv = fat_fs_file_obj:read_string()
    if type(rv) == "number" then
        local err_str = "Fail read string from file. Err: " .. get_fat_fs_err_str(rv) .. ". "
        err_str = err_str .. "Path file: " .. path
        log_err(err_str)
        return rv
    else
        ret_str = rv
        return 0
    end
end

function read_file_int (path_to_dir, file_name, fat_fs_file_obj, ret_var)
    local path = path_to_dir .. file_name
    log("Start read int from file. Path file: " .. path)
    local rv = fat_fs_file_obj:read_int()
    if rv < 0 then
        local err_str = "Fail read int from file. Var: " .. tostring(var) .. ". Err: " .. get_fat_fs_err_str(rv) .. ". "
        err_str = err_str .. "Path file: " .. path
        log_err(err_str)
        return rv
    else
        ret_var = rv
        return 0
    end
end
