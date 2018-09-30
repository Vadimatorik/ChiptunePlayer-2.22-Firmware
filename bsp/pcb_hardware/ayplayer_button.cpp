#include "ayplayer_button.h"

#include "buttons_core.h"
#include "user_os.h"
#include "ayplayer_os_object.h"
#include "ayplayer_mc_hardware.h"

bool getKey ( uint8_t id ) {
	keyboardStrings[ 0 ].set();
	keyboardStrings[ 1 ].set();
	keyboardStrings[ 2 ].set();

	switch( id ) {
	case 0:
		keyboardStrings[ 0 ].reset();	return !keyboardColumns[ 1 ].read();
	case 1:
		keyboardStrings[ 2 ].reset();	return !keyboardColumns[ 1 ].read();
	case 2:
		keyboardStrings[ 1 ].reset();	return !keyboardColumns[ 0 ].read();
	case 3:
		keyboardStrings[ 1 ].reset();	return !keyboardColumns[ 2 ].read();
	case 4:
		keyboardStrings[ 1 ].reset();	return !keyboardColumns[ 1 ].read();
	case 5:
		keyboardStrings[ 0 ].reset();	return !keyboardColumns[ 2 ].read();
	case 6:
		keyboardStrings[ 0 ].reset();	return !keyboardColumns[ 0 ].read();
	}

	return false;
}

// При опуске коротком или длинном - одна и та же очередь.
Button::OneButtonCfg cfgButtons[ 7 ] = {
	{
		.id							=	0,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::UP_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::UP_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::UP_LONG_CLICK )
		}
	},
	{
		.id							=	1,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::DOWN_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::DOWN_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::DOWN_LONG_CLICK )
		}
	},
	{
		.id							=	2,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::LEFT_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::LEFT_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::LEFT_LONG_CLICK )
		}
	},
	{
		.id							=	3,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::RIGHT_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::RIGHT_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::RIGHT_LONG_CLICK )
		}
	},
	{
		.id							=	4,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::ENTER_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::ENTER_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::ENTER_LONG_CLICK )
		}
	},
	{
		.id							=	5,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::BACK_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::BACK_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::BACK_LONG_CLICK )
		}
	},
	{
		.id							=	6,
		.stabilizationTimeMs		=	50,
		.longPressDetectionTimeS	=	1,
		.press						=	{ nullptr, nullptr, 0 },
		.click						=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::MENU_CLICK )
		},
		.longPress					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::MENU_LONG_PRESS )
		},
		.longClick					=	{
			.s						=	nullptr,
			.q						=	&osData.qAyButton,
			.v						=	M_EC_TO_U8( EC_BUTTON_NAME::MENU_LONG_CLICK )
		}
	}
};

Button::BaseCfg cfgButton = {
	.taskDelayMs					=	10,
	.taskPrio						=	3,
	.cfgArray						=	cfgButtons,
	.cfgArraySize					=	7,
	.getButtonState					=	getKey
};

Button::Base buttons( &cfgButton );
