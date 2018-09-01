#include "pin.h"
#include "port.h"

/// ADC.
extern const PinCfg adcCfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_0 |					/// Bat		*	0.5.
						GPIO_PIN_1 |					/// Left	*	0.5.
						GPIO_PIN_2,						/// Right	*	0.5.
		PINS_ADC_MODE
	}
};

/// Качелька громкости.
extern const PinCfg buttonIncCfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_7,
		PINS_EXTI_MODE
	}
};

extern const PinCfg buttonDecCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_9,
		PINS_EXTI_MODE
	}
};

/// LCD.
extern const PinCfg lcdClkCfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_5 | GPIO_PIN_7,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF5_SPI1
	}
};

extern const PinCfg lcdPwmCfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_8,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF1_TIM1
	}
};

extern const PinCfg lcdResCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_2,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg lcdDcCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_0,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg lcdCsCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_1,
		PINS_OUTPUT_PP_MODE
	}
};

/// SDIO.
extern const PinCfg sd1SmdCfg = {
	.GPIOx			=	GPIOD,
	.init = {
		.Pin		=	GPIO_PIN_2,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF12_SDIO
	}
};

extern const PinCfg sdIoAndClkCfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF12_SDIO
	}
};

extern const PinCfg sdPushCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_11,
		PINS_EXTI_MODE
	}
};

extern const PinCfg sdCheckerValueSetter = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_12,
		PINS_OUTPUT_PP_MODE
	}
};

/// USB.
extern const PinCfg usbCfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF10_OTG_FS
	}
};

/// BOARD.
extern const PinCfg spiBoardTxCfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_3,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_MEDIUM,
		.Alternate	=	GPIO_AF6_SPI3
	}
};

extern const PinCfg spiBoardClkCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_10,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_NOPULL,
		.Speed		=	GPIO_SPEED_FREQ_MEDIUM,
		.Alternate	=	GPIO_AF6_SPI3
	}
};

extern const PinCfg boardGpio1 = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_0,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg boardGpio2 = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_1,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg boardGpio3 = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_2,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg boardGpio4 = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_4,
		PINS_OUTPUT_PP_MODE
	}
};

/// JTAG.
extern const PinCfg jtagPart1Cfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_PULLUP,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF0_SWJ
	}
};

extern const PinCfg jtagPart2Cfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_3 | GPIO_PIN_4,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_PULLUP,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF0_SWJ
	}
};

/// PWR.
extern const PinCfg pwr5vOnCfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_3,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg pwrOnCfg = {								/// Подача напряжения на всю плату
	.GPIOx			=	GPIOB,							/// (может подаваться при нажатии на физическую
	.init = {											/// клавишу включения).
		.Pin		=	GPIO_PIN_5,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg tpStCfg = {								/// Флаг окончания зарядки аккумулятора.
	.GPIOx			=	GPIOC,							/// Подтягивается к земле по окончании зарядки.
	.init = {											/// В противном случае находится в
		.Pin		=	GPIO_PIN_13,					/// высокоимпедансном состоянии.
		PINS_INPUT_MODE
	}
};

extern const PinCfg tpChCfg = {								/// Флаг того, что аккумулятор заряжается.
	.GPIOx			=	GPIOB,							/// Подтягивается к земле на время зарядки.
	.init = {											/// В противном случае находится в высокоимпедансном состоянии.
		.Pin		=	GPIO_PIN_8,
		PINS_INPUT_MODE
	}
};

/// DEBUG_UART
extern const PinCfg uartDebugCfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_6 | GPIO_PIN_7,
		.Mode		=	GPIO_MODE_AF_PP,
		.Pull		=	GPIO_PULLUP,
		.Speed		=	GPIO_SPEED_FREQ_VERY_HIGH,
		.Alternate	=	GPIO_AF7_USART1
	}
};

/// Keyboard.
extern const PinCfg br0Cfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_6,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg br1Cfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_15,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg br2Cfg = {
	.GPIOx			=	GPIOB,
	.init = {
		.Pin		=	GPIO_PIN_14,
		PINS_OUTPUT_PP_MODE
	}
};

extern const PinCfg bc0Cfg = {
	.GPIOx			=	GPIOA,
	.init = {
		.Pin		=	GPIO_PIN_6,
		PINS_INPUT_MODE
	}
};

extern const PinCfg bc1Cfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_4,
		PINS_INPUT_MODE
	}
};

extern const PinCfg bc2Cfg = {
	.GPIOx			=	GPIOC,
	.init = {
		.Pin		=	GPIO_PIN_5,
		PINS_INPUT_MODE
	}
};
