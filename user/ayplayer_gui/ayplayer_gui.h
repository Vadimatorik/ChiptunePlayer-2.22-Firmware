#pragma once

#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"
#include "ayplayer_nvic.h"

namespace AyPlayer {

class Gui {
public:
	Gui	(	const AyPlayerPcbStrcut*				const pcbObj,
			const AyPlayerGuiModuleStyleCfg*		const cfg	);

public:
	void	init				(	void	);

private:
	/*!
	 *	\brief		Метод проверяет удачность выполнения
	 *				предыдущей операции и в случае провала
	 *				перезагружает контроллер.
	 */
	void	checkAndExit			(	McHardwareInterfaces::BaseResult	resultValue	);

	void	addStatusBar			(	void	);

private:
	const AyPlayerPcbStrcut*				const pcbObj;
	AyPlayerModuleGui						g;
	const AyPlayerGuiModuleStyleCfg*		const cfg;

};

}
