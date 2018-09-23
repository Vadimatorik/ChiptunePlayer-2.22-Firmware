#include "../ayplayer_class/structs/ayplayer_struct_cfg.h"
#include "ayplayer_ay_file_class.h"
#include "run_time_logger.h"

extern const AyPlayerGuiModuleStyleCfg	ayGuiCfg;
extern const AyPlayerMcuCfg				ayMcuCfg;
extern const AyPlayerPcbStrcut			ayPcbCfg;

extern AyYmFilePlay				ayYmFilePlay;
extern RunTimeLogger			ayLog;

extern const AyPlayer::BaseCfg ayCfg = {
	.mcu								=	&ayMcuCfg,
	.pcb								=	&ayPcbCfg,
	.os									=	&osData,
	.gui								=	&ayGuiCfg,

	.l									=	&ayLog,
	.ay									=	&ayYmFilePlay
};
