#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

#include "mc_hardware.h"

#include <errno.h>

SPI_HandleTypeDef s_board = {0};

int init_spi_board () {
    __HAL_RCC_SPI2_CLK_ENABLE();

    s_board.Instance = SPI2;
    s_board.Init.Mode = SPI_MODE_MASTER;
    s_board.Init.Direction = SPI_DIRECTION_2LINES;
    s_board.Init.DataSize = SPI_DATASIZE_8BIT;
    s_board.Init.CLKPolarity = SPI_POLARITY_LOW;
    s_board.Init.CLKPhase = SPI_PHASE_1EDGE;
    s_board.Init.NSS = SPI_NSS_SOFT;
    s_board.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    s_board.Init.FirstBit = SPI_FIRSTBIT_MSB;
    s_board.Init.TIMode = SPI_TIMODE_DISABLE;
    s_board.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    s_board.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&s_board) != HAL_OK) {
        return EIO;
    }

    HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
}

void SPI2_IRQHandler () {
    HAL_SPI_IRQHandler(&s_board);
}

int spi_board_device_ad5204_tx (void *d, uint32_t len) {
    reset_pin_ad5204_cs();
    HAL_StatusTypeDef rv = HAL_SPI_Transmit_IT(&s_board, d, len);
    return (rv == HAL_OK)?0:EIO;
}
