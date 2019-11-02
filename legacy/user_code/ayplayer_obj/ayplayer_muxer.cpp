#include "ayplayer_struct_cfg.h"

AyPlayer::MuxerCfg muxerCfg = {
	.dp							=	&soundDp,
	.left						=	{
		.chipNumber				=	0,
		.channelNumber			=	2,
		.valueDefault			=	0xFF / 2
	},
	.right						=	{
		.chipNumber				=	0,
		.channelNumber			=	3,
		.valueDefault			=	0xFF / 2
	},
	.A1							=	{
		.chipNumber				=	1,
		.channelNumber			=	0,
		.valueDefault			=	0xFF / 2
	},
	.B1							=	{
		.chipNumber				=	0,
		.channelNumber			=	0,
		.valueDefault			=	0xFF / 2
	},
	.C1							=	{
		.chipNumber				=	0,
		.channelNumber			=	1,
		.valueDefault			=	0xFF / 2
	},
	.A2							=	{
		.chipNumber				=	1,
		.channelNumber			=	1,
		.valueDefault			=	0xFF / 2
	},
	.B2							=	{
		.chipNumber				=	1,
		.channelNumber			=	2,
		.valueDefault			=	0xFF / 2
	},
	.C2							=	{
		.chipNumber				=	1,
		.channelNumber			=	3,
		.valueDefault			=	0xFF / 2
	},
};

AyPlayer::Muxer muxer( &muxerCfg );

