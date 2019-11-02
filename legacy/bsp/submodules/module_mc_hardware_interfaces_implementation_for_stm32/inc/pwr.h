/*

@startuml

namespace mc {

mc.Pwr ..|> mc.Pwr

class Pwr {
	{field}-	const PwrCfg*		const cfg
	{field}-	const uint32_t		cfgCount
	{field}-	uint32_t			cfgNow
	__Constructor__
	{method}+	Pwr ( const PwrCfg*	const cfg,\n\tuint32_t		cfgCount = 1 )
	__Public methods__
	{method}+	void	irq_handler		( void )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_PWR_MODULE_ENABLED

#include "mc_pwr.h"

namespace mc {

struct PwrCfg {
    const PWR_PVDTypeDef cfg;
};

class Pwr : public mc_interfaces::Pwr {
public:
    Pwr (const PwrCfg *const cfg,
         uint32_t cfgCount = 1)
        : cfg(cfg), cfgCount(cfgCount) {}
    
    mc_interfaces::res reinit (uint32_t cfgNumber = 0);
    
    void pvdEnable (void);
    
    void pvdDisable (void);
    
    void irq_handler (void);

private:
    const PwrCfg *const cfg;
    const uint32_t cfgCount;
    
    uint32_t cfgNow = 0;
};

}

#endif

#endif
