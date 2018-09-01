#include "wdt.h"

const WdtCfg wdtCfg = {
	.taskPrio			= 1,
	.runTimeMs			= 1000,
	.startupTimeMs		= 5000,
	.serviceTimeMs		= 5000
};

Wdt wdt ( &wdtCfg );
