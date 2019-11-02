#include "inc/ay_ym_file_play.h"

#ifdef MODULE_AY_YM_FILE_PLAY_ENABLED

/*!
 * Макрос позволяет определить, не было ли проблем на
 * более низком уровне (в методах, реализованных
 * пользователем при наследовании класса0).
 * В случае возникновения ошибки на низком уровне
 * производится возврат кода ошибки "как есть".
 */
#define CHACK_CALL_FUNC_ANSWER( r )	if ( r != 0 )	return r

/// Считываемый из бинарного файла пакет.
struct __attribute__((packed)) packetPsg {
	uint8_t		reg;
	uint8_t		data;
};

/// Длина полного заголовка в байтах.
#define HANDLER_FULL_BYTE_SIZE						16
#define HANDLER_SHORT_BYTE_SIZE						3
#define HANDLER_MARKER_POINT_BYTE					3

#define PSG_FILE_MARKER_FORMAT						0x1A
#define PSG_FILE_MARKER_INTERRUPT					0xFF
#define PSG_FILE_MARKER_FREE_INTERRUPT				0xFE
#define PSG_FILE_MARKER_END_TRACK					0xFD

#define MAX_REG_NUMBER								0xF

int AyYmFilePlayBase::psgFileParse (	PARSE_TYPE			type,
										uint32_t*			resultLongTrack	) {
	int	r;

	/// Открываем файл, который планируем воспроизводить.
	r	=	this->openFile( this->pathToFile );
	CHACK_CALL_FUNC_ANSWER( r );

	/// Проводим действия по инициализации
	/// аппаратной части (или эмулятора) только
	/// в случае, если у нас воспроизведение.
	if ( type == PARSE_TYPE::PLAY ) {
		/// Включаем чип.
		r	=	this->setPwrChip( true );
		CHACK_CALL_FUNC_ANSWER( r );

		/// Производим начальную инициализацию чипа.
		r	=	this->initChip();
		CHACK_CALL_FUNC_ANSWER( r );
	} else {
		*resultLongTrack = 0;				/// Сбрасываем счетчик прерываний.
	}

	/// В данных переменных будет храниться
	/// данные "регистр/данные".
	packetPsg			packet;

	/// Получаем длину файла.
	uint32_t	fileSize;
	r	=	this->getFileLen( fileSize );
	CHACK_CALL_FUNC_ANSWER( r );

	/// Проверяем, что длина минимум 16 байт.
	if ( fileSize < HANDLER_FULL_BYTE_SIZE ) {
		this->closeFile();
		return ENOEXEC;
	}

	/// Количество оставшихся байт в файле.
	uint32_t	countRemainingBytes;

	/// Данные начинаются с 4-го или 16-го байта.
	r	=	this->setOffsetByteInFile( HANDLER_MARKER_POINT_BYTE );		/// Проверим маркер (3-й байт).
	CHACK_CALL_FUNC_ANSWER( r );

	/// Првоеряем, полный заголовок или нет.
	uint8_t		buf;
	r	=	this->readInArray( &buf, 1 );

	/// Если этот флаг, то заголовок полный
	/// и данные начинаются с 16-го байта.
	if ( buf == PSG_FILE_MARKER_FORMAT ) {
		/// Первые 16 байт - заголовок. Пропускаем.
		countRemainingBytes = fileSize - HANDLER_FULL_BYTE_SIZE;
		r	=	this->setOffsetByteInFile( HANDLER_FULL_BYTE_SIZE );
		CHACK_CALL_FUNC_ANSWER( r );
	} else {
		/// Первые 3 байта заголвок.
		countRemainingBytes = fileSize - HANDLER_SHORT_BYTE_SIZE;
	}

	/*!
	 * Что ожидается увидеть.
	 * false		-	мы ждем регистр или маркер.
	 * true			-	данные.
	 */
	bool	expectedAppointment	=	false;

	/// Анализируем весь файл, пока не закончатся байты или не будет встречен байт окончания.
	while( countRemainingBytes ) {
		/// Считываем маркер.
		r	=	this->readInArray( &buf, 1 );
		CHACK_CALL_FUNC_ANSWER( r );
		countRemainingBytes--;

		/// Предыдущий байт был регистром - этот 100% данные.
		if ( expectedAppointment == true ) {
			expectedAppointment = false;

			/// Если обработка файла производится с целью воспроизведения.
			if ( type == PARSE_TYPE::PLAY ) {
				/// У нас считан регистр AY или левого устройства.
				if ( packet.reg <= MAX_REG_NUMBER ) {							/// Регистр для AY.
					packet.data	=	buf;
					r	=	this->writePacket( packet.reg, packet.data );
					CHACK_CALL_FUNC_ANSWER( r );
				};
			}
			continue;
		}

		/// Далее разбираем маркер или регистр.

		/*!
		 * Если файл не поврежден, тогда маркером
		 * может быть только 0xFF или 0xFE.
		 */
		if ( buf == PSG_FILE_MARKER_INTERRUPT ) {								/// 0xFF.
			/// 0xFF признак того, что произошло прерывание.
			if ( type == PARSE_TYPE::PLAY ) {
				r	=	this->sleepChip( 1 );
				CHACK_CALL_FUNC_ANSWER( r );
			} else {
				*resultLongTrack = *resultLongTrack + 1;
			}
			continue;
		};

		if ( buf == PSG_FILE_MARKER_FREE_INTERRUPT ) {								/// 0xFE.
			/// За 0xFE следует байт, который при *4 даст колличество
			/// прерываний, во время которых на AY не приходит никаких данных.
			r	=	this->readInArray( &buf, 1 );
			CHACK_CALL_FUNC_ANSWER( r );
			countRemainingBytes--;

			if ( type == PARSE_TYPE::PLAY ) {
				buf *= 4;
				r	=	this->sleepChip( buf );
				CHACK_CALL_FUNC_ANSWER( r );
			} else {
				*resultLongTrack += buf;
			}

			continue;
		}

		if ( buf == PSG_FILE_MARKER_END_TRACK ) {				/// Флаг конца трека.
			break;
		}

		/// Иначе это регистр.
		packet.reg				=	buf;
		expectedAppointment		=	true;						/// Ждем данные.
	}

	/// После воспроизведения отключаем чип.
	if ( type == PARSE_TYPE::PLAY ) {
		/// Отключаем чип.
		r	=	this->setPwrChip( false );
		CHACK_CALL_FUNC_ANSWER( r );
	}

	/// Закрываем файл.
	r	=	this->closeFile();
	CHACK_CALL_FUNC_ANSWER( r );

	return EOK;
}


int AyYmFilePlayBase::psgFilePlay ( std::shared_ptr< char >		fullFilePath ) {
	this->pathToFile	=	fullFilePath;

	return this->psgFileParse( PARSE_TYPE::PLAY, nullptr );
}

int AyYmFilePlayBase::psgFileGetLong ( std::shared_ptr< char >		fullFilePath,
										uint32_t& resultLong ) {
	this->pathToFile	=	fullFilePath;

	return this->psgFileParse( PARSE_TYPE::GET_LONG, &resultLong );
}

#endif

