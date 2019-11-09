#pragma once

#ifdef MODULE_MICROSD_CARD_SPI_ENABLED

#include "mc_spi.h"
#include "mc_pin.h"
#include "user_os.h"
#include "microsd_base.h"

struct microsdSpiCfg {
    PinBase*					const cs;			 // Вывод CS, подключенный к microsd.
    SpiMaster8BitBase*			const s;

    /// speed ( true == fast )
    /// speed ( false == true )
    void	( *setSpiSpeed )	( SpiMaster8BitBase* spi, bool speed );
};

class MicrosdSpi : public MicrosdBase {
public:
    MicrosdSpi ( const microsdSpiCfg* const cfg );

    EC_MICRO_SD_TYPE	initialize					( void );
    EC_MICRO_SD_TYPE	getType						( void );
    EC_SD_RESULT		readSector					( uint32_t sector, uint8_t *target_array, uint32_t cout_sector, uint32_t timeout_ms  );
    EC_SD_RESULT		writeSector					( const uint8_t* const source_array, uint32_t sector, uint32_t cout_sector, uint32_t timeout_ms  );
    EC_SD_STATUS		getStatus					( void );
    EC_SD_RESULT		getSectorCount				( uint32_t& sectorCount );
    EC_SD_RESULT		getBlockSize				( uint32_t& blockSize );
private:
    // Переключение CS.
    void			csLow							( void );		 // CS = 0, GND.
    void			csHigh							( void );		 // CS = 1, VDD.

    // Передать count пустых байт (шлем 0xFF).
    EC_SD_RES	sendEmptyPackage					( const uint16_t count );

    // Передача 1 пустого байта. Требуется после каждой команды для ОЧЕНЬ старых карт.
    EC_SD_RES	sendWaitOnePackage					( void );

    // Пропускаем count приших байт.
    EC_SD_RES	losePackage							( const uint16_t count );

    // Считывает приходящий пакет в буффер.
    EC_SD_RES	readDataPackage						( uint8_t* buf, const uint16_t count );

    // Ждем от команды специального маркера.
    EC_SD_RES	waitMark							( const uint8_t mark );

    // Сами отправляем маркер.
    EC_SD_RES	sendMark							( const uint8_t mark );

    // Просто передача команды.
    EC_SD_RES	sendCmd								( const uint8_t cmd, const uint32_t arg, const uint8_t crc );

    // Получаем адресс сектора (для аргумента команды чтения/записи).
    uint32_t	getArgAddress						( const uint32_t sector );

    // Отправить ACMD.
    EC_SD_RES	sendAcmd							( const uint8_t acmd, const uint32_t arg, const uint8_t crc );


    // Ждать R1 (если r1 != nullptr, то еще вернуть R1 ).
    EC_SD_RES	waitR1								( uint8_t* r1 = nullptr );

    EC_SD_RES	waitR2								( uint16_t* const r2 );

    // Принимаем R3 (регистр OCR).
    EC_SD_RES	waitR3								( uint32_t* const r3 );

    // Принимаем r7.
    EC_SD_RES	waitR7								( uint32_t* const r7 );

    void		generateCrcTable					( void );
    uint8_t		getCrc7								( const uint8_t cmd, const uint32_t commandData );

    const microsdSpiCfg*			const cfg;

    StaticSemaphore_t		mb;
    SemaphoreHandle_t			m				= nullptr;

    EC_MICRO_SD_TYPE				typeMicrosd		= EC_MICRO_SD_TYPE::ERROR;			 // Тип microSD.

    uint8_t							CRCTable[256];
};

#endif
