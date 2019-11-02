#include "pin_multifunc_it.h"

#ifdef HAL_GPIO_MODULE_ENABLED

namespace mc {

bool PinMultifuncIt::checkIt (void) {
    if (__HAL_GPIO_EXTI_GET_IT(this->exitPin) != RESET) {
        return true;
    } else {
        return false;
    }
}

void PinMultifuncIt::clearIt (void) {
    __HAL_GPIO_EXTI_CLEAR_IT(this->exitPin);
}

}

#endif
