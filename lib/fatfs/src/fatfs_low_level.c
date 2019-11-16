#ifdef AYM_HARDWARE
#include "diskio.h"
#include "mc_hardware.h"
#include "freertos_headers.h"

void *ff_memalloc (UINT msize) {
    return pvPortMalloc(msize);
}

void ff_memfree (void *mblock) {
    vPortFree(mblock);
}

DWORD get_fattime () {
    return 0;
}

DSTATUS disk_initialize (__attribute__((unused))    BYTE pdrv) {
    return (init_sdio() == 0)?0:STA_NOINIT;
}

DSTATUS disk_status (__attribute__((unused)) BYTE pdrv) {
    return sdio_get_status();
}

DRESULT disk_read (__attribute__((unused)) BYTE pdrv,
                   BYTE *buff,
                   LBA_t sector,
                   UINT count) {
    if (((uint32_t)buff)%4 != 0) {
        while (1);
    }

    return (sdio_read((uint32_t*)buff, sector, count) == 0)?0:STA_NOINIT;
}

DRESULT disk_write (__attribute__((unused))    BYTE pdrv,
                    const BYTE *buff,
                    LBA_t sector,
                    UINT count) {
    if (((uint32_t)buff)%4 != 0) {
        while (1);
    }

    return (sdio_write((const uint32_t*)buff, sector, count) == 0)?0:STA_NOINIT;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff) {
    DRESULT res;

    if (disk_status(pdrv) & STA_NOINIT) {
        return RES_NOTRDY;
    }

    res = RES_ERROR;

    switch (cmd) {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT :
            if (sdio_get_sector_count(buff) == 0) {
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE :
            if (sdio_get_block_size(buff) == 0) {
                res = RES_OK;
            }
            break;

        default:
            res = RES_PARERR;
            break;
    }

    return res;
}
#endif
