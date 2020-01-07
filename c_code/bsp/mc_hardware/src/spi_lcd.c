#ifdef AYM_HARDWARE
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_cortex.h"
#endif

#include "mc_hardware.h"
#include <errno.h>

#ifdef AYM_HARDWARE
#include "freertos_headers.h"

__attribute__ ((section (".bss_ccm")))
SPI_HandleTypeDef s_lcd = {0};

__attribute__ ((section (".bss_ccm")))
DMA_HandleTypeDef s_lcd_dma = {0};

__attribute__ ((section (".bss_ccm")))
SemaphoreHandle_t spi_lcd_msg_semaphore = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticSemaphore_t spi_lcd_msg_semaphore_str = {0};

__attribute__ ((section (".bss_ccm")))
SemaphoreHandle_t spi_lcd_mutex = NULL;

__attribute__ ((section (".bss_ccm")))
StaticSemaphore_t spi_lcd_mutex_buf = {0};
#endif

#ifdef AYM_SOFT
#include "socket_emul_layer.h"
#endif


int init_spi_lcd () {
#if defined(AYM_HARDWARE)
    spi_lcd_msg_semaphore = xSemaphoreCreateBinaryStatic(&spi_lcd_msg_semaphore_str);
    spi_lcd_mutex = xSemaphoreCreateMutexStatic(&spi_lcd_mutex_buf);

    __HAL_RCC_SPI1_CLK_ENABLE();

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

    HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

    __HAL_LINKDMA(&s_lcd, hdmatx, s_lcd_dma);

    return 0;
#elif defined(AYM_SOFT)
    set_pin_lcd_cs();

    return 0;
#endif
}

#ifdef AYM_HARDWARE
void DMA2_Stream5_IRQHandler () {
    HAL_DMA_IRQHandler(&s_lcd_dma);
}
#endif



int spi_lcd_tx (void *d, uint32_t len) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(spi_lcd_mutex, portMAX_DELAY);
    xSemaphoreTake(spi_lcd_msg_semaphore, 0);
    reset_pin_lcd_cs();
    HAL_StatusTypeDef rv = HAL_SPI_Transmit_DMA(&s_lcd, d, len);
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

