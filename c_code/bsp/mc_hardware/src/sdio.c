#include "mc_hardware.h"

#include <errno.h>

#ifdef AYM_HARDWARE
#include "stm32f4xx_hal_sd.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_cortex.h"
#include "freertos_headers.h"
#elif defined(AYM_SOFT)
#include <fcntl.h>
#include <sys/stat.h>
#endif

#ifdef AYM_HARDWARE
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
#elif defined(AYM_SOFT)
#include <stdio.h>
#define SD_BLOCK_SIZE 512
static FILE *sd = NULL;
#endif

int sdio_is_detected () {
#ifdef AYM_HARDWARE
#endif
    return 0;
}

int init_sdio () {
    if (sdio_is_detected() != 0) {
        return EIO;
    }

#ifdef AYM_HARDWARE
    rx_msg_semaphore = xSemaphoreCreateBinaryStatic(&rx_msg_semaphore_str);
    tx_msg_semaphore = xSemaphoreCreateBinaryStatic(&tx_msg_semaphore_str);

    __HAL_RCC_SDIO_CLK_ENABLE();

    sdio.Instance = SDIO;
    sdio.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    sdio.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    sdio.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    sdio.Init.BusWide = SDIO_BUS_WIDE_1B;
    sdio.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_ENABLE;
    sdio.Init.ClockDiv = 4;

    if (HAL_SD_DeInit(&sdio) != HAL_OK) {
        return EIO;
    }

    if (HAL_SD_Init(&sdio) != HAL_OK) {
        return EIO;
    }

    if (HAL_SD_ConfigWideBusOperation(&sdio, SDIO_BUS_WIDE_4B) != HAL_OK) {
        return EIO;
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
        return EIO;
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
        return EIO;
    }

    __HAL_LINKDMA(&sdio, hdmatx, sdio_tx);

    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

    return 0;
#elif defined(AYM_SOFT)
    sd = fopen("../ChiptunePlayer-2.22-Firmware/resurse/microsd.img", "r");
    if (sd == NULL) {
        return -1;
    } else {
        return 0;
    }
#endif
}

int sdio_read (uint32_t *buf, uint32_t block_num, uint32_t num_block) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(rx_msg_semaphore, 0);

    if (HAL_SD_ReadBlocks_DMA(&sdio, (uint8_t *)buf, block_num, num_block) != HAL_OK) {
        return EIO;
    }

    if (xSemaphoreTake(rx_msg_semaphore, 100) == pdTRUE) {
        return 0;
    } else {
        return EIO;
    }
#elif defined(AYM_SOFT)
    if (fseek(sd, block_num*SD_BLOCK_SIZE, SEEK_SET) != 0) {
        return -1;
    }

    if (fread(buf, SD_BLOCK_SIZE, num_block, sd) == num_block) {
        return 0;
    } else {
        return -1;
    }
#endif
}

int sdio_write (const uint32_t *buf, uint32_t block_num, uint32_t num_block) {
#ifdef AYM_HARDWARE
    xSemaphoreTake(tx_msg_semaphore, 0);

    if ((HAL_SD_WriteBlocks_DMA(&sdio, (uint8_t *)buf, block_num, num_block) == HAL_OK) != HAL_OK) {
        return EIO;
    }

    if (xSemaphoreTake(tx_msg_semaphore, 100) == pdTRUE) {
        return 0;
    } else {
        return EIO;
    }
#elif defined(AYM_SOFT)
    if (fseek(sd, block_num*SD_BLOCK_SIZE, SEEK_SET) != 0) {
        return -1;
    }

    if (fwrite(buf, SD_BLOCK_SIZE, num_block, sd) == num_block) {
        return 0;
    } else {
        return -1;
    }
#endif
}


int sdio_get_status () {
#ifdef AYM_HARDWARE
    HAL_SD_CardStateTypeDef s = HAL_SD_GetCardState(&sdio);

    if ((s == HAL_SD_CARD_READY) || (s == HAL_SD_CARD_TRANSFER)) {
        return 0;
    }

    return -1;
#elif defined(AYM_SOFT)
    if (sd != NULL) {
        return 0;
    } else {
        return -1;
    }
#endif
}

int sdio_get_sector_count (uint32_t *count) {
#ifdef AYM_HARDWARE
    HAL_SD_CardInfoTypeDef info;
    if (HAL_SD_GetCardInfo(&sdio, &info) != HAL_OK) {
        return EIO;
    }

    *count = info.BlockNbr;
#elif defined(AYM_SOFT)
    fseek(sd, 0, SEEK_END);
    *count = ftell(sd) / SD_BLOCK_SIZE;
#endif

    return 0;
}

int sdio_get_block_size (uint32_t *size) {
#ifdef AYM_HARDWARE
    HAL_SD_CardInfoTypeDef info;
    if (HAL_SD_GetCardInfo(&sdio, &info) != HAL_OK) {
        return EIO;
    }

    *size = info.BlockSize;
#elif defined(AYM_SOFT)
    *size = SD_BLOCK_SIZE;
#endif

    return 0;
}

#ifdef AYM_HARDWARE
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
