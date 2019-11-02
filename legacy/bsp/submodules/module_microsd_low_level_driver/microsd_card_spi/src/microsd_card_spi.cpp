#include "microsd_card_spi.h"

#ifdef MODULE_MICROSD_CARD_SPI_ENABLED

#define CMD0		( 0x40 )														// Программный сброс.
#define CMD1		( 0x40 + 1)														// Инициировать процесс инициализации.
#define CMD8		( 0x40 + 8 )													// Уточнить поддерживаемое нарпряжение.
#define CMD9		( 0x40 + 9 )													// Спрашивает у карты её информацию "о карте" (CSD).

#define CMD13		( 0x40 + 13 )													// Статус карты, если вставлена.
#define CMD16		( 0x40 + 16 )													// Размер физического блока.
#define CMD17		( 0x40 + 17 )													// Считать блок.
#define CMD24		( 0x40 + 24 )													// Записать блок.
#define CMD55		( 0x40 + 55 )													// Указание, что далее ACMD.
#define CMD58		( 0x40 + 58 )													// Считать OCR регистр карты.

#define ACMD13		( 0x40 + 13 )													// Статус карты.
#define ACMD41		( 0x40 + 41 )													// Инициировать процесс инициализации.
#define ACMD55		( 0x40 + 55 )													// Инициировать процесс инициализации.

#define CMD17_MARK	( 0b11111110 )
#define CMD24_MARK	( 0b11111110 )


MicrosdSpi::MicrosdSpi ( const microsdSpiCfg* const cfg ) : cfg( cfg ) {
    this->m = USER_OS_STATIC_MUTEX_CREATE( &this->mb );
    this->generateCrcTable();
}

//**********************************************************************
// Низкоуровневые функции.
//**********************************************************************

// Управление линией CS.
void MicrosdSpi::csLow ( void ) {
    this->cfg->cs->reset();
}

void MicrosdSpi::csHigh ( void ) {
    this->cfg->cs->set();
}

// Передать count 0xFF.
EC_SD_RES MicrosdSpi::sendEmptyPackage ( const uint16_t count ) {
    if ( this->cfg->s->txOneItem( 0xFF, count, 10 ) == BASE_RESULT::OK ) {
        return EC_SD_RES::OK;
    } else {
        return EC_SD_RES::IO_ERROR;
    }
}

EC_SD_RES MicrosdSpi::readDataPackage ( uint8_t* buf, const uint16_t count ) {
    this->csLow();
    BASE_RESULT r = this->cfg->s->rx( buf, count, 10, 0xFF );
    this->csHigh();
    if ( r == BASE_RESULT::OK ) {
        return EC_SD_RES::OK;
    } else {
        return EC_SD_RES::IO_ERROR;
    }
}

// Передача одного 0xFF. Требуется после каждой команды для ОЧЕНЬ старых карт.
EC_SD_RES MicrosdSpi::sendWaitOnePackage ( void ) {
    this->csLow();
    EC_SD_RES r = this->sendEmptyPackage( 1 );
    this->csHigh();
    return r;
}

// Ждем от карты "маркер"
// - специальный байт, показывающий, что далее идет команда/данные.
EC_SD_RES MicrosdSpi::waitMark ( uint8_t mark ) {
    EC_SD_RES	r = EC_SD_RES::TIMEOUT;

    this->csLow();

    for ( int l_d = 0; l_d < 3; l_d++ ) {												// До 3 мс даем возможность карте ответить.
        for ( int loop = 0; loop < 10; loop++ ) {
            uint8_t input_buf;

            if ( this->cfg->s->rx( &input_buf, 1, 10, 0xFF ) != BASE_RESULT::OK ) {
                r = EC_SD_RES::IO_ERROR;
                break;
            }

            if ( input_buf == mark ) {
                r = EC_SD_RES::OK;
                break;
            }
        }
        if ( r != EC_SD_RES::TIMEOUT ) break;
        USER_OS_DELAY_MS(1);
    }

    this->csHigh();

    return r;
}

