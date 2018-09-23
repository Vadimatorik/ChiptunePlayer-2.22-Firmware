#pragma once

#include "nvic.h"
#include <memory>
#include "../structs/ayplayer_struct_gui_cfg.h"
#include "../structs/ayplayer_struct_pcb_cfg.h"

#define TB_ILLUMINATION_CONTROL_TASK_SIZE			200

namespace AyPlayer {

class WindowMessage {
public:
	WindowMessage (	const char*								const message,
					const MakiseStyle_SMessageWindow*		style,
					MContainer*								c,
					int32_t									x,
					int32_t									y,
					uint32_t								w,
					uint32_t								h			);

	~WindowMessage();

private:
	MMessageWindow					mw;
	char*							str;

};



class StatusBar {
public:
	StatusBar	(	const MakiseStyle_SMPlayerStatusBar*	style,
					const MPlayerStatusBar_CallbackFunc* 	callbacks,
					MContainer*								c,
					int32_t									x,
					int32_t									y,
					uint32_t								w,
					uint32_t								h			);

	~StatusBar();

private:
	MPlayerStatusBar		sb;

};


class WindowIndexingSupportedFiles {
public:
	WindowIndexingSupportedFiles	(	const MakiseStyle_SList*		style,
										const MakiseStyle_SListItem*	itemsStyle,
										MContainer*						c,
										int32_t							x,
										int32_t							y,
										uint32_t						w,
										uint32_t						h				);

	void setActualState ( const char* const stateMessage );
	void addItem ( const char* const itemName );
	~WindowIndexingSupportedFiles();

private:
	const uint32_t cout	=	4;

private:
	MSList					sl;
	MSList_Item				slItem[ 4 ];
	char*					str;

};


class Gui {
public:
	Gui	(	const AyPlayerPcbStrcut*				const pcbObj,
			const AyPlayerGuiModuleStyleCfg*		const cfg,
			McHardwareInterfaces::TimPwmOneChannel*	ledPwm );

public:
	void	init						(	void	);

	void	setMaxIlluminationDuty		(	float			maxIlluminationDuty	);
	void	setMinIlluminationDuty		(	float			minIlluminationDuty	);
	void	setMaxIlluminationTime		(	uint32_t		maxIlluminationTimeS	);
	void	setMinIlluminationTime		(	uint32_t		minIlluminationTimeS	);

	/// Перерисовывает GUI и обновляет экран.
	void	update				(	void	);

	/// Создает окно поверх остальных с каким-то уведомлением.
	/// Когда удалится объект - удалится и уведомление.
	WindowMessage*	addWindowMessage					(	const char* const message	);

	StatusBar*						addStatusBar						(	void	);
	WindowIndexingSupportedFiles*	addWindowIndexingSupportedFiles		(	void	);


private:
	/*!
	 *	\brief		Метод проверяет удачность выполнения
	 *				предыдущей операции и в случае провала
	 *				перезагружает контроллер.
	 */
	void	checkAndExit			(	McHardwareInterfaces::BaseResult	resultValue	);

private:
	static	void	illuminationControlTask		(	void*	obj	);

private:
	const AyPlayerPcbStrcut*				const pcbObj;
	const AyPlayerGuiModuleStyleCfg*		const cfg;
	McHardwareInterfaces::TimPwmOneChannel*	ledPwm;
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
	StatusBar*								statusBar;

private:
	const uint32_t	tbIlluminationControlTaskSize			=	TB_ILLUMINATION_CONTROL_TASK_SIZE;
	const uint32_t	illuminationControlTaskPrio				=	1;

};

}
