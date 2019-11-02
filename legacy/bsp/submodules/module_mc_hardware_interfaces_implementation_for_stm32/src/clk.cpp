#include "clk.h"

#ifdef HAL_RCC_MODULE_ENABLED

namespace mc {

mc_interfaces::RccResult Rcc::setCfg (const uint32_t numberCfg) {
    if (numberCfg >= this->cfgCount)
        return mc_interfaces::RccResult::errCfgNumber;
    
    HAL_RCC_DeInit();
    
    if (HAL_RCC_OscConfig((RCC_OscInitTypeDef *)&this->cfg[numberCfg].osc) != HAL_OK) {
        this->cfgNumberSet = -1;
        return mc_interfaces::RccResult::errOscInit;
    }
    
    if (HAL_RCC_ClockConfig((RCC_ClkInitTypeDef *)&this->cfg[numberCfg].clk,
                            this->cfg[numberCfg].fLatency) != HAL_OK) {
        this->cfgNumberSet = -1;
        return mc_interfaces::RccResult::errClkInit;
    }
    
    this->cfgNumberSet = static_cast< int >( numberCfg );
    return mc_interfaces::RccResult::ok;
}

mc_interfaces::RccResult Rcc::getCfgNumber (uint32_t &cfgNumber) {
    if (this->cfgNumberSet == -1)
        return mc_interfaces::RccResult::errNoInit;
    
    cfgNumber = static_cast< int >( this->cfgNumberSet );
    
    return mc_interfaces::RccResult::ok;
}

}

#endif
