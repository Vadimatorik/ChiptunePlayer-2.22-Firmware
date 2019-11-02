#include "port_global.h"

#ifdef HAL_GPIO_MODULE_ENABLED

namespace mc {

extern void gpioClkEn (const GPIO_TypeDef *GPIOx);

mc_interfaces::res PortGlobal::reinitAllPorts (void) {
    for (uint32_t l = 0; l < this->cfgCount; l++) {
        gpioClkEn((const GPIO_TypeDef *)this->cfg[l].GPIOx);
        HAL_GPIO_Init((GPIO_TypeDef *)cfg[l].GPIOx, (GPIO_InitTypeDef *)(&cfg[l].init));
    }
    return mc_interfaces::res::err_ok;
}

}

#endif
