#pragma once

#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"
#include "ayplayer_nvic.h"

#define TB_ILLUMINATION_CONTROL_TASK_SIZE			200

namespace AyPlayer {

class Gui {
public:
	Gui	(	const AyPlayerPcbStrcut*				const pcbObj,
			const AyPlayerGuiModuleStyleCfg*		const cfg,
			McHardwareInterfaces::TimPwmOneChannel*	ledPwm );

public:
	void	init						(	void	);

	void	setMaxIlluminationDuty		(	float	maxIlluminationDuty	);
	void	setMinIlluminationDuty		(	float	minIlluminationDuty	);
	void	setMaxIlluminationTime		(	uint32_t		maxIlluminationTimeDs	);
	void	setMinIlluminationTime		(	uint32_t		minIlluminationTimeDs	);

	// Перерисовывает GUI и обновляет экран.
	void	update				(	void	);

private:
	/*!
	 *	\brief		Метод проверяет удачность выполнения
	 *				предыдущей операции и в случае провала
	 *				перезагружает контроллер.
	 */
	void	checkAndExit			(	McHardwareInterfaces::BaseResult	resultValue	);

	void	addStatusBar			(	void	);

private:
	static	void	illuminationControlTask		(	void*	obj	);

private:
	const AyPlayerPcbStrcut*				const pcbObj;
	AyPlayerModuleGui						g;
	const AyPlayerGuiModuleStyleCfg*		const cfg;
	McHardwareInterfaces::TimPwmOneChannel*	ledPwm;
	USER_OS_STATIC_MUTEX_BUFFER				mbHost;
	USER_OS_STATIC_MUTEX					mHost;
	USER_OS_STATIC_STACK_TYPE				tbIlluminationControlTask[ TB_ILLUMINATION_CONTROL_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE			tsIlluminationControlTask;
	float									maxIlluminationDuty;
	float									minIlluminationDuty;
	uint32_t								maxIlluminationTimeDs;
	uint32_t								minIlluminationTimeDs;

private:
	const uint32_t	tbIlluminationControlTaskSize			=	TB_ILLUMINATION_CONTROL_TASK_SIZE;
	const uint32_t	illuminationControlTaskPrio				=	1;

};

}
