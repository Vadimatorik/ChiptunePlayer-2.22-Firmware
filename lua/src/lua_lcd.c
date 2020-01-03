#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

#include "u8g2.h"
#include "l.h"
#include "lua.h"

#include <string.h>

u8g2_t u8g2 = {0};

#define LIB_POS_FIX 1

static int lua_u8g2_clean (lua_State *L) {
    u8g2_ClearBuffer(&u8g2);
    return 0;
}

static int lua_u8g2_draw_box (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);

    u8g2_DrawBox(&u8g2, x, y, w, h);

    return 0;
}

static int lua_u8g2_draw_circle (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int rad = luaL_checkinteger(L, 3);
    int opt = luaL_optinteger(L, 4, U8G2_DRAW_ALL);

    u8g2_DrawCircle(&u8g2, x0, y0, rad, opt);

    return 0;
}

static int lua_u8g2_draw_disc (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int rad = luaL_checkinteger(L, 3);
    int opt = luaL_optinteger(L, 4, U8G2_DRAW_ALL);

    u8g2_DrawDisc(&u8g2, x0, y0, rad, opt);

    return 0;
}

static int lua_u8g2_draw_ellipse (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int rx = luaL_checkinteger(L, 3);
    int ry = luaL_checkinteger(L, 4);
    int opt = luaL_optinteger(L, 5, U8G2_DRAW_ALL);

    u8g2_DrawEllipse(&u8g2, x0, y0, rx, ry, opt);

    return 0;
}

static int lua_u8g2_draw_filled_ellipse (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int rx = luaL_checkinteger(L, 3);
    int ry = luaL_checkinteger(L, 4);
    int opt = luaL_optinteger(L, 5, U8G2_DRAW_ALL);

    u8g2_DrawFilledEllipse(&u8g2, x0, y0, rx, ry, opt);

    return 0;
}

static int lua_u8g2_draw_frame (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);

    u8g2_DrawFrame(&u8g2, x, y, w, h);

    return 0;
}

static int lua_u8g2_draw_glyph (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int enc = luaL_checkinteger(L, 3);

    u8g2_DrawGlyph(&u8g2, x, y, enc);

    return 0;
}

static int lua_u8g2_draw_h_line (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);

    u8g2_DrawHLine(&u8g2, x, y, w);

    return 0;
}

static int lua_u8g2_draw_line (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int x1 = luaL_checkinteger(L, 3) - LIB_POS_FIX;
    int y1 = luaL_checkinteger(L, 4) - LIB_POS_FIX;

    u8g2_DrawLine(&u8g2, x0, y0, x1, y1);

    return 0;
}

static int lua_u8g2_draw_pixel (lua_State *L) {


    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;

    u8g2_DrawPixel(&u8g2, x, y);

    return 0;
}

static int lua_u8g2_draw_r_box (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int r = luaL_checkinteger(L, 5);

    u8g2_DrawRBox(&u8g2, x, y, w, h, r);

    return 0;
}

static int lua_u8g2_draw_r_frame (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int r = luaL_checkinteger(L, 5);

    u8g2_DrawRFrame(&u8g2, x, y, w, h, r);

    return 0;
}

static int lua_u8g2_draw_str (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    const char *str = luaL_checkstring(L, 3);

    u8g2_DrawStr(&u8g2, x, y, str);

    return 0;
}

static int lua_u8g2_draw_triangle (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int x1 = luaL_checkinteger(L, 3) - LIB_POS_FIX;
    int y1 = luaL_checkinteger(L, 4) - LIB_POS_FIX;
    int x2 = luaL_checkinteger(L, 5) - LIB_POS_FIX;
    int y2 = luaL_checkinteger(L, 6) - LIB_POS_FIX;

    u8g2_DrawTriangle(&u8g2, x0, y0, x1, y1, x2, y2);

    return 0;
}

static int lua_u8g2_draw_utf8 (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    const char *str = luaL_checkstring(L, 3);

    u8g2_DrawUTF8(&u8g2, x, y, str);

    return 0;
}

