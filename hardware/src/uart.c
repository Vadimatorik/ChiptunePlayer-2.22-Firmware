#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_cortex.h"

#include "freertos_headers.h"

#include <errno.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <memory.h>

UART_HandleTypeDef u = {0};
DMA_HandleTypeDef u_dma = {0};

int init_uart () {
    __HAL_RCC_USART1_CLK_ENABLE();

    u.Instance = USART1;
    u.Init.BaudRate = 115200;
    u.Init.WordLength = UART_WORDLENGTH_8B;
    u.Init.StopBits = UART_STOPBITS_1;
    u.Init.Parity = UART_PARITY_NONE;
    u.Init.Mode = UART_MODE_TX_RX;
    u.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    u.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&u) != HAL_OK) {
        return EIO;
    }

    __HAL_RCC_DMA2_CLK_ENABLE();
    u_dma.Instance = DMA2_Stream7;
    u_dma.Init.Channel = DMA_CHANNEL_4;
    u_dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    u_dma.Init.PeriphInc = DMA_PINC_DISABLE;
    u_dma.Init.MemInc = DMA_MINC_ENABLE;
    u_dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    u_dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    u_dma.Init.Mode = DMA_NORMAL;
    u_dma.Init.Priority = DMA_PRIORITY_MEDIUM;
    u_dma.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    u_dma.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    u_dma.Init.MemBurst = DMA_MBURST_SINGLE;
    u_dma.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&u_dma) != HAL_OK) {
        return EIO;
    }

    __HAL_LINKDMA(&u, hdmatx, u_dma);

    __HAL_UART_ENABLE_IT(&u, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&u, UART_IT_PE);
    __HAL_UART_ENABLE_IT(&u, UART_IT_ERR);

    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    return 0;
}

void DMA2_Stream7_IRQHandler () {
    HAL_DMA_IRQHandler(&u_dma);
}

void USART1_IRQHandler () {
    HAL_UART_IRQHandler(&u);
}

size_t fwrite (const void *buf, size_t size, size_t count, FILE *stream) {
    stream = stream;
    size_t len = size*count;

    if (HAL_UART_Transmit_DMA(&u, (uint8_t *)buf, len) == HAL_OK) {
        return len;
    }

    return 0;
}

int _write (int file, char *buf, int len) {
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
        errno = EBADF;
        return -1;
    }

    while (HAL_UART_GetState(&u) != HAL_UART_STATE_READY) {
        vTaskDelay(1);
    }

    if (HAL_UART_Transmit_DMA(&u, (uint8_t *)buf, len) == HAL_OK) {
        return len;
    }

    return 0;
}

uint8_t u_rx_buffer[1024] = {0};
int u_rx_pointer = 0;

void HAL_UART_RxCpltCallback (UART_HandleTypeDef *huart) {
    if (u_rx_pointer == sizeof(u_rx_buffer)) {
        volatile int buf = huart->Instance->DR;
        (void)buf;
        return;
    }

    __disable_irq();
    u_rx_buffer[u_rx_pointer++] = huart->Instance->DR;
    __enable_irq();
}

int __attribute__ ((weak)) _read (char *ptr, int len) {
    while (u_rx_pointer == 0) {
        vTaskDelay(1);
    }

    __disable_irq();
    int l_cpy = (u_rx_pointer > len)?len:u_rx_pointer;
    memcpy(ptr, u_rx_buffer, l_cpy);
    u_rx_pointer -= l_cpy;
    __enable_irq();

    return l_cpy;
}

