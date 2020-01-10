#ifdef HARD
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_uart.h"
#include "stm32f4xx_hal_cortex.h"

#include "freertos_headers.h"

#include <errno.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include "freertos_obj.h"

__attribute__ ((section (".bss_ccm")))
static StaticTask_t task_uart_buf = {0};

__attribute__ ((section (".bss_ccm")))
static StackType_t task_uart_stack[UART_THREAD_STACK_SIZE] = {0};

__attribute__ ((section (".bss_ccm")))
static SemaphoreHandle_t rx_msg_semaphore = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticSemaphore_t rx_msg_semaphore_str = {0};

typedef struct _tx_msg_cfg {
    union data {
        uint8_t *p;
        uint8_t data[4];
    } data_t;

    uint32_t len;
} tx_msg_cfg_t;

__attribute__ ((section (".bss_ccm")))
static QueueHandle_t tx_points_queue = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticQueue_t tx_points_queue_str = {0};

__attribute__ ((section (".bss_ccm")))
static uint8_t tx_points_queue_buf[UART_TX_QUEUE_LEN*sizeof(tx_msg_cfg_t)] = {0};

__attribute__ ((section (".bss_ccm")))
SemaphoreHandle_t rx_buf_mutex = NULL;

__attribute__ ((section (".bss_ccm")))
StaticSemaphore_t rx_buf_mutex_buf = {0};

__attribute__ ((section (".bss_ccm")))
static UART_HandleTypeDef u = {0};

__attribute__ ((section (".bss_ccm")))
static DMA_HandleTypeDef u_dma = {0};

__attribute__ ((section (".bss_ccm")))
static char u_rx_buffer[1024] = {0};

__attribute__ ((section (".bss_ccm")))
volatile int u_rx_pointer = 0;

__attribute__ ((section (".bss_ccm")))
QueueHandle_t tx_semaphore = NULL;

__attribute__ ((section (".bss_ccm")))
StaticSemaphore_t tx_semaphore_str = {0};


static void task_uart (void *p) {
    tx_msg_cfg_t msg = {0};

    while (1) {
        xQueueReceive(tx_points_queue, &msg, portMAX_DELAY);
        xSemaphoreTake(tx_semaphore, 0);

        if (msg.len <= 4) {
            while (HAL_UART_Transmit_DMA(&u, msg.data_t.data, msg.len) != HAL_OK) {
                vTaskDelay(10);
            };

            xSemaphoreTake(tx_semaphore, portMAX_DELAY);
        } else {
            if (msg.data_t.p == NULL) {
                continue;
            }

            while (HAL_UART_Transmit_DMA(&u, msg.data_t.p, msg.len) != HAL_OK) {
                vTaskDelay(10);
            }

            xSemaphoreTake(tx_semaphore, portMAX_DELAY);

            free(msg.data_t.p);
        }
    }
}

static int init_mc_uart () {
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

    return 0;
}

static int init_mc_uart_dma () {
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

    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

    return 0;
}

static int init_mc_uart_os () {
    rx_msg_semaphore = xSemaphoreCreateBinaryStatic(&rx_msg_semaphore_str);
    rx_buf_mutex = xSemaphoreCreateMutexStatic(&rx_buf_mutex_buf);
    tx_semaphore = xSemaphoreCreateBinaryStatic(&tx_semaphore_str);
    tx_points_queue = xQueueCreateStatic(UART_TX_QUEUE_LEN, sizeof(tx_msg_cfg_t),
                                         &tx_points_queue_buf[0], &tx_points_queue_str);

    if (xTaskCreateStatic(task_uart, "uart", UART_THREAD_STACK_SIZE, NULL, UART_THREAD_PRIO,
                          task_uart_stack, &task_uart_buf) == NULL) {
        return ENOMEM;
    }

    return 0;
}

static int start_mc_uart () {
    __HAL_UART_ENABLE_IT(&u, UART_IT_PE);
    __HAL_UART_ENABLE_IT(&u, UART_IT_ERR);
    __HAL_UART_ENABLE_IT(&u, UART_IT_RXNE);

    NVIC_SetPriority(USART1_IRQn, 6);
    NVIC_EnableIRQ(USART1_IRQn);

    return 0;
}
#endif

int init_uart () {
    int rv = 0;

#ifdef HARD
    if ((rv = init_mc_uart()) != 0) {
        return rv;
    }

    if ((rv = init_mc_uart_dma()) != 0) {
        return rv;
    }

    if ((rv = init_mc_uart_os()) != 0) {
        return rv;
    }

    if ((rv = start_mc_uart()) != 0) {
        return rv;
    }
#endif

    return rv;
}

#ifdef HARD
static int add_transaction (const void *p, uint32_t len) {
    tx_msg_cfg_t msg;
    msg.len = len;
    if (len <= 4) {
        memcpy(msg.data_t.data, p, len);
    } else {
        msg.data_t.p = malloc(msg.len);
        if (p == NULL) {
            return ENOMEM;
        }
        memcpy(msg.data_t.p, p, len);
    }

    xQueueSend(tx_points_queue, &msg, portMAX_DELAY);

    return 0;
}

static int add_transaction_from_irq (uint8_t data) {
    tx_msg_cfg_t msg;
    msg.len = 1;
    msg.data_t.data[0] = data;
    xQueueSendFromISR(tx_points_queue, &msg, NULL);
}

size_t fwrite (const void *buf, size_t size, size_t count, FILE *stream) {
    stream = stream;

    if (add_transaction(buf, size*count) == 0) {
        return size*count;
    }

    return 0;
}

int _write (int file, char *ptr, int len) {
    if (add_transaction(ptr, len) == 0) {
        return len;
    }

    return 0;
}

static void rx_byte (uint8_t data) {
    if (u_rx_pointer == sizeof(u_rx_buffer)) {
        return;
    }

    xSemaphoreTakeFromISR(rx_buf_mutex, NULL);
    if (data == '\r') {
        u_rx_buffer[u_rx_pointer++] = '\n';
        xSemaphoreGiveFromISR(rx_msg_semaphore, NULL);
        add_transaction_from_irq(data);
    } else if (data == 127) {
        if (u_rx_pointer > 0) {
            u_rx_pointer--;
            u_rx_buffer[u_rx_pointer] = 0;
            add_transaction_from_irq(data);
        }
    } else {
        u_rx_buffer[u_rx_pointer++] = data;
        add_transaction_from_irq(data);
    }

    xSemaphoreGiveFromISR(rx_buf_mutex, NULL);
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart) {
    xSemaphoreGiveFromISR (tx_semaphore, NULL);
}

void USART1_IRQHandler () {
    if (__HAL_UART_GET_FLAG(&u, UART_FLAG_RXNE)) {
        rx_byte(u.Instance->DR);
    }

    HAL_UART_IRQHandler(&u);
}

int _read (int fd, void *buf, size_t count) {
    xSemaphoreTake(rx_msg_semaphore, portMAX_DELAY);
    xSemaphoreTake(rx_buf_mutex, portMAX_DELAY);
    memcpy(buf, u_rx_buffer, u_rx_pointer);
    int rv = u_rx_pointer;
    u_rx_pointer = 0;
    xSemaphoreGive(rx_buf_mutex);
    return rv;
}

void DMA2_Stream7_IRQHandler () {
    HAL_DMA_IRQHandler(&u_dma);
}

#endif