static int lua_u8g2_draw_v_line (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int h = luaL_checkinteger(L, 3);

    u8g2_DrawVLine(&u8g2, x, y, h);

    return 0;
}

static int lua_u8g2_draw_xbm (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    size_t len;
    const char *bitmap = luaL_checklstring(L, 5, &len);

    u8g2_DrawXBM(&u8g2, x, y, w, h, (uint8_t *)bitmap);

    return 0;
}

static int lua_u8g2_get_ascent (lua_State *L) {
    lua_pushinteger(L, u8g2_GetAscent(&u8g2));
    return 1;
}

static int lua_u8g2_get_descent (lua_State *L) {
    lua_pushinteger(L, u8g2_GetDescent(&u8g2));
    return 1;
}

static int lua_u8g2_get_str_width (lua_State *L) {
    const char *s = luaL_checkstring(L, 1);

    lua_pushinteger(L, u8g2_GetStrWidth(&u8g2, s));
    return 1;
}

static int lua_u8g2_get_utf8_width (lua_State *L) {
    const char *s = luaL_checkstring(L, 1);

    lua_pushinteger(L, u8g2_GetUTF8Width(&u8g2, s));
    return 1;
}

static int lua_u8g2_send_buf (lua_State *L) {
    u8g2_SendBuffer(&u8g2);

    return 0;
}

static int lua_u8g2_set_bitmap_mode (lua_State *L) {
    int is_transparent = luaL_checkinteger(L, 1);

    u8g2_SetBitmapMode(&u8g2, is_transparent);

    return 0;
}

static int lua_u8g2_set_contrast (lua_State *L) {
    int value = luaL_checkinteger(L, 1);

    u8g2_SetContrast(&u8g2, value);

    return 0;
}

static int lua_u8g2_set_display_rotation (lua_State *L) {
    const u8g2_cb_t *u8g2_cb = (u8g2_cb_t *)lua_touserdata(L, 1);

    u8g2_SetDisplayRotation(&u8g2, u8g2_cb);

    return 0;
}

static int lua_u8g2_set_draw_color (lua_State *L) {
    int col = luaL_checkinteger(L, 1);

    u8g2_SetDrawColor(&u8g2, col);

    return 0;
}

static int lua_u8g2_set_clip_window (lua_State *L) {
    int x0 = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y0 = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int x1 = luaL_checkinteger(L, 3) - LIB_POS_FIX;
    int y1 = luaL_checkinteger(L, 4) - LIB_POS_FIX;

    u8g2_SetClipWindow(&u8g2, x0, y0, x1, y1);

    return 0;
}

static int lua_u8g2_set_flip_mode (lua_State *L) {
    int is_enable = luaL_checkinteger(L, 1);

    u8g2_SetFlipMode(&u8g2, is_enable);

    return 0;
}

static int lua_u8g2_set_font_direction (lua_State *L) {
    int dir = luaL_checkinteger(L, 1);

    u8g2_SetFontDirection(&u8g2, dir);

    return 0;
}

static int lua_u8g2_set_font_mode (lua_State *L) {
    int is_transparent = luaL_checkinteger(L, 1);

    u8g2_SetFontMode(&u8g2, is_transparent);

    return 0;
}

