#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_cortex.h"

#include <errno.h>

SPI_HandleTypeDef s_lcd = {0};
DMA_HandleTypeDef s_lcd_dma = {0};

int init_spi_lcd () {
    __HAL_RCC_SPI1_CLK_ENABLE();

    s_lcd.Instance = SPI1;
    s_lcd.Init.Mode = SPI_MODE_MASTER;
    s_lcd.Init.Direction = SPI_DIRECTION_2LINES;
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

    s_lcd_dma.Instance = DMA2_Stream3;
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

    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    __HAL_LINKDMA(&s_lcd, hdmatx, s_lcd_dma);

    return 0;
}

void SPI1_IRQHandler () {
    HAL_SPI_IRQHandler(&s_lcd);
}

void DMA2_Stream3_IRQHandler () {
    HAL_DMA_IRQHandler(&s_lcd_dma);
}

