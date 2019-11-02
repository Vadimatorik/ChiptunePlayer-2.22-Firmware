#pragma once

/// Количество режимов тактирования контроллера.
#define	AYPLAYER_RCC_CFG_COUNT						4

/// SYSCLK = 36;	FCLK = 18;	PCLK1 = 4.5;	PCLK2 = 4.5
#define RCC_SPEED_FREQ_LOW							0

/// SYSCLK = 36;	FCLK = 36;	PCLK1 = 18;	PCLK2 = 18
#define RCC_SPEED_FREQ_MEDIUM						1

/// SYSCLK = 72;	FCLK = 72;	PCLK1 = 36;	PCLK2 = 36
#define RCC_SPEED_FREQ_HIGH							2

/// SYSCLK = 168;	FCLK = 168;	PCLK1 = 42;	PCLK2 = 84
#define RCC_SPEED_FREQ_VERY_HIGH					3
