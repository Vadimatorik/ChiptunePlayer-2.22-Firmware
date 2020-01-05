#include "mc_hardware.h"
#include <errno.h>

#ifdef AYM_HARDWARE
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"

#include "freertos_headers.h"

SPI_HandleTypeDef s_board = {0};
SPI_BOARD_DEVICE spi_device = SPI_BOARD_DEVICE_NO_SET;

SemaphoreHandle_t spi_board_tx_msg_semaphore = NULL;
static StaticSemaphore_t spi_board_tx_msg_semaphore_str = {0};

SemaphoreHandle_t spi_board_mutex = NULL;
StaticSemaphore_t spi_board_mutex_buf = {0};
#endif

int init_spi_board () {
#ifdef AYM_HARDWARE
    spi_board_tx_msg_semaphore = xSemaphoreCreateBinaryStatic(&spi_board_tx_msg_semaphore_str);
    spi_board_mutex = xSemaphoreCreateMutexStatic(&spi_board_mutex_buf);

    __HAL_RCC_SPI2_CLK_ENABLE();

    s_board.Instance = SPI2;
    s_board.Init.Mode = SPI_MODE_MASTER;
    s_board.Init.Direction = SPI_DIRECTION_1LINE;
    s_board.Init.DataSize = SPI_DATASIZE_8BIT;
    s_board.Init.CLKPolarity = SPI_POLARITY_LOW;
    s_board.Init.CLKPhase = SPI_PHASE_1EDGE;
    s_board.Init.NSS = SPI_NSS_SOFT;
    s_board.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    s_board.Init.FirstBit = SPI_FIRSTBIT_MSB;
    s_board.Init.TIMode = SPI_TIMODE_DISABLE;
    s_board.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    s_board.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&s_board) != HAL_OK) {
        return EIO;
    }

    HAL_NVIC_SetPriority(SPI2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);
#endif

    return 0;
}

#ifdef AYM_HARDWARE
void SPI2_IRQHandler () {
    HAL_SPI_IRQHandler(&s_board);
}
#endif

int spi_board_device_ltc6903_tx (void *d, uint32_t len) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(spi_board_mutex, portMAX_DELAY);
    xSemaphoreTake(spi_board_tx_msg_semaphore, 0);
    spi_device = SPI_BOARD_DEVICE_LTC6903;
    reset_pin_ltc_cs();
    HAL_StatusTypeDef rv = HAL_SPI_Transmit_IT(&s_board, d, len);
    if (rv != HAL_OK) {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }

    if (xSemaphoreTake(spi_board_tx_msg_semaphore, 100) == pdTRUE) {
        xSemaphoreGive(spi_board_mutex);
        return 0;
    } else {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }
#else
    return 0;
#endif
}

int spi_board_device_ad5204_tx (void *d, uint32_t len) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(spi_board_mutex, portMAX_DELAY);
    xSemaphoreTake(spi_board_tx_msg_semaphore, 0);
    spi_device = SPI_BOARD_DEVICE_AD5204;
    reset_pin_ad5204_cs();
    HAL_StatusTypeDef rv = HAL_SPI_Transmit_IT(&s_board, d, len);
    if (rv != HAL_OK) {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }

    if (xSemaphoreTake(spi_board_tx_msg_semaphore, 100) == pdTRUE) {
        xSemaphoreGive(spi_board_mutex);
        return 0;
    } else {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }
#else
    return 0;
#endif
}

int spi_board_device_sr_tx (void *d, uint32_t len) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(spi_board_mutex, portMAX_DELAY);
    xSemaphoreTake(spi_board_tx_msg_semaphore, 0);
    spi_device = SPI_BOARD_DEVICE_SR;
    reset_pin_sr_strob();
    HAL_StatusTypeDef rv = HAL_SPI_Transmit_IT(&s_board, d, len);
    if (rv != HAL_OK) {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }

    if (xSemaphoreTake(spi_board_tx_msg_semaphore, 100) == pdTRUE) {
        xSemaphoreGive(spi_board_mutex);
        return 0;
    } else {
        xSemaphoreGive(spi_board_mutex);
        return EIO;
    }
#else
    return 0;
#endif
}