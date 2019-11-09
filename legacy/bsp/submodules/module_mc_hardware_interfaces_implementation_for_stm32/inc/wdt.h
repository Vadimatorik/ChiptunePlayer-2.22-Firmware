/*

@startuml

namespace mc {

mc.Wdt ..|> mc.Wdt

class Wdt {
	{field}-	const WdtCfg*					const cfg
	{field}-	const uint32_t					cfgCount
	{field}-	uint32_t						cfgNow
	{field}-	StackType_t		taskStack[ 64 ]
	{field}-	StaticTask_t	taskStruct
	{field}-	uint8_t						reboot
	__Constructor__
	{method}+	Wdt ( const WdtCfg*		const cfg,\n\tuint32_t			cfgCount = 1 )
	__Private methods__
	{method}{static}-	void	task	( void*	obj )
}

}

@enduml

*/

#pragma once

#ifdef __cplusplus

#include "platform.h"

#ifdef HAL_WWDG_MODULE_ENABLED

#include "mc_wdt.h"
#include "user_os.h"

namespace mc {

#define WDT_TASK_STACK_SIZE                        128

struct WdtCfg {
    const uint8_t taskPrio;                // Приоритет задачи, сбрасывающий wdt.
    const uint32_t runTimeMs;                // Время перезагрузки по сторожевому таймеру.
    // при номинальном режиме работы системы.
    const uint32_t startupTimeMs;            // Время перезагрузки по сторожевому таймеру при запуске системы.
    const uint32_t serviceTimeMs;            // Время перезагрузки по сторожевому таймеру
    // во время сервисных операций (например, стирание и перезапись flash).
};

class Wdt : public mc_interfaces::Wdt {
public:
    Wdt (const WdtCfg *const cfg,
         uint32_t cfgCount = 1)
        : cfg(cfg), cfgCount(cfgCount),
          cfgNow(0) {}
    
    mc_interfaces::res reinit (uint32_t numberCfg = 0);
    
    void reset (void);
    
    void resetService (void);

private:
    const WdtCfg *const cfg;
    const uint32_t cfgCount;
    
    uint32_t cfgNow;
    
    static void task (void *obj);
    
    StackType_t taskStack[WDT_TASK_STACK_SIZE];
    StaticTask_t taskStruct;
    
    uint8_t reboot;
};

}

#endif

#endif
