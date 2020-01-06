#include "lua.h"

#include "lauxlib.h"
#include <string.h>
#include "ff.h"

static int lua_fat_new_fat (lua_State *L) {
    FATFS *f = (FATFS *)lua_newuserdata(L, sizeof(FATFS));
    memset(f, 0, sizeof(FATFS));

    luaL_getmetatable(L, "fat.fat");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_fat_mount (lua_State *L) {
    FATFS *f = (FATFS *)luaL_checkudata(L, 1, "fat.fat");
    FRESULT fr = 0;
    const char *path = luaL_checkstring(L, 2);

    fr = f_mount(f, path, 1);

    if (fr != FR_OK) {
        luaL_error(L, "error mount, code %lu", fr);
    };

    return 0;
}

static int lua_fat_new_dir (lua_State *L) {
    DIR *f = (DIR *)lua_newuserdata(L, sizeof(DIR));
    memset(f, 0, sizeof(DIR));

    luaL_getmetatable(L, "fat.dir");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_dir_open (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    FRESULT fr = 0;
    const char *path = luaL_checkstring(L, 2);

    fr = f_opendir(d, path);

    if (fr != FR_OK) {
        luaL_error(L, "error open dir, code %lu", fr);
    };

    return 0;
}

static int lua_fat_dir_findfirst (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    FRESULT fr = 0;
    const char *path = luaL_checkstring(L, 2);
    const char *mask = luaL_checkstring(L, 3);

    FILINFO *filinfo = (FILINFO *)lua_newuserdata(L, sizeof(FILINFO));
    luaL_getmetatable(L, "fat.filinfo");
    lua_setmetatable(L, -2);

    memset(filinfo, 0, sizeof(FILINFO));

    fr = f_findfirst(d, filinfo, path, mask);

    if (fr != FR_OK) {
        luaL_error(L, "error findfirst in dir, code %lu", fr);
    };

    return 1;
}

static int lua_fat_dir_findnext (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    FRESULT fr = 0;

    FILINFO *filinfo = (FILINFO *)lua_newuserdata(L, sizeof(FILINFO));
    luaL_getmetatable(L, "fat.filinfo");
    lua_setmetatable(L, -2);

    memset(filinfo, 0, sizeof(FILINFO));

    fr = f_findnext(d, filinfo);

    if (fr != FR_OK) {
        luaL_error(L, "error findnext in dir, code %lu", fr);
    };

    return 1;
}

static int lua_fat_new_file (lua_State *L) {
    FILE *f = (FILE *)lua_newuserdata(L, sizeof(FILE));
    memset(f, 0, sizeof(FILE));

    luaL_getmetatable(L, "fat.fil");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_file_open (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");
    FRESULT fr = 0;
    const char *path = luaL_checkstring(L, 2);
    const char *flags = luaL_checkstring(L, 3);

    BYTE mode = 0;

    while (*flags != 0) {
        if (*flags == 'w') {
            mode |= FA_WRITE;
        }

        if (*flags == 'r') {
            mode |= FA_READ;
        }

        flags++;
    }

    fr = f_open(f, path, mode);

    if (fr != FR_OK) {
        luaL_error(L, "error open file, code %lu", fr);
    };

    return 0;
}

static int lua_fat_filinfo_name (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    lua_pushstring(L, f_info->fname);

    return 1;
}

static int lua_fat_filinfo_size (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    lua_pushinteger(L, f_info->fsize);

    return 1;
}

static int lua_fat_filinfo_is_dir (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    lua_pushboolean(L, f_info->fattrib & AM_DIR);

    return 1;
}

static const luaL_Reg fat_fat[] = {
    {"mount", lua_fat_fat_mount},
    {NULL, NULL}
};

static const luaL_Reg fat_dir[] = {
    {"open",      lua_fat_dir_open},
    {"findfirst", lua_fat_dir_findfirst},
    {"findnext", lua_fat_dir_findnext},
    {NULL, NULL}
};

static const luaL_Reg lua_fat_filinfo[] = {
    {"name", lua_fat_filinfo_name},
    {"size", lua_fat_filinfo_size},
    {"is_dir", lua_fat_filinfo_is_dir},
    {NULL, NULL}
};

static const luaL_Reg fat_file[] = {
    {"open",      lua_fat_file_open},
    {NULL, NULL}
};

static const luaL_Reg fat_lib[] = {
    {"new_fat",  lua_fat_new_fat},
    {"new_dir",  lua_fat_new_dir},
    {"new_file", lua_fat_new_file},
    {NULL, NULL}
};

LUAMOD_API int luaopen_fat (lua_State *L) {
    luaL_newmetatable(L, "fat.dir");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, fat_dir, 0);

    luaL_newmetatable(L, "fat.filinfo");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, lua_fat_filinfo, 0);

    luaL_newmetatable(L, "fat.fil");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, fat_file, 0);

    luaL_newmetatable(L, "fat.fat");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, fat_fat, 0);

    luaL_newlib(L, fat_lib);
    return 1;
}
