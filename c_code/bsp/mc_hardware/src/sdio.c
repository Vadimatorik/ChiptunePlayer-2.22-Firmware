#include "mc_hardware.h"

#ifdef HARD
#include "stm32f4xx_hal_sd.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#include "freertos_headers.h"
#elif defined(SOFT)
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#define SD_BLOCK_SIZE 512

const char FILE_MICROSD_PATH[] = "../ChiptunePlayer-2.22-Firmware/resurse/microsd.img";
#endif

#ifdef HARD
__attribute__ ((section (".bss_ccm")))
SD_HandleTypeDef sdio = {0};

__attribute__ ((section (".bss_ccm")))
DMA_HandleTypeDef sdio_tx = {0};

__attribute__ ((section (".bss_ccm")))
DMA_HandleTypeDef sdio_rx = {0};

__attribute__ ((section (".bss_ccm")))
static SemaphoreHandle_t rx_msg_semaphore = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticSemaphore_t rx_msg_semaphore_str = {0};

__attribute__ ((section (".bss_ccm")))
static SemaphoreHandle_t tx_msg_semaphore = NULL;

__attribute__ ((section (".bss_ccm")))
static StaticSemaphore_t tx_msg_semaphore_str = {0};
#endif

int sdio_is_detected () {
#ifdef HARD
#endif
    return 0;
}

int init_sdio () {
    if (sdio_is_detected() != 0) {
        return -1;
    }

#ifdef HARD
    rx_msg_semaphore = xSemaphoreCreateBinaryStatic(&rx_msg_semaphore_str);
    tx_msg_semaphore = xSemaphoreCreateBinaryStatic(&tx_msg_semaphore_str);

    __HAL_RCC_SDIO_CLK_ENABLE();

    sdio.Instance = SDIO;
    sdio.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    sdio.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    sdio.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    sdio.Init.BusWide = SDIO_BUS_WIDE_1B;
    sdio.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
    sdio.Init.ClockDiv = 48 - 1;

    if (HAL_SD_DeInit(&sdio) != HAL_OK) {
        return -1;
    }

    if (HAL_SD_Init(&sdio) != HAL_OK) {
        return -1;
    }

    if (HAL_SD_ConfigWideBusOperation(&sdio, SDIO_BUS_WIDE_4B) != HAL_OK) {
        return -1;
    }

    sdio_rx.Instance = DMA2_Stream3;
    sdio_rx.Init.Channel = DMA_CHANNEL_4;
    sdio_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    sdio_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    sdio_rx.Init.MemInc = DMA_MINC_ENABLE;
    sdio_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    sdio_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    sdio_rx.Init.Mode = DMA_PFCTRL;
    sdio_rx.Init.Priority = DMA_PRIORITY_LOW;
    sdio_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    sdio_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    sdio_rx.Init.MemBurst = DMA_MBURST_INC4;
    sdio_rx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&sdio_rx) != HAL_OK) {
        return -1;
    }

    __HAL_LINKDMA(&sdio, hdmarx, sdio_rx);

    sdio_tx.Instance = DMA2_Stream6;
    sdio_tx.Init.Channel = DMA_CHANNEL_4;
    sdio_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    sdio_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    sdio_tx.Init.MemInc = DMA_MINC_ENABLE;
    sdio_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    sdio_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    sdio_tx.Init.Mode = DMA_PFCTRL;
    sdio_tx.Init.Priority = DMA_PRIORITY_LOW;
    sdio_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    sdio_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    sdio_tx.Init.MemBurst = DMA_MBURST_INC4;
    sdio_tx.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&sdio_tx) != HAL_OK) {
        return -1;
    }

    __HAL_LINKDMA(&sdio, hdmatx, sdio_tx);

    HAL_NVIC_SetPriority(SDIO_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

    return 0;
#elif defined(SOFT)
    int sd = open(FILE_MICROSD_PATH, O_RDWR);
    if (sd == -1) {
        return -1;
    };

    int rv = close(sd);
    if (rv != 0) {
        return -1;
    };

    return 0;
#endif
}

static int sdio_preinit () {
#ifdef HARD
    while (1) {
        HAL_SD_CardStateTypeDef s = HAL_SD_GetCardState(&sdio);

        if ((s == HAL_SD_CARD_TRANSFER) || (s == HAL_SD_CARD_READY)) {
            return 0;
        }

        if ((s == HAL_SD_CARD_SENDING) || (s == HAL_SD_CARD_RECEIVING) || (s == HAL_SD_CARD_PROGRAMMING)) {
            vTaskDelay(1);
            continue;
        }

        if (HAL_SD_InitCard(&sdio) != HAL_OK) {
            return -1;
        }

        if (HAL_SD_ConfigWideBusOperation(&sdio, SDIO_BUS_WIDE_4B) != HAL_OK) {
            return -1;
        }
    }

    return -1;
#elif defined(SOFT)
    return 0;
#endif
}

