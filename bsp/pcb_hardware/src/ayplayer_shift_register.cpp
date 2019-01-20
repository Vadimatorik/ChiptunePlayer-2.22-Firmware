#include "shift_register_base.h"

#include "ayplayer_os_object.h"
#include "ayplayer_mc_hardware.h"

uint8_t boardShiftRegistersData[3];

const ShiftRegister::BaseStaticCfg boardShiftRegistersCfg = {
    .st                =    &shStrob,
    .dataArray        =    boardShiftRegistersData,
    .arraySize        =    sizeof(boardShiftRegistersData),
    .strobActive    =    true,
    .spiObj            =    &boardSpi,
    .mutex            =    &osData.mBoardSpi
};

ShiftRegister::Base boardShiftRegisters(&boardShiftRegistersCfg);