// Пропустить n байт.
EC_SD_RES MicrosdSpi::losePackage ( uint16_t count ) {
    this->csLow();
    EC_SD_RES r = this->sendEmptyPackage( count );
    this->csHigh();
    return r;
}

EC_SD_RES MicrosdSpi::sendCmd ( uint8_t cmd, uint32_t arg, uint8_t crc ) {
    this->csLow();

    uint8_t output_package[6];
    output_package[0] = cmd;
    output_package[1] = ( uint8_t )( arg >> 24 );
    output_package[2] = ( uint8_t )( arg >> 16 );
    output_package[3] = ( uint8_t )( arg >> 8 );
    output_package[4] = ( uint8_t )( arg );
    output_package[5] = crc;

    EC_SD_RES r = EC_SD_RES::OK;
    if ( this->cfg->s->tx( output_package, 6, 10 ) != BASE_RESULT::OK ) {
        r = EC_SD_RES::IO_ERROR;
    }

    this->csHigh();

    return r;
}

// Сами отправляем маркер (нужно, например, для записи).
EC_SD_RES MicrosdSpi::sendMark ( uint8_t mark ) {
    this->csLow();

    EC_SD_RES r = EC_SD_RES::OK;
    if ( this->cfg->s->tx( &mark, 1, 10 ) != BASE_RESULT::OK ) {
        r = EC_SD_RES::IO_ERROR;
    }

    this->csHigh();

    return r;
}

// Ожидаем R1 (значение R1 нам не нужно).
EC_SD_RES MicrosdSpi::waitR1 ( uint8_t* r1 ) {
    this->csLow();
    EC_SD_RES	r = EC_SD_RES::TIMEOUT;

    // Карта должна принять команду в течении 3 обращений (чаще всего на 2-й итерации).
    for ( int loop = 0; loop < 3; loop++ ) {
        uint8_t input_buf;

        if ( this->cfg->s->rx( &input_buf, 1, 10, 0xFF ) != BASE_RESULT::OK ) {
            r = EC_SD_RES::IO_ERROR;
            break;
        }

        // Сброшенный старший бит символизирует успешное принятие R1 ответа.
        if ( ( input_buf & ( 1 << 7 ) ) == 0 ) {
            if ( r1 != nullptr ) {
                *r1 = input_buf;
            }
            r = EC_SD_RES::OK;
            break;
        }
    }

    this->csHigh();
    return r;
}

#define R1_ILLEGAL_COMMAND_MSK		( 1 << 2 )

// Ждем R3 (регистр OCR).
EC_SD_RES MicrosdSpi::waitR3 ( uint32_t* r3 ) {
    uint8_t r1;
    EC_SD_RES r = this->waitR1( &r1 );

    if ( r == EC_SD_RES::OK ) {
        if ( r1 & R1_ILLEGAL_COMMAND_MSK ) {
            return EC_SD_RES::R1_ILLEGAL_COMMAND;
        }
    } else {
        return r;
    }

    this->csLow();

    uint8_t buf_u8[4];

    if ( this->cfg->s->rx( buf_u8, 4, 10, 0xFF ) != BASE_RESULT::OK ) {
        r = EC_SD_RES::IO_ERROR;
    }

    uint32_t buf_u32 = 0;
    buf_u32 |= buf_u8[ 0 ] << 24;
    buf_u32 |= buf_u8[ 1 ] << 16;
    buf_u32 |= buf_u8[ 2 ] << 8;
    buf_u32 |= buf_u8[ 3 ];

    *r3 = buf_u32;

    this->csHigh();

    return r;
}

EC_SD_RES MicrosdSpi::waitR2 ( uint16_t* r2 ) {
    uint8_t r1;
    EC_SD_RES r = this->waitR1( &r1 );

    if ( r == EC_SD_RES::OK ) {
        if ( r1 & R1_ILLEGAL_COMMAND_MSK ) {
            return EC_SD_RES::R1_ILLEGAL_COMMAND;
        }
    } else {
        return r;
    }

    this->csLow();

    uint8_t buf_u8;

    if ( this->cfg->s->rx( &buf_u8, 1, 10, 0xFF ) != BASE_RESULT::OK ) {
        r = EC_SD_RES::IO_ERROR;
    }

    uint16_t buf_u16 = 0;
    buf_u16 |= r1 << 8;
    buf_u16 |= buf_u8;

    *r2 = buf_u16;

    this->csHigh();

    return r;
}


