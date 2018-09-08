#include "ayplayer_nvic.h"

namespace AyPlayer {

void Nvic::setPriorityCallsInterruptVectors (	void	) {
	NVIC_SetPriority( DMA2_Stream5_IRQn,		this->DMA2_Stream5_IRQn_priority );
	NVIC_SetPriority( DMA1_Stream4_IRQn,		this->DMA1_Stream4_IRQn_priority );

	NVIC_SetPriority( DMA2_Stream3_IRQn,		this->DMA2_Stream3_IRQn_priority );
	NVIC_SetPriority( DMA2_Stream6_IRQn,		this->DMA2_Stream6_IRQn_priority );
	NVIC_SetPriority( SDIO_IRQn,				this->SDIO_IRQn_priority );

	NVIC_SetPriority( USART1_IRQn,				this->USART1_IRQn_priority );

	NVIC_SetPriority( PVD_IRQn,					this->PVD_IRQn_priority );

	NVIC_SetPriority( TIM6_DAC_IRQn,			this->TIM6_DAC_IRQn_priority );
}

void Nvic::enableAllIrq ( void ) {
	NVIC_EnableIRQ( DMA2_Stream5_IRQn );
	NVIC_EnableIRQ( DMA1_Stream4_IRQn );

	NVIC_EnableIRQ( DMA2_Stream3_IRQn );
	NVIC_EnableIRQ( DMA2_Stream6_IRQn );
	NVIC_EnableIRQ( SDIO_IRQn );

	NVIC_EnableIRQ( USART1_IRQn );

	NVIC_EnableIRQ( PVD_IRQn );

	NVIC_EnableIRQ( TIM6_DAC_IRQn );
}

void Nvic::reboot ( void ) {
	NVIC_SystemReset();
}

}
