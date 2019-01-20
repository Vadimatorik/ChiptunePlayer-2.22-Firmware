#include "mc_hardware_interfaces_implementation_for_stm32_pwr.h"

McHardwareInterfacesImplementation::PwrCfg pwrCfg = {
	.cfg = {
		/// Min: 2.65; Typ: 2.84; Max: 3.02
		.PVDLevel		=	PWR_PVDLEVEL_5,
		.Mode			=	PWR_PVD_MODE_IT_RISING
	}
};

McHardwareInterfacesImplementation::Pwr	pwr( &pwrCfg );
