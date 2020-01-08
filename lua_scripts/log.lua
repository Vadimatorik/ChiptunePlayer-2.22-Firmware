lua_msg = "[LUA] "
end_msg = "\r\n"

function log (str)
    io.write(lua_msg .. str .. end_msg)
end