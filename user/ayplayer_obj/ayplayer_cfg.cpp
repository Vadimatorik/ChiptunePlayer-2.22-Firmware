#include "ayplayer_struct_cfg.h"

AyPlayerCfg ayCfg = {
	.mcu								=	&ayMcuCfg,
	.pcb								=	&ayPcbCfg,
	.os									=	&osData,
	.gui								=	&ayGuiCfg,

	.l									=	&ayLog,
	.ay									=	&ayYmFilePlay
};
