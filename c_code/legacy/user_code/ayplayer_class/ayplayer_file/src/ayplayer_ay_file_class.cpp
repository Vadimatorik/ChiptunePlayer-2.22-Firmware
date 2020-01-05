#include "ayplayer_ay_file_class.h"

AyYmFilePlay::AyYmFilePlay( const AyYmFilePlayCfg* const cfg ) :
		cfg( cfg ) {
}

/*
void AyYmFilePlay::setUsingChip ( uint32_t chipNumber ) {
	this->usingChip = chipNumber;
}*/



void AyYmFilePlay::setPause ( bool state ) {
	this->cfg->ayLow->playStateSet( state );
}

void AyYmFilePlay::stop ( void ) {
	this->flagStop	=	true;
}


void AyYmFilePlay::resetFlags ( void ) {
	this->flagStop		=	false;

	this->pointStartSeekBuffer		=	0;
	this->pointInBuffer				=	0;
}

int AyYmFilePlay::get_len_file (uint32_t &returnFileLenByte) {
	returnFileLenByte = this->fat.getFileSize( this->file );
	return EOK;
}

int AyYmFilePlay::open_file (std::shared_ptr<char> fullFilePath) {
	this->resetFlags();

	int r;

	this->file = this->fat.openFile( fullFilePath, r );
	errnoCheckAndReturn( r );


	uint32_t countByteInTreck;
	countByteInTreck = this->fat.getFileSize( this->file );

	if ( countByteInTreck < 16 )
		return ENOEXEC;

	UINT getCountByte;
	if ( countByteInTreck < AY_YM_FILE_PLAY_FLASH_BUFFER_SIZE_BYTE ) {
		getCountByte =	countByteInTreck;
	} else {
		getCountByte =	AY_YM_FILE_PLAY_FLASH_BUFFER_SIZE_BYTE;
	}

	r = this->fat.readFromFile( this->file, this->flashBuffer, getCountByte );
	if ( r != EOK ) {
		return this->fat.closeFile( this->file );
	}

	return EOK;
}

int AyYmFilePlay::close_file (void) {
	while( this->cfg->ayLow->queueEmptyCheck() != true ) {			/// Ждем, пока AY освободится.
		vTaskDelay(20);
	}

	this->cfg->ayLow->playStateSet( 0 );								/// Отключаем аппаратку.
    this->set_pwr_chip(false);									/// Питание чипа.
	this->cfg->ayLow->queueClear();									/// Чистим очередь.

	return this->fat.closeFile( this->file );
}

int AyYmFilePlay::set_pwr_chip (bool state) {
	this->cfg->setPwr( this->usingChip, state );
	return 0;
}

int AyYmFilePlay::init_chip (void) {
	this->cfg->ayLow->queueClear();
	this->cfg->ayLow->hardwareClear();
	this->cfg->ayLow->playStateSet( 1 );
	return 0;
}

int AyYmFilePlay::sleep (const uint32_t countTick) {
	int r;
	for ( uint32_t i = 0; i < countTick; i++ ) {
		r = this->set_reg(0xFF, 0);			/// 0xFF - знак паузы.
		if ( r != 0 )								/// Если была остановка или проблемы на нижнем уровне...
			return r;								///  - выходим.
	}
	return 0;
}

int AyYmFilePlay::set_reg (const uint8_t reg,
                           const uint8_t data) {
	ayQueueStruct		s;
	s.data			=	data;
	s.reg			=	reg;
	s.numberChip	=	static_cast< uint8_t >( this->usingChip );

	while( 1 ) {
		/// Если нет, снова смотрим флаги и ждем.
		if ( this->flagStop ) {
			this->abort();
			return -1;
		}

		if ( this->cfg->ayLow->queueAddElement( &s ) == 0 )
			return 0;		/// Место в очереди есть, все хорошо.
	}
}

void AyYmFilePlay::abort ( void ) {
	this->cfg->ayLow->playStateSet( 0 );				/// Отключаем аппаратку.
    this->set_pwr_chip(false);					/// Питание чипа.
	this->cfg->ayLow->queueClear();					/// Чистим очередь.
	this->fat.closeFile( this->file );			///	Хотя бы удаляем FIL-структуру.
}