// Ждем ответа r7.
EC_SD_RES MicrosdSpi::waitR7 ( uint32_t* r7 ) {
    return this->waitR3( r7 );	// Структура r3 и r7 идентичны по формату. Так экономим память.
}

void MicrosdSpi::generateCrcTable( void ) {
    int i, j;
    uint8_t CRCPoly = 0x89;	// the value of our CRC-7 polynomial

    // generate a table value for all 256 possible byte values
    for (i = 0; i < 256; ++i) {
    this->CRCTable[i] = (i & 0x80) ? i ^ CRCPoly : i;
    for (j = 1; j < 8; ++j) {
        this->CRCTable[i] <<= 1;
        if ( this->CRCTable[i] & 0x80 )
            this->CRCTable[i] ^= CRCPoly;
    }
    }
}

uint8_t MicrosdSpi::getCrc7 ( const uint8_t cmd, const uint32_t commandData ) {
    uint8_t CRC = 0;

    CRC = CRCTable[(CRC << 1) ^ cmd];

    uint8_t* message = ( uint8_t* )&commandData;

    for ( int i = 0; i < 4; ++i )
        CRC = CRCTable[(CRC << 1) ^ message[ 3 - i ]];

    CRC = ( CRC << 1 ) | 1;
    return CRC;
}

// Передаем CMD55,
// Дожидаемся сообщения об успешном принятии.
// Если не удалось принять - информируем об ошибке и выходим.
EC_SD_RES MicrosdSpi::sendAcmd ( uint8_t acmd, uint32_t arg, uint8_t crc ) {
    EC_SD_RES r = this->sendCmd( CMD55, 0, this->getCrc7( CMD55, 0 ) );
    if ( r != EC_SD_RES::OK )				return r;

    uint8_t r1;
    r = this->waitR1( &r1 );
    if ( r != EC_SD_RES::OK )				return r;
    if ( r1 & ~(0x01) )						return EC_SD_RES::R1_ILLEGAL_COMMAND;

    r = this->sendCmd( acmd, arg, crc );
    return r;
}

// Получая сектор, возвращает адресс, который следует отправить с параметром карты.
// Иначе говоря, в зависимости от типа адресации либо возвращает тот же номер сектора,
// либо номер первого байта.
uint32_t MicrosdSpi::getArgAddress ( uint32_t sector ) {
    if ( !( ( uint32_t )this->typeMicrosd & ( uint32_t )EC_MICRO_SD_TYPE::BLOCK ) )
        sector *= 512;
    return sector;
}

//**********************************************************************
// Основной функционал.
//**********************************************************************
#define R1_ILLEGAL_COMMAND_MSK				( 1 << 2 )
#define R1_IN_IDLE_STATE_MSK				( 1 << 0 )
#define ACMD41_HCS_MSK						( 1 << 30 )
#define OCR_CCS_MSK							( 1 << 30 )

