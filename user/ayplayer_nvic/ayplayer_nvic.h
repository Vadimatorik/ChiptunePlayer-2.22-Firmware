/*

@startuml

namespace AyPlayer {

class Nvic {
	{field}-	const uint32_t DMA1_Stream4_IRQn_priority
	{field}-	const uint32_t DMA2_Stream3_IRQn_priority
	{field}-	const uint32_t DMA2_Stream5_IRQn_priority
	{field}-	const uint32_t DMA2_Stream6_IRQn_priority
	{field}-	const uint32_t SDIO_IRQn_priority
	{field}-	const uint32_t USART1_IRQn_priority
	{field}-	const uint32_t PVD_IRQn_priority
	{field}-	const uint32_t TIM6_DAC_IRQn_priority
	__Constructor__
	{method}+	Nvic	( void )
	__Public methods__
	void	setPriorityCallsInterruptVectors	( void )
	void	enableAllIrq				( void )
	void	reboot					( void )
}

}

@enduml

 */

#pragma once

#include "platform.h"

namespace AyPlayer {

/*!
 *	\brief		Объект данного класса позволяет
 *				взаимодействовать с аппаратным
 *				блоком NVIC.
 */
class Nvic {
public:
	Nvic() {}

public:
	/*!
	 *	\brief		Метод устанавливает приоритеты для
	 *				всех используемых в приложении векторов
	 *				прерываний.
	 */
	void			setPriorityCallsInterruptVectors	(	void	);

	/*!
	 *	\brief		Метод разрешает возникновение прерываний
	 *				ото всех имеющихся в приложении векторов
	 *				прерываний.
	 */
	void			enableAllIrq						(	void	);

	/*!
	 *	\brief		Метод вызывает перезагрузку процессора.
	 */
	static	void	reboot							(	void	);

private:
	const uint32_t DMA1_Stream4_IRQn_priority			=	10;

	const uint32_t DMA2_Stream3_IRQn_priority			=	10;
	const uint32_t DMA2_Stream5_IRQn_priority			=	10;
	const uint32_t DMA2_Stream6_IRQn_priority			=	10;

	const uint32_t SDIO_IRQn_priority					=	10;
	const uint32_t USART1_IRQn_priority					=	10;
	const uint32_t PVD_IRQn_priority					=	10;
	const uint32_t TIM6_DAC_IRQn_priority				=	10;
};

}
