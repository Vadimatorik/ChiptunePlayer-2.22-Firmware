#include "port.h"
#include "pin.h"
#include "user_os.h"

extern const pinCfg gpCfg[ 35 ];

GlobalPort gp( gpCfg, M_SIZE_ARRAY( gpCfg ) );
