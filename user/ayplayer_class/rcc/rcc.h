#pragma once

#include "ayplayer_struct_mcu_cfg.h"
#include "ayplayer_clock.h"

namespace AyPlayer {

class Rcc {
public:
	Rcc	(	const AyPlayerMcuCfg*	const mcuCfg	);

public:
	/*!
	 *	\brief		Метод инициализирует RCC а самой
	 *				высокоскоростной конфигурации.
	 *
	 *	\return		0	- если инициализация прошла успешно.
	 *				-1	- в противном случае.
	 */
	McHardwareInterfaces::BaseResult	initRccFrequancyMax		(	void	);

private:
	/*!
	 *	\brief		Останавливает все аппаратные модули, зависящие
	 *				от тактового сигнала, пытается установить заданную
	 *				тактовую частоту на шинах, после чего конфигурирует
	 *				заново все объекты, зависящие от частоты тактового
	 *				сигнала ( вызывается метод offDependingRccObj ).
	 *				Замечание: в случае, если не удалось настроить
	 *				заданную конфигурацию - микроконтроллер переходит
	 *				в режим работы по умолчанию.
	 */
	McHardwareInterfaces::RccResult		setCfg					(	uint32_t	cfgNumber	);

	/*!
	 *	\brief		Метод останавливает работу всех аппаратных блоков,
	 *				зависящих от RCC.
	 */
	void								offObjDependingRcc		(	void	);

	/*!
	 *	\brief		Метод переинициализирует заново все аппаратные
	 *				блоки, работа которых зависит от RCC.
	 */
	void								reinitObjDependingRcc	(	void	);

	/*!
	 *	\brief		Метод запускает базовые интерфейсы
	 *				(заранее инициализированные).
	 */
	void								startBaseInterfaces		(	void	);

	/*!
	 *	\brief		Метод проверяет удачность выполнения
	 *				предыдущей операции и в случае провала
	 *				перезагружает контроллер.
	 */
	void								checkAndExit			(	McHardwareInterfaces::BaseResult	resultValue	);

private:
	const AyPlayerMcuCfg*			const mcuCfg;

	/*!
	 *	\brief		Текущий режим работы RCC.
	 */
	uint32_t						rccIndex = 0;

};

}
