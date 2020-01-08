#include "lua_ext_func.h"

#include <stdlib.h>

#include "lauxlib.h"
#include <errno.h>

#include <string.h>

#define LOAD_F_SIZE 512

#include "ff.h"

typedef struct LoadF {
    FIL *f;  /* file being read */
    char buff[LOAD_F_SIZE];  /* area for reading file */
} LoadF;

static const char *getF (lua_State *L, void *ud, size_t *size) {
    LoadF *lf = (LoadF *)ud;
    if (f_eof(lf->f)) return NULL;

    if (f_read(lf->f, lf->buff, LOAD_F_SIZE, size) != FR_OK) {
        return NULL;
    }

    return lf->buff;
}

static int errfile (lua_State *L, const char *what, int fnameindex) {
    const char *serr = strerror(errno);
    const char *filename = lua_tostring(L, fnameindex) + 1;
    lua_pushfstring(L, "cannot %s %s: %s", what, filename, serr);
    lua_remove(L, fnameindex);
    return LUA_ERRFILE;
}

int lua_fatfs_loadfilex (lua_State *L, const char *filename) {
    LoadF *lf = malloc(sizeof(LoadF));
    if (lf == NULL) {
        return ENOMEM;
    }

    int status, readstatus;
    int c;
    int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */

    lua_pushfstring(L, "@%s", filename);

    lf->f = malloc(sizeof(FIL));
    if (lf->f == NULL) {
        errfile(L, "malloc", fnameindex);
        free(lf);
        return ENOMEM;
    }

    if (f_open(lf->f, filename, FA_READ) != FR_OK) {
        errfile(L, "open", fnameindex);
        free(lf->f);
        free(lf);
        return EIO;
    }

    status = lua_load(L, getF, lf, lua_tostring(L, -1), "t");
    readstatus = f_error(lf->f);
    if (filename) f_close(lf->f);  /* close file (even in case of errors) */
    free(lf->f);
    free(lf);

    if (readstatus) {
        lua_settop(L, fnameindex);  /* ignore results from 'lua_load' */
        return errfile(L, "read", fnameindex);
    }

    lua_remove(L, fnameindex);
    return status;
}