int sdio_read (uint32_t *buf, uint32_t block_num, uint32_t num_block) {
#ifdef HARD
    xSemaphoreTake(rx_msg_semaphore, 0);

    if (sdio_preinit() != 0) {
        return -1;
    }

    HAL_StatusTypeDef rv = HAL_SD_ReadBlocks_DMA(&sdio, (uint8_t *)buf, block_num, num_block);

    if (rv != HAL_OK) {
        return -1;
    }

    if (xSemaphoreTake(rx_msg_semaphore, 100) == pdTRUE) {
        return 0;
    } else {
        return -1;
    }
#elif defined(SOFT)
    int sd = open(FILE_MICROSD_PATH, O_RDONLY);
    if (sd == -1) {
        return -1;
    };

    int rv = lseek(sd, block_num*SD_BLOCK_SIZE, SEEK_SET);
    if (rv != block_num*SD_BLOCK_SIZE) {
        close(sd);
        return -1;
    }

    ssize_t read_byte_num = 0;
    while (read_byte_num != SD_BLOCK_SIZE*num_block) {
        ssize_t real_read_byte_num = read(sd, &buf[read_byte_num], SD_BLOCK_SIZE*num_block - read_byte_num);
        if (real_read_byte_num == -1) {
            close(sd);
            return -1;
        }

        read_byte_num += real_read_byte_num;
    }

    rv = close(sd);
    return rv;
#endif
}

int sdio_write (const uint32_t *buf, uint32_t block_num, uint32_t num_block) {
#ifdef HARD
    xSemaphoreTake(tx_msg_semaphore, 0);

    if (sdio_preinit() != 0) {
        return -1;
    }

    HAL_StatusTypeDef rv = HAL_SD_WriteBlocks_DMA(&sdio, (uint8_t *)buf, block_num, num_block);

    if (rv != HAL_OK) {
        return -1;
    }

    if (xSemaphoreTake(tx_msg_semaphore, 100) == pdTRUE) {
        return 0;
    } else {
        return -1;
    }
#elif defined(SOFT)
    int sd = open(FILE_MICROSD_PATH, O_WRONLY);
    if (sd == -1) {
        return -1;
    };

    int rv = lseek(sd, block_num*SD_BLOCK_SIZE, SEEK_SET);
    if (rv != block_num*SD_BLOCK_SIZE) {
        close(sd);
        return -1;
    }

    ssize_t write_byte_num = 0;
    while (write_byte_num != SD_BLOCK_SIZE*num_block) {
        ssize_t real_write_byte_num = write(sd, &buf[write_byte_num], SD_BLOCK_SIZE*num_block - write_byte_num);
        if (real_write_byte_num == -1) {
            close(sd);
            return -1;
        }

        write_byte_num += real_write_byte_num;
    }

    rv = close(sd);
    return rv;
#endif
}


int sdio_get_status () {
#ifdef HARD
    return 0;
#elif defined(SOFT)
    return 0;
#endif
}

int sdio_get_sector_count (uint32_t *count) {
#ifdef HARD
    HAL_SD_CardInfoTypeDef info;
    if (HAL_SD_GetCardInfo(&sdio, &info) != HAL_OK) {
        return -1;
    }

    *count = info.BlockNbr;
#elif defined(SOFT)
    FILE *sd = fopen(FILE_MICROSD_PATH, "r");
    if (sd == NULL) {
        return -1;
    };

    if (fseek(sd, 0, SEEK_END) != 0) {
        fclose(sd);
        return -1;
    }

    *count = ftell(sd)/SD_BLOCK_SIZE;

    int rv = fclose(sd);
    return (rv)?-1:0;
#endif
}

int sdio_get_block_size (uint32_t *size) {
#ifdef HARD
    HAL_SD_CardInfoTypeDef info;
    if (HAL_SD_GetCardInfo(&sdio, &info) != HAL_OK) {
        return -1;
    }

    *size = info.BlockSize;
#elif defined(SOFT)
    *size = SD_BLOCK_SIZE;
#endif

    return 0;
}

#ifdef HARD
void SDIO_IRQHandler () {
    HAL_SD_IRQHandler(&sdio);
}

void DMA2_Stream3_IRQHandler () {
    HAL_DMA_IRQHandler(&sdio_rx);
}

void DMA2_Stream6_IRQHandler () {
    HAL_DMA_IRQHandler(&sdio_tx);
}

void HAL_SD_RxCpltCallback (SD_HandleTypeDef *hsd) {
    xSemaphoreGiveFromISR(rx_msg_semaphore, NULL);
}

void HAL_SD_TxCpltCallback (SD_HandleTypeDef *hsd) {
    xSemaphoreGiveFromISR(tx_msg_semaphore, NULL);
}
#endif