static int lua_u8g2_set_font_pos_baseline (lua_State *L) {
    u8g2_SetFontPosBaseline(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_pos_bottom (lua_State *L) {
    u8g2_SetFontPosBottom(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_pos_top (lua_State *L) {
    u8g2_SetFontPosTop(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_pos_center (lua_State *L) {
    u8g2_SetFontPosCenter(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_ref_height_all (lua_State *L) {
    u8g2_SetFontRefHeightAll(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_ref_height_extended_text (lua_State *L) {
    u8g2_SetFontRefHeightExtendedText(&u8g2);
    return 0;
}

static int lua_u8g2_set_font_ref_height_text (lua_State *L) {
    u8g2_SetFontRefHeightText(&u8g2);
    return 0;
}

static int lua_u8g2_set_pwr_save (lua_State *L) {
    int is_enable = luaL_checkinteger(L, 1);

    u8g2_SetPowerSave(&u8g2, is_enable);

    return 0;
}

static int lua_u8g2_update (lua_State *L) {
    u8g2_UpdateDisplay(&u8g2);
    return 0;
}

static int lua_u8g2_update_area (lua_State *L) {
    int x = luaL_checkinteger(L, 1) - LIB_POS_FIX;
    int y = luaL_checkinteger(L, 2) - LIB_POS_FIX;
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);

    u8g2_UpdateDisplayArea(&u8g2, x, y, w, h);

    return 0;
}

typedef struct {
    const char *name;
    const uint8_t *data;
} font_item;

static font_item fonts[] = {
    {"u8g2_font_5x7_tf", u8g2_font_5x7_tf}
};

static int lua_u8g2_set_font (lua_State *L) {
    const char *font_name = luaL_checklstring(L, 1, NULL);

    for (int i = 0; i < sizeof(fonts)/sizeof(fonts[0]); i++) {
        if (strcmp(fonts[i].name, font_name) == 0) {
            u8g2_SetFont(&u8g2, fonts[i].data);
            return 0;
        }
    }

    luaL_argcheck(L, 0, 1, "invalid font");

    return 0;
}

static const luaL_Reg lcd_lib[] = {
    {"clean", lua_u8g2_clean},
    {"send_buf", lua_u8g2_send_buf},
    {"set_pwr_save", lua_u8g2_set_pwr_save},
    {"draw_box", lua_u8g2_draw_box},
    {"draw_circle", lua_u8g2_draw_circle},
    {"draw_disc", lua_u8g2_draw_disc},
    {"draw_ellipse", lua_u8g2_draw_ellipse},
    {"draw_filled_ellipse", lua_u8g2_draw_filled_ellipse},
    {"draw_frame", lua_u8g2_draw_frame},
    {"draw_glyph", lua_u8g2_draw_glyph},
    {"draw_h_line", lua_u8g2_draw_h_line},
    {"draw_line", lua_u8g2_draw_line},
    {"draw_pixel", lua_u8g2_draw_pixel},
    {"draw_r_box", lua_u8g2_draw_r_box},
    {"draw_r_frame", lua_u8g2_draw_r_frame},
    {"draw_str", lua_u8g2_draw_str},
    {"draw_triangle", lua_u8g2_draw_triangle},
    {"draw_utf8", lua_u8g2_draw_utf8},
    {"draw_v_line", lua_u8g2_draw_v_line},
    {"draw_xbm", lua_u8g2_draw_xbm},
    {"get_ascent", lua_u8g2_get_ascent},
    {"get_descent", lua_u8g2_get_descent},
    {"get_str_width", lua_u8g2_get_str_width},
    {"get_utf8_width", lua_u8g2_get_utf8_width},
    {"set_bitmap_mode", lua_u8g2_set_bitmap_mode},
    {"set_contrast", lua_u8g2_set_contrast},
    {"set_display_rotation", lua_u8g2_set_display_rotation},
    {"set_draw_color", lua_u8g2_set_draw_color},
    {"set_clip_window", lua_u8g2_set_clip_window},
    {"set_flip_mode", lua_u8g2_set_flip_mode},
    {"set_font", lua_u8g2_set_font},
    {"set_font_direction", lua_u8g2_set_font_direction},
    {"set_font_mode", lua_u8g2_set_font_mode},
    {"set_font_pos_baseline", lua_u8g2_set_font_pos_baseline},
    {"set_font_pos_bottom", lua_u8g2_set_font_pos_bottom},
    {"set_font_pos_top", lua_u8g2_set_font_pos_top},
    {"set_font_pos_center", lua_u8g2_set_font_pos_center},
    {"set_font_ref_height_all", lua_u8g2_set_font_ref_height_all},
    {"set_font_ref_height_extended_text", lua_u8g2_set_font_ref_height_extended_text},
    {"set_font_ref_height_text", lua_u8g2_set_font_ref_height_text},
    {"update", lua_u8g2_update},
    {"update_area", lua_u8g2_update_area},
    {NULL, NULL}
};

LUAMOD_API int luaopen_lcd (lua_State *L) {
    luaL_newlib(L, lcd_lib);
    return 1;
}
