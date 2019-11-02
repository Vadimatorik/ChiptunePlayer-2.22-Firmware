#pragma once

#include "nvic.h"
#include <memory>
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"
#include "gui_element_status_bar.h"
#include "gui_element_window_indexing_supported_files.h"
#include "gui_element_window_message.h"
#include "gui_element_horizontal_list.h"
#include "gui_element_play_bar.h"
#include "ayplayer_struct_fat_cfg.h"

#define TB_ILLUMINATION_CONTROL_TASK_SIZE			200

namespace AyPlayer {

class Gui {
public:
	Gui	(	const AyPlayerPcbStrcut*				const pcbObj,
			const AyPlayerGuiModuleStyleCfg*		const cfg,
			mc_interfaces::TimPwmOneChannel*	ledPwm );


public:
	void	init						(	void	);

public:
	void	setMaxIlluminationDuty		(	float			maxIlluminationDuty	);
	void	setMinIlluminationDuty		(	float			minIlluminationDuty	);
	void	setMaxIlluminationTime		(	uint32_t		maxIlluminationTimeS	);
	void	setMinIlluminationTime		(	uint32_t		minIlluminationTimeS	);

public:
	void	setWindowMain				(	std::shared_ptr< ItemFileInFat >	item,
											uint32_t							countItems	);
	void	resetPlayBarInMainWindow	(	void	);
	void	incTickPlayBarInMainWindow	(	void	);
	void	updateTreckWindowMain		(	std::shared_ptr< ItemFileInFat >	item	);


	void	setWindowIndexingSupportedFiles		(	WindowIndexingSupportedFiles**		returnWisfObj	);

public:
	void	update				(	void	);
	void	updateWithoutLed	(	void	);

public:
	/// Создает окно поверх остальных с каким-то уведомлением.
	/// Когда удалится объект - удалится и уведомление.
	WindowMessage*					addWindowMessage					(	const char* const message	);

private:
	static	void	timerHandler				(	TimerHandle_t		timer		);

private:
	HorizontalList* addHorizontalList ( void );
	PlayBar* addPlayBar ( void );

private:
	StatusBar*						addStatusBar						(	void	);

	void	removeOldWindow	( void );
private:
	/*!
	 *	\brief		Метод проверяет удачность выполнения
	 *				предыдущей операции и в случае провала
	 *				перезагружает контроллер.
	 */
	void	checkAndExit			(	mc_interfaces::res	resultValue	);

private:
	static	void	illuminationControlTask		(	void*	obj	);

private:
	const AyPlayerPcbStrcut*				const pcbObj;
	const AyPlayerGuiModuleStyleCfg*		const cfg;
	mc_interfaces::TimPwmOneChannel*	ledPwm;
	USER_OS_STATIC_MUTEX_BUFFER				mbHost;
	USER_OS_STATIC_MUTEX					mHost;
	USER_OS_STATIC_STACK_TYPE				tbIlluminationControlTask[ TB_ILLUMINATION_CONTROL_TASK_SIZE ];
	USER_OS_STATIC_TASK_STRUCT_TYPE			tsIlluminationControlTask;
	USER_OS_STATIC_BIN_SEMAPHORE			sUpdateLcd;
	USER_OS_STATIC_BIN_SEMAPHORE_BUFFER		sbUpdateLcd;
	float									maxIlluminationDuty;
	float									minIlluminationDuty;
	uint32_t								maxIlluminationTimeS;
	uint32_t								minIlluminationTimeS;

private:
	const uint32_t	tbIlluminationControlTaskSize			=	TB_ILLUMINATION_CONTROL_TASK_SIZE;
	const uint32_t	illuminationControlTaskPrio				=	1;

private:
	StatusBar*							statusBar	=	nullptr;
	PlayBar*							pb			=	nullptr;
	HorizontalList*						shl			=	nullptr;
	WindowIndexingSupportedFiles*		wisf		=	nullptr;

private:
	uint32_t							timerSpeedLow			=	1000;
	uint32_t							timerSpeedFast			=	200;
	USER_OS_TIMER						timStringScroll;
	USER_OS_TIMER_STATIC_STRUCT			timStStringScroll;

};

}
