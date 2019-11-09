#include "microsd_card_sdio.h"

#ifdef MODULE_MICROSD_CARD_SDIO_ENABLED

#define checkResult(r)                                    \
        if ( r != 0 ) return EC_MICRO_SD_TYPE::ERROR;

MicrosdSdio::MicrosdSdio (const MicrosdSdioCfg *const cfg) : cfg(cfg) {
    this->handle.Instance = SDIO;
    this->handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    this->handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    this->handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    this->handle.Init.BusWide = SDIO_BUS_WIDE_1B;
    this->handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    this->handle.Init.ClockDiv = cfg->div;
    
    this->handle.obj = this;
    
    this->handle.hdmarx = &this->dmaRx;
    this->handle.hdmatx = nullptr;
    
    this->handle.hdmarx->Parent = &this->handle;
    this->handle.hdmatx->Parent = nullptr;
    
    this->handle.hdmarx->Instance = this->cfg->dmaRx;
    this->handle.hdmarx->Init.Channel = this->cfg->dmaRxCh;
    this->handle.hdmarx->Init.Direction = DMA_PERIPH_TO_MEMORY;
    this->handle.hdmarx->Init.PeriphInc = DMA_PINC_DISABLE;
    this->handle.hdmarx->Init.MemInc = DMA_MINC_ENABLE;
    this->handle.hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    this->handle.hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    this->handle.hdmarx->Init.Mode = DMA_PFCTRL;
    this->handle.hdmarx->Init.Priority = DMA_PRIORITY_LOW;
    this->handle.hdmarx->Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    this->handle.hdmarx->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    this->handle.hdmarx->Init.MemBurst = DMA_MBURST_INC4;
    this->handle.hdmarx->Init.PeriphBurst = DMA_PBURST_INC4;
    
    this->m = xSemaphoreCreateMutexStatic(&mb);
    this->s = xSemaphoreCreateBinaryStatic(&this->sb);
}

EC_SD_RESULT MicrosdSdio::waitReadySd (void) {
    uint32_t timeout_flag = 1000;
    while (timeout_flag) {
        if (HAL_SD_GetCardState(&this->handle) != HAL_SD_CARD_TRANSFER) {
            vTaskDelay(1);
            timeout_flag--;
        } else {
            return EC_SD_RESULT::OK;
        }
    }
    return EC_SD_RESULT::ERROR;
}

void MicrosdSdio::dmaRxHandler (void) {
    HAL_DMA_IRQHandler(&this->dmaRx);
}

EC_MICRO_SD_TYPE MicrosdSdio::initialize (void) {
    if (HAL_SD_GetState(&this->handle) == HAL_SD_STATE_RESET) {        /// Первый запуск.
        __HAL_RCC_SYSCFG_CLK_ENABLE();
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_RCC_SDIO_CLK_ENABLE();
        
        mc::dmaClkOn(this->cfg->dmaRx);
        
        checkResult(HAL_DMA_DeInit(&this->dmaRx));
        checkResult(HAL_DMA_Init(&this->dmaRx));
        
        mc::dmaIrqOn(this->cfg->dmaRx, this->cfg->dmaRxIrqPrio);
        
        checkResult(HAL_SD_DeInit(&this->handle));
        checkResult(HAL_SD_Init(&this->handle));
        checkResult(HAL_SD_ConfigWideBusOperation(&this->handle, this->cfg->wide));
        
        return this->getType();
    } else {
        checkResult(HAL_SD_InitCard(&this->handle));
        return this->getType();
    }
}

EC_MICRO_SD_TYPE MicrosdSdio::getType (void) {
    EC_MICRO_SD_TYPE t;
    t = (this->handle.SdCard.CardVersion == CARD_V1_X) ? EC_MICRO_SD_TYPE::SD1 : EC_MICRO_SD_TYPE::SD2;
    if (this->handle.SdCard.CardType == CARD_SDHC_SDXC) {
        t = (EC_MICRO_SD_TYPE)((uint32_t)t | (uint32_t)EC_MICRO_SD_TYPE::BLOCK);
    }
    
    return t;
}

EC_SD_RESULT MicrosdSdio::readSector (uint32_t sector, uint8_t *targetArray, uint32_t countSector, uint32_t timeoutMs) {
    if ((uint32_t)targetArray & 0b11)            /// Указатель должен быть выравнен на 4.
        return EC_SD_RESULT::POINTERR;
    
    EC_SD_RESULT rv = EC_SD_RESULT::ERROR;
    
    xSemaphoreTake(this->m, portMAX_DELAY);
    
    xSemaphoreTake (this->s, 0);
    
    if (this->waitReadySd() == EC_SD_RESULT::OK) {
        
        if (HAL_SD_ReadBlocks_DMA(&this->handle, targetArray, sector, countSector) == HAL_OK) {
            if (xSemaphoreTake (this->s, timeoutMs) == pdTRUE) {
                rv = EC_SD_RESULT::OK;
            };
        }
        
    }
    
    xSemaphoreGive(this->m);
    
    return rv;
}

EC_SD_RESULT
MicrosdSdio::writeSector (const uint8_t *const sourceArray, uint32_t sector, uint32_t countSector, uint32_t timeoutMs) {
    if ((uint32_t)sourceArray & 0b11)            /// Указатель должен быть выравнен на 4.
        return EC_SD_RESULT::POINTERR;
    
    EC_SD_RESULT rv = EC_SD_RESULT::ERROR;
    
    xSemaphoreTake(this->m, portMAX_DELAY);
    
    if (this->waitReadySd() == EC_SD_RESULT::OK) {
        
        HAL_StatusTypeDef res;
        res = HAL_SD_WriteBlocks(&this->handle, (uint8_t *)sourceArray, sector, countSector, timeoutMs);
        
        if (res == HAL_OK) {
            rv = EC_SD_RESULT::OK;
        }
        
    }
    
    xSemaphoreGive(this->m);
    
    return rv;
}

void MicrosdSdio::giveSemaphore (void) {
    if (this->s) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR (this->s, &xHigherPriorityTaskWoken);
    }
}

extern "C" {

void HAL_SD_RxCpltCallback (SD_HandleTypeDef *hsd) {
    MicrosdSdio *o = (MicrosdSdio *)hsd->obj;
    o->giveSemaphore();
}

}

#define NUMBER_OF_ATTEMPTS                30


EC_SD_STATUS MicrosdSdio::getStatus (void) {
    if (this->handle.State == HAL_SD_STATE_RESET) {
        return EC_SD_STATUS::NOINIT;
    }
    
    int l = NUMBER_OF_ATTEMPTS;
    
    do {
        HAL_SD_CardStateTypeDef s = HAL_SD_GetCardState(&this->handle);
        if (s == HAL_SD_CARD_TRANSFER) {
            return EC_SD_STATUS::OK;
        }
        l--;
        vTaskDelay(1);
    } while (l);
    
    return EC_SD_STATUS::NOINIT;
}

EC_SD_RESULT MicrosdSdio::getSectorCount (uint32_t &sectorCount) {
    if (this->handle.State == HAL_SD_STATE_RESET) {
        return EC_SD_RESULT::ERROR;
    }
    
    sectorCount = this->handle.SdCard.LogBlockNbr;
    return EC_SD_RESULT::OK;
}

EC_SD_RESULT MicrosdSdio::getBlockSize (uint32_t &blockSize) {
    if (this->handle.State == HAL_SD_STATE_RESET) {
        return EC_SD_RESULT::ERROR;
    }
    
    blockSize = this->handle.SdCard.LogBlockSize;
    return EC_SD_RESULT::OK;
}

#endif