// Определяем тип карты и инициализируем ее.
EC_MICRO_SD_TYPE MicrosdSpi::initialize ( void ) {
    uint8_t			r1;
    EC_SD_RES		sendResult;

    USER_OS_TAKE_MUTEX( this->m, portMAX_DELAY );
    this->cfg->setSpiSpeed( this->cfg->s, false );
    this->typeMicrosd = EC_MICRO_SD_TYPE::ERROR;

    this->sendEmptyPackage( 10 );

    do {
        if ( this->sendCmd( CMD0, 0, 0x95 )		!= EC_SD_RES::OK )					break;
        if ( this->waitR1()						!= EC_SD_RES::OK )					break;
        if ( this->sendCmd( CMD8, 0x1AA, 0x87 )	!= EC_SD_RES::OK )					break;

        if ( this->waitR1( &r1 )				!= EC_SD_RES::OK )					break;

        uint32_t timer = 300;

        /// CMD8 поддерживается.
        if ( !( r1 & R1_ILLEGAL_COMMAND_MSK ) ) {
            uint8_t	ocr[4];
            if ( this->readDataPackage( ocr, 4 ) != EC_SD_RES::OK )					break;
            if ( !( ocr[2] == 0x01 && ocr[3] == 0xAA ) )							break;

            while ( timer ) {
                sendResult = this->sendAcmd( ACMD41, 1UL << 30, this->getCrc7( ACMD41, 1UL << 30 ) );
                if ( sendResult != EC_SD_RES::OK ) {
                    timer = 0;
                    break;
                }

                if ( this->waitR1( &r1 ) != EC_SD_RES::OK ) {
                    timer = 0;
                    break;
                }

                timer--;

                if ( r1 == 0 ) {
                    break;
                }
            }

            if ( timer == 0 ) break;

            sendResult = this->sendCmd( CMD58, 0, this->getCrc7( CMD58, 0 ) );
            if ( this->waitR1( &r1 ) != EC_SD_RES::OK ) {
                timer = 0;
                break;
            }

            if ( this->readDataPackage( ocr, 4 ) != EC_SD_RES::OK ) {
                timer = 0;
                break;
            }

            if ( ocr[0] & 0x40 ) {
                this->typeMicrosd = ( EC_MICRO_SD_TYPE )( ( uint32_t )EC_MICRO_SD_TYPE::SD2 | ( uint32_t )EC_MICRO_SD_TYPE::BLOCK );
            } else {
                this->typeMicrosd = EC_MICRO_SD_TYPE::SD2;
            }

        } else {
            sendResult = this->sendAcmd( ACMD41, 1UL << 30, this->getCrc7( ACMD41, 1UL << 30 ) );
            if ( sendResult != EC_SD_RES::R1_ILLEGAL_COMMAND ) {
                this->typeMicrosd = EC_MICRO_SD_TYPE::SD1;
                while ( timer ) {
                    sendResult = this->sendAcmd( ACMD41, 0, this->getCrc7( ACMD41, 0 ) );

                    if ( sendResult != EC_SD_RES::OK ) {
                        timer = 0;
                        break;
                    }

                    if ( this->waitR1( &r1 ) != EC_SD_RES::OK ) {
                        timer = 0;
                        break;
                    }

                    if ( r1 != 0 ) {
                        continue;
                    }

                    timer--;
                }

                if ( timer == 0 ) break;

                if ( this->sendCmd( CMD16, 0, this->getCrc7( CMD16, 0 ) ) != EC_SD_RES::OK )
                    break;

                if ( this->waitR1() != EC_SD_RES::OK ) {
                    this->typeMicrosd = EC_MICRO_SD_TYPE::ERROR;
                    break;
                }
            }
        }
    } while( false );

    // Теперь с SD можно работать на высоких скоростях.
    if ( this->typeMicrosd != EC_MICRO_SD_TYPE::ERROR ) {
        this->cfg->setSpiSpeed( this->cfg->s, true );
    }

    USER_OS_GIVE_MUTEX( this->m );

    return this->typeMicrosd;
}

EC_SD_STATUS MicrosdSpi::getStatus ( void ) {
    if ( this->getType() == EC_MICRO_SD_TYPE::ERROR ) {
        return EC_SD_STATUS::NOINIT;
    }

    if ( this->sendCmd( CMD1, 0, this->getCrc7( CMD0, 0 ) )	!= EC_SD_RES::OK ) {
        return EC_SD_STATUS::NOINIT;
    }

    uint8_t r1;
    if ( this->waitR1( &r1 )	!= EC_SD_RES::OK ) {
        return EC_SD_STATUS::NOINIT;
    }

    if ( r1 != 0 ) {
        return EC_SD_STATUS::NOINIT;
    }

    return EC_SD_STATUS::OK;
}

EC_MICRO_SD_TYPE MicrosdSpi::getType ( void ) {
    return this->typeMicrosd;
}

// Считать сектор.
// dst - указатель на массив, куда считать 512 байт.
// sector - требуемый сектор, с 0.
// Предполагается, что с картой все хорошо (она определена, инициализирована).

