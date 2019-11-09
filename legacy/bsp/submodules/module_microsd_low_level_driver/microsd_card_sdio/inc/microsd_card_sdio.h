#pragma once

#include "project_config.h"

#ifdef MODULE_MICROSD_CARD_SDIO_ENABLED

#include "mc_pin.h"
#include "user_os.h"
#include "microsd_base.h"

#ifdef STM32F2
#include "stm32f2xx_hal_sd.h"
#endif

#ifdef STM32F4

#include "stm32f4xx_hal_sd.h"

#endif

#include "dma.h"
#include "mc_clk.h"

struct MicrosdSdioCfg {
    uint32_t wide;                /// SDIO_BUS_WIDE_1B, SDIO_BUS_WIDE_4B, SDIO_BUS_WIDE_8B.
    uint32_t div;
    
    DMA_Stream_TypeDef *dmaRx;                /// Из мерии DMAx_Streamx.
    uint32_t dmaRxCh;            /// Из серии DMA_CHANNEL_x.
    uint8_t dmaRxIrqPrio;
};


class MicrosdSdio : public MicrosdBase {
public:
    MicrosdSdio (const MicrosdSdioCfg *const cfg);
    
    EC_MICRO_SD_TYPE initialize (void);
    
    EC_MICRO_SD_TYPE getType (void);
    
    EC_SD_RESULT readSector (uint32_t sector,
                             uint8_t *target_array,
                             uint32_t cout_sector,
                             uint32_t timeout_ms);
    
    EC_SD_RESULT writeSector (const uint8_t *const source_array,
                              uint32_t sector,
                              uint32_t cout_sector,
                              uint32_t timeout_ms);
    
    EC_SD_STATUS getStatus (void);
    
    EC_SD_RESULT getSectorCount (uint32_t &sectorCount);
    
    EC_SD_RESULT getBlockSize (uint32_t &blockSize);
    
    void dmaRxHandler (void);
    
    void giveSemaphore (void);         // Отдать симафор из прерывания (внутренняя функция.

private:
    EC_SD_RESULT waitReadySd (void);

private:
    const MicrosdSdioCfg *const cfg;
    
    SD_HandleTypeDef handle;
    DMA_HandleTypeDef dmaRx;
    
    SemaphoreHandle_t m = nullptr;
    StaticSemaphore_t mb;
    
    StaticSemaphore_t sb;
    SemaphoreHandle_t s = nullptr;
};

#endif
