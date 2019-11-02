#include "pin_multifunc.h"

#ifdef HAL_GPIO_MODULE_ENABLED

namespace mc {

bool PinMultifunc::reinit (uint32_t cfgNumber) {
    if (cfgNumber >= this->cfgCount) return false;
    HAL_GPIO_DeInit((GPIO_TypeDef *)this->cfg->GPIOx, this->cfg->init.Pin);
    this->init();
    return true;
}

}

#endif
