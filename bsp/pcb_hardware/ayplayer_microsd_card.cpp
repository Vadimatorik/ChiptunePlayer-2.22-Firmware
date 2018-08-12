#include "microsd_card_sdio.h"
#include "ayplayer_mc_hardware.h"

const MicrosdSdioCfg sdCfg = {
	.wide						= SDIO_BUS_WIDE_4B,
	.div						= 2,
	.dmaRx						= DMA2_Stream3,
	.dmaRxCh					= DMA_CHANNEL_4,
	.dmaRxIrqPrio				= 6,
	.dmaTx						= DMA2_Stream6,
	.dmaTxCh					= DMA_CHANNEL_4,
	.dmaTxIrqPrio				= 6,
	.sdioIrq					= 6
};

MicrosdSdio sd( &sdCfg );
