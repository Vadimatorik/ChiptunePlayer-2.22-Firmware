#include "lua.h"

#include "lauxlib.h"

#include "freertos_obj.h"
#include "freertos_headers.h"
#include "pcb_hardware.h"

enum LUA_OS_CMD_TYPE {
    LUA_OS_CMD_TYPE_KEYBOARD_PRESS = 0,
    LUA_OS_CMD_TYPE_KEYBOARD_CLICK = 1,
    LUA_OS_CMD_TYPE_KEYBOARD_LONG_PRESS = 2,
    LUA_OS_CMD_TYPE_KEYBOARD_LONG_CLICK = 3
};

typedef struct _lua_os_cmd {
    uint8_t type; // LUA_OS_CMD_TYPE.
    uint8_t cmd;
} lua_os_cmd_t;

static QueueHandle_t lua_os_cmd_queue = NULL;
static uint8_t lua_os_cmd_queue_storage_area[LUA_OS_CMD_QUEUE_SIZE*sizeof(lua_os_cmd_t)] = {0};
static StaticQueue_t lua_os_cmd_queue_str = {0};

static int lua_delay_ms (lua_State *L) {
    int delay = luaL_checkinteger(L, 1);
    if (delay < 0) {
        luaL_argcheck(L, 0, 1, "invalid value");
    }

    vTaskDelay(delay);

    return 0;
}

void matrix_keyboard_event_press (uint8_t key) {
    lua_os_cmd_t send_cmd_buf;
    send_cmd_buf.type = LUA_OS_CMD_TYPE_KEYBOARD_PRESS;
    send_cmd_buf.cmd = key;
    xQueueSend(lua_os_cmd_queue, &send_cmd_buf, portMAX_DELAY);
}

void matrix_keyboard_event_click (uint8_t key) {
    lua_os_cmd_t send_cmd_buf;
    send_cmd_buf.type = LUA_OS_CMD_TYPE_KEYBOARD_CLICK;
    send_cmd_buf.cmd = key;
    xQueueSend(lua_os_cmd_queue, &send_cmd_buf, portMAX_DELAY);
}

void matrix_keyboard_event_long_press (uint8_t key) {
    lua_os_cmd_t send_cmd_buf;
    send_cmd_buf.type = LUA_OS_CMD_TYPE_KEYBOARD_LONG_PRESS;
    send_cmd_buf.cmd = key;
    xQueueSend(lua_os_cmd_queue, &send_cmd_buf, portMAX_DELAY);
}

void matrix_keyboard_event_long_click (uint8_t key) {
    lua_os_cmd_t send_cmd_buf;
    send_cmd_buf.type = LUA_OS_CMD_TYPE_KEYBOARD_LONG_CLICK;
    send_cmd_buf.cmd = key;
    xQueueSend(lua_os_cmd_queue, &send_cmd_buf, portMAX_DELAY);
}

static int lua_get_cmd (lua_State *L) {
    static lua_os_cmd_t resv_cmd_buf;
    xQueueReceive(lua_os_cmd_queue, &resv_cmd_buf, portMAX_DELAY);
    lua_pushinteger(L, resv_cmd_buf.type);
    lua_pushinteger(L, resv_cmd_buf.cmd);
    return 2;
}

static int lua_init (lua_State *L) {
    lua_os_cmd_queue = xQueueCreateStatic(LUA_OS_CMD_QUEUE_SIZE, sizeof(lua_os_cmd_t),
                                          lua_os_cmd_queue_storage_area, &lua_os_cmd_queue_str);

    if (init_pcb_hardware() != 0) {
        while (1);
    }

    return 0;
}

static const luaL_Reg os_lib[] = {
    {"init",     lua_init},
    {"delay_ms", lua_delay_ms},
    {"get_cmd",  lua_get_cmd},
    {NULL, NULL}
};

LUAMOD_API int luaopen_os (lua_State *L) {
    luaL_newlib(L, os_lib);
    return 1;
}
