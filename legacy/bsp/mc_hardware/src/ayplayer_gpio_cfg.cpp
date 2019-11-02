extern const mc::PinCfg pwrOnCfg = {								/// Подача напряжения на всю плату
	.GPIOx			=	GPIOB,							/// (может подаваться при нажатии на физическую
	.init = {											/// клавишу включения).
		.Pin		=	GPIO_PIN_5,
		PINS_OUTPUT_PP_MODE
	}
};

extern const mc::PinCfg tpStCfg = {								/// Флаг окончания зарядки аккумулятора.
	.GPIOx			=	GPIOC,							/// Подтягивается к земле по окончании зарядки.
	.init = {											/// В противном случае находится в
		.Pin		=	GPIO_PIN_13,					/// высокоимпедансном состоянии.
		PINS_INPUT_MODE
	}
};

extern const mc::PinCfg tpChCfg = {		/// Флаг того, что аккумулятор заряжается.
	.GPIOx			=	GPIOB,											/// Подтягивается к земле на время зарядки.
	.init = {															/// В противном случае находится в высокоимпедансном состоянии.
		.Pin		=	GPIO_PIN_8,
		PINS_INPUT_MODE
	}
}