EC_SD_RESULT MicrosdSpi::readSector ( uint32_t sector, uint8_t *target_array, uint32_t cout_sector, uint32_t timeout_ms	) {
    ( void )timeout_ms;

/// В релизе не должно быть такой ситуации,
/// чтобы указатель был не выравнен.
#ifdef DEBUG
    /// Указатель должен быть выравнен на 4.
    if ( ( uint32_t )target_array & 0b11 ) {
        while(1);
    }
#endif

    uint32_t address;

    this->cfg->setSpiSpeed( this->cfg->s, true );

    EC_SD_RESULT r = EC_SD_RESULT::ERROR;


    USER_OS_TAKE_MUTEX( this->m, portMAX_DELAY );

    uint8_t* p_buf = target_array;

    do {
        address = this->getArgAddress( sector );									// В зависимости от типа карты - адресация может быть побайтовая или поблочная
                                                                                    // (блок - 512 байт).

        if ( this->sendCmd( CMD17, address, this->getCrc7( CMD17, address ) )	 != EC_SD_RES::OK ) break;			// Отправляем CMD17.
        uint8_t r1;
        if ( this->waitR1( &r1 )					!= EC_SD_RES::OK ) break;
        if ( r1 != 0 ) break;
        if ( this->waitMark( CMD17_MARK )			!= EC_SD_RES::OK ) break;

        // Считываем 512 байт.
        this->csLow();

        if ( this->cfg->s->rx( p_buf, 512, 100, 0xFF )		!= BASE_RESULT::OK )	break;
        uint8_t crc_in[2] = {0xFF, 0xFF};

        if ( this->cfg->s->rx( crc_in, 2, 10, 0xFF )		!= BASE_RESULT::OK )	break;
        if ( this->sendWaitOnePackage()						!= EC_SD_RES::OK )		break;

        cout_sector--;						// cout_sector 1 сектор записали.

        if ( cout_sector == 0 ) {
            r = EC_SD_RESULT::OK;
            break;
        } else {
            sector++;							// Будем писать следующий сектор.
            p_buf += 512;						// 512 байт уже записали.
        }

    }	while ( true );

    this->csHigh();

    USER_OS_GIVE_MUTEX( this->m );

    return r;
}

// Записать по адресу address массив src длинной 512 байт.
EC_SD_RESULT MicrosdSpi::writeSector ( const uint8_t* const source_array, uint32_t sector, uint32_t cout_sector, uint32_t timeout_ms	) {
    ( void )timeout_ms;

    /// В релизе не должно быть такой ситуации,
    /// чтобы указатель был не выравнен.
#ifdef DEBUG
    /// Указатель должен быть выравнен на 4.
    if ( ( uint32_t )source_array & 0b11 ) {
        while(1);
    }
#endif

    uint32_t address;

    this->cfg->setSpiSpeed( this->cfg->s, true );

    EC_SD_RESULT r = EC_SD_RESULT::ERROR;

    USER_OS_TAKE_MUTEX( this->m, portMAX_DELAY );

    uint8_t* p_buf = ( uint8_t* )source_array;

    do {
        address = this->getArgAddress( sector );		// В зависимости от типа карты - адресация может быть побайтовая или поблочная
                                                            // (блок - 512 байт).

        if ( this->sendCmd( CMD24, address, this->getCrc7( CMD24, address ) )	!= EC_SD_RES::OK )
            break;					// Отправляем CMD24.

        uint8_t r1;
        if ( this->waitR1( &r1 )								!= EC_SD_RES::OK ) break;
        if ( r1 != 0 ) break;
        if ( this->sendWaitOnePackage()						!= EC_SD_RES::OK ) break;					// Обязательно ждем 1 пакет.
        if ( this->sendMark( CMD24_MARK )						!= EC_SD_RES::OK ) break;

        // Пишем 512 байт.
        this->csLow();

        if ( this->cfg->s->tx( p_buf, 512, 100 )	!= BASE_RESULT::OK )	break;
        uint8_t crc_out[2] = { 0 };						// Отправляем любой CRC.
        if ( this->cfg->s->tx( crc_out, 2, 100 )			!= BASE_RESULT::OK )	break;
        // Сразу же должен прийти ответ - принята ли команда записи.
        uint8_t answer_write_commend_in;
        if ( this->cfg->s->rx( &answer_write_commend_in, 1, 10, 0xFF ) != BASE_RESULT::OK )	break;
        if ( ( answer_write_commend_in & ( 1 << 4 ) ) != 0 ) break;
        answer_write_commend_in &= 0b1111;
        if ( answer_write_commend_in != 0b0101 )			 break; // Если не успех - выходим.
        // Ждем окончания записи.
        uint8_t write_wait = 0;
        while ( write_wait == 0 ) {
            if ( this->cfg->s->rx( &write_wait, 1, 10, 0xFF ) != BASE_RESULT::OK ) break;
        }
        if ( write_wait == 0 ) break;
        if ( this->sendWaitOnePackage() != EC_SD_RES::OK ) break;

        cout_sector--;						// cout_sector 1 сектор записали.

        if ( cout_sector == 0 ) {
            r = EC_SD_RESULT::OK;
            break;
        } else {
            sector++;							// Будем писать следующий сектор.
            p_buf += 512;						// 512 байт уже записали.
        }
    } while ( true );

    this->csHigh();

    USER_OS_GIVE_MUTEX( this->m );

    return r;
}

