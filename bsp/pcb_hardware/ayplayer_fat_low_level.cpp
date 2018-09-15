#include "ff.h"
#include "diskio.h"
#include "user_os.h"
#include "ffconf.h"
#include "microsd_card_spi.h"
#include "microsd_card_sdio.h"

extern MicrosdSdio						sd;

#define NUMBER_OF_ATTEMPTS				30

extern "C" {

/// Allocate memory block.
void* ff_memalloc ( UINT msize ) {
	return pvPortMalloc( msize );
}

/// Free memory block
void ff_memfree ( void* mblock ) {
	vPortFree( mblock );
}

DWORD get_fattime ( void ) {
	return 0;
}

DSTATUS disk_initialize ( __attribute__((unused))	BYTE pdrv ) {
	if ( sd.initialize() != EC_MICRO_SD_TYPE::ERROR ) {
		return 0;
	} else {
		return STA_NOINIT;
	}
}

DSTATUS disk_status ( __attribute__((unused)) BYTE pdrv ) {
	return ( DSTATUS )sd.getStatus();
}

DRESULT disk_read (	__attribute__((unused))	BYTE pdrv,
					BYTE* buff,
					DWORD sector,
					UINT count ) {
	DRESULT funcR;
	uint32_t l = NUMBER_OF_ATTEMPTS;

	do {
		funcR = ( DRESULT )sd.readSector( sector, buff, count, 1000 );
		if ( funcR != RES_OK ) {
			l--;
			USER_OS_DELAY_MS(1);
		}
	} while ( ( funcR != RES_OK ) && ( l != 0 ) );

	return funcR;
}

DRESULT disk_write ( __attribute__((unused))	BYTE pdrv,
						const BYTE* buff,
						DWORD sector,
						UINT count ) {
	DRESULT funcR;
	uint32_t l = NUMBER_OF_ATTEMPTS;

	do {
		funcR = ( DRESULT )sd.writeSector( buff, sector, count, 1000 );
		if ( funcR != RES_OK ) {
			l--;
			USER_OS_DELAY_MS(1);
		}
	} while ( ( funcR != RES_OK ) && ( l != 0 ) );

	return funcR;
}

DRESULT disk_ioctl ( BYTE pdrv, BYTE cmd, void* buff ) {
	DRESULT res;

	if ( disk_status( pdrv ) & STA_NOINIT )
		return DRESULT::RES_NOTRDY;				/// Check if drive is ready.

	res = DRESULT::RES_ERROR;

	switch ( cmd ) {

	/// Буффер не используется (пишем/читаем напрямую).
	case CTRL_SYNC:
		res = DRESULT::RES_OK;
		break;

	/// Возвращает количество секторов на карте памяти.
	case GET_SECTOR_COUNT :
		if ( sd.getSectorCount( *( ( uint32_t* )buff ) ) == EC_SD_RESULT::OK )
			res = DRESULT::RES_OK;
		break;

	case GET_SECTOR_SIZE :
		if ( sd.getBlockSize( *( ( uint32_t* )buff ) ) == EC_SD_RESULT::OK )
			res = DRESULT::RES_OK;
		break;

	default:
		res = DRESULT::RES_PARERR;
	}

	return res;
}

#if FF_FS_REENTRANT

int ff_cre_syncobj ( BYTE vol,  FF_SYNC_t *sobj ) {
	int ret;
	static FF_SYNC_t								mutex[ FF_VOLUMES ];
	static USER_OS_STATIC_BIN_SEMAPHORE_BUFFER		mutex_buf[ FF_VOLUMES ];
	if (!mutex[vol])
	mutex[vol] = USER_OS_STATIC_MUTEX_CREATE( &mutex_buf[vol] );
	*sobj = mutex[vol];
	ret = (*sobj != NULL);

	return ret;
}

int ff_del_syncobj ( FF_SYNC_t sobj ) {
	( void )sobj;
	USER_OS_STATIC_MUTEX_DELETE(sobj);
	return 1;
}

int ff_req_grant ( FF_SYNC_t sobj ) {
	int ret;
	ret = (USER_OS_TAKE_MUTEX( sobj, FF_FS_TIMEOUT ) == pdTRUE);
	return ret;
}

void ff_rel_grant ( FF_SYNC_t sobj ) {
	USER_OS_GIVE_MUTEX(sobj);
}

#endif

}
