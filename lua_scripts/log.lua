lua_msg = "[LUA] "
lua_err_msg = "[LUA_ERR] "

end_msg = "\r\n"


function log (str)
    io.write(lua_msg .. str .. end_msg)
end

function log_used_ram ()
    io.write(lua_msg .. "Used ram: " .. tostring(collectgarbage("count")*1024) .. end_msg)
end

function log_err (str)
    io.write(lua_err_msg .. str .. end_msg)
end