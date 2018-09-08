#pragma once

#include "ayplayer_struct_fat_cfg.h"
#include "ayplayer_struct_gui_cfg.h"
#include "ayplayer_struct_mcu_cfg.h"
#include "ayplayer_struct_pcb_cfg.h"

#include "ayplayer_ay_file_class.h"

#include "run_time_logger.h"
#include "ayplayer_os_object.h"

struct AyPlayerCfg {
	const AyPlayerMcuCfg*					const mcu;
	const AyPlayerPcbStrcut*				const pcb;
	FreeRtosObj*							const os;
	const AyPlayerGuiModuleStyleCfg*		const gui;

	RunTimeLogger*							const l;
	AyYmFilePlay*							const ay;
};
