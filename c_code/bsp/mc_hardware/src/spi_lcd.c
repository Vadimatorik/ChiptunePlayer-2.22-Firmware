#ifdef HARD
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_cortex.h"
#endif

#include "mc_hardware.h"
#include <errno.h>

#ifdef HARD
#include "freertos_headers.h"

SPI_HandleTypeDef s_lcd = {0};
DMA_HandleTypeDef s_lcd_dma = {0};
SemaphoreHandle_t spi_lcd_msg_semaphore = NULL;
static StaticSemaphore_t spi_lcd_msg_semaphore_str = {0};
SemaphoreHandle_t spi_lcd_mutex = NULL;
StaticSemaphore_t spi_lcd_mutex_buf = {0};
#endif

#ifdef SOFT
#include "socket_emul_layer.h"
#endif


int init_spi_lcd () {
#if defined(HARD)
    spi_lcd_msg_semaphore = xSemaphoreCreateBinaryStatic(&spi_lcd_msg_semaphore_str);
    spi_lcd_mutex = xSemaphoreCreateMutexStatic(&spi_lcd_mutex_buf);

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    s_lcd.Instance = SPI1;
    s_lcd.Init.Mode = SPI_MODE_MASTER;
    s_lcd.Init.Direction = SPI_DIRECTION_1LINE;
    s_lcd.Init.DataSize = SPI_DATASIZE_8BIT;
    s_lcd.Init.CLKPolarity = SPI_POLARITY_HIGH;
    s_lcd.Init.CLKPhase = SPI_PHASE_2EDGE;
    s_lcd.Init.NSS = SPI_NSS_SOFT;
    s_lcd.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    s_lcd.Init.FirstBit = SPI_FIRSTBIT_MSB;
    s_lcd.Init.TIMode = SPI_TIMODE_DISABLE;
    s_lcd.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    s_lcd.Init.CRCPolynomial = 10;

    if (HAL_SPI_Init(&s_lcd) != HAL_OK) {
        return EIO;
    }

    s_lcd_dma.Instance = DMA2_Stream5;
    s_lcd_dma.Init.Channel = DMA_CHANNEL_3;
    s_lcd_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    s_lcd_dma.Init.PeriphInc = DMA_PINC_DISABLE;
    s_lcd_dma.Init.MemInc = DMA_MINC_ENABLE;
    s_lcd_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    s_lcd_dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    s_lcd_dma.Init.Mode = DMA_NORMAL;
    s_lcd_dma.Init.Priority = DMA_PRIORITY_LOW;
    s_lcd_dma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    s_lcd_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    s_lcd_dma.Init.MemBurst = DMA_MBURST_SINGLE;
    s_lcd_dma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&s_lcd_dma) != HAL_OK) {
        return EIO;
    }

    __HAL_LINKDMA(&s_lcd, hdmatx, s_lcd_dma);

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

    HAL_NVIC_SetPriority(SPI1_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    return 0;
#elif defined(SOFT)
    set_pin_lcd_cs();

    return 0;
#endif
}

#ifdef HARD
void DMA2_Stream5_IRQHandler () {
    HAL_DMA_IRQHandler(&s_lcd_dma);
}

void SPI1_IRQHandler () {
    HAL_SPI_IRQHandler(&s_lcd);
}
#endif

int spi_lcd_tx (void *d, uint32_t len) {
#ifdef HARD
    xSemaphoreTake(spi_lcd_mutex, portMAX_DELAY);
    xSemaphoreTake(spi_lcd_msg_semaphore, 0);
    reset_pin_lcd_cs();

    HAL_StatusTypeDef rv;
    if ((uint32_t)d % 4 == 0) {
        rv = HAL_SPI_Transmit_DMA(&s_lcd, d, len);
    } else {
        rv = HAL_SPI_Transmit_IT(&s_lcd, d, len);
    }

    if (rv != HAL_OK) {
        xSemaphoreGive(spi_lcd_mutex);
        return EIO;
    }

    if (xSemaphoreTake(spi_lcd_msg_semaphore, 100) == pdTRUE) {
        xSemaphoreGive(spi_lcd_mutex);
        return 0;
    } else {
        xSemaphoreGive(spi_lcd_mutex);
        return EIO;
    }
#else
    reset_pin_lcd_cs();
    socket_spi_lcd_tx(d, len);
    set_pin_lcd_cs();
    return 0;
#endif
}

