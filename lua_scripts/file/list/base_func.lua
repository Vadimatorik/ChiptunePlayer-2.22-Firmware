function get_item_type (item)
    if item:is_dir() then
        return "dir"
    else
        return "file"
    end
end