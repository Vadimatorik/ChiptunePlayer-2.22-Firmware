function create_dir_name_sort_list (path_to_dir)
    log("Start create dir name sort list")
    local fat_dir_no_sort_list_obj = fat.new_file()
    local fat_dir_name_sort_list_obj = fat.new_file()

    dl = dir_list:new(path_to_dir, ".dir_no_sort_list.txt", fat_dir_no_sort_list_obj)
    sl = dir_list:new(path_to_dir, ".dir_name_sort_list.txt", fat_dir_name_sort_list_obj)

    local rv = dl:open()
    if rv ~= 0 then
        return rv
    end

    rv = sl:create()
    if rv ~= 0 then
        return rv
    end

    log("Get num items")

    rv = dl:get_item_num()
    if rv < 0 then
        return rv
    end
    local item_num = rv
    log("Find items: " .. tonumber(item_num))

    pos_items = {}
    for i = 1, item_num do
        pos_items[i] = i
    end

    local sort_dir_item_func = function(p1, p2)
        if sd_err ~= 0 then
            return false
        end

        collectgarbage("collect")

        local rv = dl:read_item(p1)
        if type(rv) == "number" then
            sd_err = rv
            return false
        end

        local s1 = rv

        rv = dl:read_item(p2)
        if type(rv) == "number" then
            sd_err = rv
            return false
        end

        local s2 = rv

        return s1 < s2
    end

    log("Start sort")
    table.sort(pos_items, sort_dir_item_func)

    if sd_err ~= 0 then
        log_err("Fail sort")
        rv = sd_err
        sd_err = 0
        return sd_err
    end

    log("Write sort name dir list")
    for sort_list_pos, no_sort_list_pos in pairs(pos_items) do
        collectgarbage("collect")

        local rv = dl:read_item(no_sort_list_pos)
        if type(rv) == "number" then
            return rv
        end

        local item_data = rv

        rv = sl:write_item(sort_list_pos, item_data)
        if rv ~= 0 then
            return rv
        end
    end

    rv = dl:close()
    if rv ~= 0 then
        return rv
    end

    rv = sl:close()
    return rv
end