EC_SD_RESULT MicrosdSpi::getSectorCount ( uint32_t& sectorCount ) {
    if ( this->sendCmd( CMD9, 0, this->getCrc7( CMD9, 0  ) ) != EC_SD_RES::OK )
        return EC_SD_RESULT::ERROR;

    uint8_t	csd[16];
    if ( this->readDataPackage( csd, 16 ) != EC_SD_RES::OK )
        return EC_SD_RESULT::ERROR;

    uint32_t csize;
    if ( ( csd[0] >> 6 ) == 1) {	// SDC ver 2.00
        csize = csd[9] + ((uint32_t)csd[8] << 8) + ((uint32_t)(csd[7] & 63) << 16) + 1;
        sectorCount= csize << 10;
    } else {						// SDC ver 1.XX or MMC ver 3
        uint32_t n;
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((uint32_t)csd[7] << 2) + ((uint32_t)(csd[6] & 3) << 10) + 1;
        sectorCount = csize << (n - 9);
    }

    return EC_SD_RESULT::OK;
}

EC_SD_RESULT MicrosdSpi::getBlockSize ( uint32_t& blockSize ) {
    /// Метод не работает нормально!
    if ( ( uint32_t )this->typeMicrosd & ( uint32_t )EC_MICRO_SD_TYPE::SD2 ) {		// SDC ver 2.00
        if ( this->sendAcmd( ACMD13, 0, this->getCrc7( ACMD13, 0 ) ) != EC_SD_RES::OK )			// Read SD status
            return EC_SD_RESULT::ERROR;

        if ( this->waitR1()	!= EC_SD_RES::OK )
            return EC_SD_RESULT::ERROR;

        uint8_t	csd[16];
        if ( this->readDataPackage( csd, 16 ) != EC_SD_RES::OK )		// Read partial block
            return EC_SD_RESULT::ERROR;

        this->sendEmptyPackage( 64 - 16 );								// Purge trailing data
        blockSize = 16UL << (csd[10] >> 4);

    } else {															// SDC ver 1.XX or MMC
        if ( this->sendCmd( CMD9, 0, this->getCrc7( CMD9, 0 )  ) != EC_SD_RES::OK )
            return EC_SD_RESULT::ERROR;

        uint8_t	csd[16];
        if ( this->readDataPackage( csd, 16 ) != EC_SD_RES::OK )
            return EC_SD_RESULT::ERROR;

        if ( ( uint32_t )this->typeMicrosd & ( uint32_t )EC_MICRO_SD_TYPE::SD1 ) {			// SDC ver 1.XX
            blockSize = (((csd[10] & 63) << 1) + ((uint32_t)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
        } else {														// MMC
            blockSize = ((uint32_t)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
        }
    }

    return EC_SD_RESULT::OK;
}

#endif
