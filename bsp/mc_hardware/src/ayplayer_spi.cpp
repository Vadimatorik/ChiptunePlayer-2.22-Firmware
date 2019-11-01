#include "ayplayer_mc_hardware.h"
#include "ayplayer_clock.h"

uint32_t baudratePrescalerLcdSpiCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_2
};

/// Конфигурация SPI для работы с LCD
mc::SpiMaster8BitCfg lcdSpiCfg = {
	.SPIx						=	SPI1,

	.pinCs						=	nullptr,

	.clkPolarity				=	SPI_POLARITY_HIGH,
	.clkPhase					=	SPI_PHASE_2EDGE,

	.baudratePrescalerArray		=	baudratePrescalerLcdSpiCfg,
	.numberBaudratePrescalerCfg	=	AYPLAYER_RCC_CFG_COUNT,

	.dmaTx						=	DMA2_Stream5,
	.dmaRx						=	nullptr,
	.dmaTxCh					=	DMA_CHANNEL_3,
	.dmaRxCh					=	0
};

mc::SpiMaster8Bit lcdSpi( &lcdSpiCfg );

uint32_t baudratePrescalerBoardSpiCfg[ AYPLAYER_RCC_CFG_COUNT ] = {
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_2,
	SPI_BAUDRATEPRESCALER_4,
	SPI_BAUDRATEPRESCALER_8
};

/// Board SPI.
mc::SpiMaster8BitCfg boardSpiCfg = {
	.SPIx						=	SPI2,

	.pinCs						=	nullptr,

	.clkPolarity				=	SPI_POLARITY_LOW,
	.clkPhase					=	SPI_PHASE_1EDGE,

	.baudratePrescalerArray		=	baudratePrescalerBoardSpiCfg,
	.numberBaudratePrescalerCfg	=	AYPLAYER_RCC_CFG_COUNT,

	.dmaTx						=	DMA1_Stream4,
	.dmaRx						=	nullptr,
	.dmaTxCh					=	DMA_CHANNEL_0,
	.dmaRxCh					=	0
};

mc::SpiMaster8Bit boardSpi( &boardSpiCfg );
