#include "wdt.h"

const McHardwareInterfacesImplementation::WdtCfg wdtCfg = {
	.taskPrio			= 1,
	.runTimeMs			= 1000,
	.startupTimeMs		= 5000,
	.serviceTimeMs		= 5000
};

McHardwareInterfacesImplementation::Wdt wdt ( &wdtCfg );
