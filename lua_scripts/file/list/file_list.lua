file_list = {
    item = {
        size = {
            name = 256,
            track_tick_len = 4
        }
    }
}

file_list.item.size.all = file_list.item.size.name + file_list.item.size.track_tick_len

function file_list:new (path_to_dir, name, fat_fs_file_obj)
    local o = {
        path_to_dir = path_to_dir,
        name = name,
        fat_fs_file_obj = fat_fs_file_obj
    }

    setmetatable(o, self)
    self.__index = self

    return o
end

function file_list:create ()
    return open_file(self.path_to_dir, self.name, self.fat_fs_file_obj, "wrC")
end

function file_list:open ()
    return open_file(self.path_to_dir, self.name, self.fat_fs_file_obj, "r")
end

function file_list:close ()
    return close_file(self.path_to_dir, self.name, self.fat_fs_file_obj)
end

function file_list:write_item (number, file_data)
    -- Name
    local lseek_byte = (number - 1) * self.item.size.all
    local rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = write_file_string(self.path_to_dir, self.name, self.fat_fs_file_obj, file_data.name)
    if rv ~= 0 then
        return rv
    end

    -- Len
	lseek_byte = lseek_byte + self.item.size.name
    rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = write_file_int(self.path_to_dir, self.name, self.fat_fs_file_obj, file_data.len)
    return rv
end

function file_list:read_item (number)
    local ret_file_data = {}

    -- Name
    local lseek_byte = (number - 1) * self.item.size.all
    local rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = read_file_string(self.path_to_dir, self.name, self.fat_fs_file_obj)
    if type(rv) == "number" then
        return rv
    end

    ret_file_data.name = rv

    -- Len
    lseek_byte = lseek_byte + self.item.size.name
    rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = read_file_int(self.path_to_dir, self.name, self.fat_fs_file_obj)
    if rv < 0 then
        return rv
    end

    ret_file_data.len = rv

    return ret_file_data
end

function file_list:get_item_num ()
    local rv = get_file_size(self.path_to_dir, self.name, self.fat_fs_file_obj)

    if rv < 0 then
        return rv
    end

    return math.ceil(rv / self.item.size.all)
end