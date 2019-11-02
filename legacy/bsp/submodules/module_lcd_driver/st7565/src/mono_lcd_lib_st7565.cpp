#include "mono_lcd_lib_st7565.h"

#ifdef MODULE_MONO_LCD_ST7565_ENABLED

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mono_lcd_lib_st7565_cmd.h"

namespace mono_lcd {

#define        checkResultAndBreak(baseResultVariable)                                        \
                if ( baseResultVariable != mc_interfaces::res::err_ok ) {        \
                    break;                                                                \
                }

st7565::st7565 (const st7565_cfg *const cfg, uint8_t *const buf) :
    cfg(cfg), userBuf(buf) {
    this->m = USER_OS_STATIC_MUTEX_CREATE(&this->mb);
}

mc_interfaces::res st7565::comOut (uint8_t command) {
    cfg->a0->reset();
    cfg->cs->reset();
    mc_interfaces::res r = this->cfg->s->tx(&command, 1, 100);
    cfg->cs->set();
    return r;
}

mc_interfaces::res st7565::dataOut (uint8_t data) {
    cfg->a0->set();
    cfg->cs->reset();
    mc_interfaces::res r = this->cfg->s->tx(&data, 1, 100);
    cfg->cs->set();
    return r;
}


mc_interfaces::res st7565::set_contrast (uint8_t val) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    
    do {
        r = this->comOut(CMD_SET_VOLUME_FIRST);
        checkResultAndBreak(r);
        r = this->comOut(CMD_SET_VOLUME_SECOND | (val & 0x3f));
    } while (false);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

mc_interfaces::res st7565::reset (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    
    do {
        cfg->cs->set();
        cfg->res->reset();
        USER_OS_DELAY_MS(5);
        cfg->res->set();
        USER_OS_DELAY_MS(5);
        
        // LCD bias select
        r = this->comOut(CMD_SET_BIAS_9);
        checkResultAndBreak(r);
        
        // ADC select
        r = this->comOut(CMD_SET_ADC_REVERSE);
        checkResultAndBreak(r);
        
        // SHL select
        r = this->comOut(CMD_SET_COM_NORMAL);
        checkResultAndBreak(r);
        
        // Initial display line
        r = this->comOut(CMD_SET_DISP_START_LINE);
        checkResultAndBreak(r);
        
        // turn on voltage converter (VC=1, VR=0, VF=0)
        r = this->comOut(CMD_SET_POWER_CONTROL | 0x4);
        checkResultAndBreak(r);
        USER_OS_DELAY_MS(5);
        
        // turn on voltage regulator (VC=1, VR=1, VF=0)
        r = this->comOut(CMD_SET_POWER_CONTROL | 0x6);
        checkResultAndBreak(r);
        USER_OS_DELAY_MS(5);
        
        // turn on voltage follower (VC=1, VR=1, VF=1)
        r = this->comOut(CMD_SET_POWER_CONTROL | 0x7);
        checkResultAndBreak(r);
        USER_OS_DELAY_MS(1);
        
        // set lcd operating voltage (regulator resistor, ref voltage resistor)
        r = this->comOut(CMD_SET_RESISTOR_RATIO | 0x6);
        checkResultAndBreak(r);
        
        r = this->comOut(CMD_SET_ALLPTS_NORMAL);
    } while (false);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

mc_interfaces::res st7565::on (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    r = this->comOut(CMD_DISPLAY_ON);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

mc_interfaces::res st7565::off (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    r = this->comOut(CMD_DISPLAY_OFF);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

mc_interfaces::res st7565::update (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    
    do {
        for (int page_l = 0; page_l < 8; page_l++) {
            r = this->comOut(CMD_SET_PAGE | page_l);
            checkResultAndBreak(r);
            r = this->comOut(CMD_SET_COLUMN_UPPER);
            checkResultAndBreak(r);
            
            cfg->a0->set();
            cfg->cs->reset();
            
            memset(this->lcdImage, 0, 128);
            
            for (uint32_t string_l = 0; string_l < 8; string_l++) {
                uint32_t us_p_string;
                if ((this->cfg->mode == st7565_mode::ivert_y) || (this->cfg->mode == st7565_mode::ivert_x_amd_y)) {
                    us_p_string = (7 - page_l) * 128 + (7 - string_l) * 16;
                } else {
                    us_p_string = page_l * 128 + string_l * 16;
                }
                for (uint32_t column_l = 0; column_l < 128; column_l++) {
                    switch ((uint32_t)this->cfg->mode) {
                        case (uint32_t)st7565_mode::standard:
                        case (uint32_t)st7565_mode::ivert_y:
                            this->lcdImage[column_l] |=
                                ((this->userBuf[us_p_string + column_l / 8] >> (column_l % 8)) & 1) << string_l;
                            break;
                        case (uint32_t)st7565_mode::ivert_x:
                        case (uint32_t)st7565_mode::ivert_x_amd_y:
                            this->lcdImage[127 - column_l] |=
                                ((this->userBuf[us_p_string + column_l / 8] >> (column_l % 8)) & 1) << string_l;
                            break;
                    }
                }
            }
            
            r = this->cfg->s->tx(this->lcdImage, 128, 100);
            checkResultAndBreak(r);
            
            cfg->cs->set();
        }
    } while (false);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

mc_interfaces::res st7565::lcd_clear (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    mc_interfaces::res r;
    
    do {
        uint8_t buf = 0;
        
        for (int p = 0; p < 8; p++) {
            r = this->comOut(CMD_SET_PAGE | p);
            checkResultAndBreak(r);
            r = this->comOut(CMD_SET_COLUMN_UPPER);
            checkResultAndBreak(r);
            
            cfg->a0->set();
            cfg->cs->reset();
            r = this->cfg->s->txOneItem(buf, 128, 100);
            checkResultAndBreak(r);
            cfg->cs->set();
        }
    } while (false);
    
    USER_OS_GIVE_MUTEX(this->m);
    
    return r;
}

void st7565::buf_clear (void) {
    USER_OS_TAKE_MUTEX(this->m, portMAX_DELAY);
    
    memset(this->userBuf, 0, 1024);
    
    USER_OS_GIVE_MUTEX(this->m);
}

}

#endif
