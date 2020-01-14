dir_list = {
    item = {
        size = {
            name = 256
        }
    }
}

dir_list.item.size.all = dir_list.item.size.name

function dir_list:new (path_to_dir, name, fat_fs_file_obj)
    local o = {
        path_to_dir = path_to_dir,
        name = name,
        fat_fs_file_obj = fat_fs_file_obj
    }

    setmetatable(o, self)
    self.__index = self

    return o
end

function dir_list:create ()
    return open_file(self.path_to_dir, self.name, self.fat_fs_file_obj, "wrC")
end

function dir_list:open ()
    return open_file(self.path_to_dir, self.name, self.fat_fs_file_obj, "r")
end

function dir_list:close ()
    return close_file(self.path_to_dir, self.name, self.fat_fs_file_obj)
end

function dir_list:write_item (number, name)
    local lseek_byte = (number - 1) * self.item.size.all
    local rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    return write_file_string(self.path_to_dir, self.name, self.fat_fs_file_obj, name)
end

function dir_list:read_item (number)
    local lseek_byte = (number - 1) * self.item.size.all
    local rv = lseek_file(self.path_to_dir, self.name, self.fat_fs_file_obj, lseek_byte)
    if rv ~= 0 then
        return rv
    end

    rv = read_file_string(self.path_to_dir, self.name, self.fat_fs_file_obj)

    if type(rv) ~= "number" then
        log("Read str: " .. rv)
    end

    return rv
end

function dir_list:get_item_num ()
    local rv = get_file_size(self.path_to_dir, self.name, self.fat_fs_file_obj)

    if rv < 0 then
        return rv
    end

    return math.ceil(rv / self.item.size.all)
end