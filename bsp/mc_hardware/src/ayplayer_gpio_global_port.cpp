#include "ayplayer_mc_hardware.h"

extern const McHardwareInterfacesImplementation::PinCfg gpCfg[ 35 ];

McHardwareInterfacesImplementation::PortGlobal gp( gpCfg, M_SIZE_ARRAY( gpCfg ) );
