#include "wdt.h"

const mc::WdtCfg wdtCfg = {
	.taskPrio			= 1,
	.runTimeMs			= 1000,
	.startupTimeMs		= 5000,
	.serviceTimeMs		= 5000
};

mc::Wdt wdt ( &wdtCfg );
