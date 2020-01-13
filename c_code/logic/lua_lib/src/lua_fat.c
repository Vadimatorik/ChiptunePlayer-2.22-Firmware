#include "lua.h"

#include "lauxlib.h"
#include <string.h>
#include "ff.h"
#include <stdlib.h>

#define ERR_OS_MALLOC_FAIL 20

// NEW
static int lua_fat_new_fat (lua_State *L) {
    FATFS *f = (FATFS *)lua_newuserdata(L, sizeof(FATFS));
    memset(f, 0, sizeof(FATFS));

    luaL_getmetatable(L, "fat.fat");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_new_dir (lua_State *L) {
    DIR *f = (DIR *)lua_newuserdata(L, sizeof(DIR));
    memset(f, 0, sizeof(DIR));

    luaL_getmetatable(L, "fat.dir");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_new_file (lua_State *L) {
    FIL *f = (FIL *)lua_newuserdata(L, sizeof(FIL));
    memset(f, 0, sizeof(FIL));

    luaL_getmetatable(L, "fat.fil");
    lua_setmetatable(L, -2);

    return 1;
}

static int lua_fat_new_filinfo (lua_State *L) {
    FILINFO *f = (FILINFO *)lua_newuserdata(L, sizeof(FILINFO));
    memset(f, 0, sizeof(FILINFO));

    luaL_getmetatable(L, "fat.filinfo");
    lua_setmetatable(L, -2);

    return 1;
}

// MOUNT
static int lua_fat_fat_mount (lua_State *L) {
    FATFS *f = (FATFS *)luaL_checkudata(L, 1, "fat.fat");
    FRESULT fr = 0;
    const char *path = luaL_checkstring(L, 2);

    fr = f_mount(f, path, 1);

    lua_pushnumber(L, fr);

    return 1;
}

// OPEN
static int lua_fat_dir_open (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    const char *path = luaL_checkstring(L, 2);

    FRESULT fr = f_opendir(d, path);

    lua_pushnumber(L, fr);

    return 1;
}

static int lua_fat_file_open (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");

    const char *path = luaL_checkstring(L, 2);
    const char *flags = luaL_checkstring(L, 3);

    BYTE mode = 0;

    for (; *flags != 0; flags++) {
        if (*flags == 'w') {
            mode |= FA_WRITE;
            continue;
        }

        if (*flags == 'r') {
            mode |= FA_READ;
            continue;
        }

        if (*flags == 'c') {
            mode |= FA_CREATE_NEW;
            continue;
        }

        if (*flags == 'C') {
            mode |= FA_CREATE_ALWAYS;
            continue;
        }
    }

    FRESULT fr = f_open(f, path, mode);

    lua_pushnumber(L, fr);

    return 1;
}

static int lua_fat_file_get_size (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");

    int size = f_size(f);

    lua_pushnumber(L, size);
    return 1;
}


// CLOSE
static int lua_fat_dir_close (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");

    FRESULT fr = f_closedir(d);

    lua_pushnumber(L, fr);

    return 1;
}

static int lua_fat_file_close (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");

    FRESULT fr = f_close(f);

    lua_pushnumber(L, fr);

    return 1;
}


static int lua_fat_dir_findfirst (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    FILINFO *fi = (FILINFO *)luaL_checkudata(L, 2, "fat.filinfo");
    const char *path = luaL_checkstring(L, 3);
    const char *mask = luaL_checkstring(L, 4);

    FRESULT fr = f_findfirst(d, fi, path, mask);

    lua_pushnumber(L, fr);

    return 1;
}

static int lua_fat_dir_findnext (lua_State *L) {
    DIR *d = (DIR *)luaL_checkudata(L, 1, "fat.dir");
    FILINFO *fi = (FILINFO *)luaL_checkudata(L, 2, "fat.filinfo");

    FRESULT fr = f_findnext(d, fi);

    lua_pushnumber(L, fr);
    return 1;
}

static int lua_fat_file_write_string (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");
    const char *s = luaL_checkstring(L, 2);

    uint32_t r_w_size = 0;
    int fr = f_write(f, s, strlen(s) + 1, &r_w_size);

    lua_pushinteger(L, -fr);
    return 1;
}

static int lua_fat_file_write_int (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");
    int data = luaL_checkinteger(L, 2);

    uint32_t r_w_size = 0;
    int fr = f_write(f, &data, sizeof(data), &r_w_size);

    lua_pushinteger(L, -fr);
    return 1;
}

static int lua_fat_file_read_string (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");
    TCHAR *s_buf = malloc(sizeof(TCHAR)*FF_MAX_LFN);

    if (s_buf == NULL) {
        lua_pushinteger(L, -ERR_OS_MALLOC_FAIL);
        return 1;
    }

    char *str = f_gets(s_buf, FF_MAX_LFN, f);
    if (str == NULL) {
        free(s_buf);
        lua_pushinteger(L, -f_error(f));
        return 1;
    }

    lua_pushstring(L, s_buf);
    free(s_buf);
    return 1;
}

static int lua_fat_file_read_int (lua_State *L) {
    FIL *f = luaL_checkudata(L, 1, "fat.fil");

    int data;
    uint32_t r_w_size = 0;
    int fr = f_read(f, &data, sizeof(data), &r_w_size);
    if (fr != 0) {
        lua_pushinteger(L, -fr);
    } else {
        lua_pushinteger(L, data);
    }

    return 1;
}

static int lua_fat_file_lseek (lua_State *L) {
    FIL *f = (FIL *)luaL_checkudata(L, 1, "fat.fil");
    int seek = luaL_checkinteger(L, 2);

    int fr = f_lseek(f, seek);

    lua_pushinteger(L, -fr);
    return 1;
}

static int lua_fat_filinfo_name (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    if (strlen(f_info->fname)) {
        lua_pushstring(L, f_info->fname);
        return 1;
    } else {
        return 0;
    }
}

static int lua_fat_filinfo_size (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    if (strlen(f_info->fname)) {
        lua_pushinteger(L, f_info->fsize);
        return 1;
    } else {
        return 0;
    }
}

static int lua_fat_filinfo_is_dir (lua_State *L) {
    FILINFO *f_info = (FILINFO *)luaL_checkudata(L, 1, "fat.filinfo");

    if (strlen(f_info->fname)) {
        lua_pushboolean(L, f_info->fattrib & AM_DIR);
        return 1;
    } else {
        return 0;
    }
}

static const luaL_Reg fat_fat[] = {
    {"mount", lua_fat_fat_mount},
    {NULL, NULL}
};

static const luaL_Reg fat_dir[] = {
    {"open",      lua_fat_dir_open},
    {"close",     lua_fat_dir_close},
    {"findfirst", lua_fat_dir_findfirst},
    {"findnext",  lua_fat_dir_findnext},
    {NULL, NULL}
};

static const luaL_Reg lua_fat_filinfo[] = {
    {"name",     lua_fat_filinfo_name},
    {"get_size", lua_fat_filinfo_size},
    {"is_dir",   lua_fat_filinfo_is_dir},
    {NULL, NULL}
};

static const luaL_Reg fat_file[] = {
    {"open",         lua_fat_file_open},
    {"close",        lua_fat_file_close},
    {"get_size",     lua_fat_file_get_size},
    {"write_string", lua_fat_file_write_string},
    {"write_int",    lua_fat_file_write_int},
    {"read_string",  lua_fat_file_read_string},
    {"read_int",     lua_fat_file_read_int},
    {"lseek",        lua_fat_file_lseek},
    {NULL, NULL}
};

static const luaL_Reg fat_lib[] = {
    {"new_fat",     lua_fat_new_fat},
    {"new_dir",     lua_fat_new_dir},
    {"new_file",    lua_fat_new_file},
    {"new_filinfo", lua_fat_new_filinfo},
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
