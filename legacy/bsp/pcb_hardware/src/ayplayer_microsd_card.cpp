#include "microsd_card_sdio.h"
#include "ayplayer_mc_hardware.h"

const MicrosdSdioCfg sdCfg = {
    .wide                        = SDIO_BUS_WIDE_4B,
    .div                        = 0,
    .dmaRx                        = DMA2_Stream3,
    .dmaRxCh                    = DMA_CHANNEL_4,
    .dmaRxIrqPrio                = 6
};

MicrosdSdio sd(&sdCfg);
