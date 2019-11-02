#include "../inc/ay_ym_note_mode.h"

#ifdef MODULE_AY_YM_NOTE_MODE_ENABLED

// Массив делителей для формирования совместимых с фортепианными звуками нот.

/*
// Включить/выключить 1 канал одного из чипов. Через очередь.

void ay_ym_low_lavel::set_channel ( uint8_t number_ay, uint8_t channel, bool set ) {
    if ( set ) {                                                                // Если включить канал.
        this->cfg->r7_reg[number_ay] &= ~( 1 << channel);
    } else {
        this->cfg->r7_reg[number_ay] |= 1 << channel;                                // Если отключить.
    }
    ay_queue_struct buf;
    buf.data = this->cfg->r7_reg[number_ay];
    buf.reg  = 7;
    buf.number_chip = number_ay;
    this->queue_add_element( &buf );    // Выбираем R7.
}*/

int AyYmNote::reinit ( uint8_t chipNumber ) {
	ayQueueStruct bufData;
	bufData.numberChip  = chipNumber;
	bufData.reg = 7;
	bufData.data = 0b111000;
	cfg->array_chip->queueAddElement( &bufData );

    return 0;
}

int AyYmNote::write_note_to_channel ( uint8_t chip_number, uint8_t channel, uint8_t note ) {
    uint8_t older_byte  = (uint8_t)( this->array_divider_chip[note] >> 8 );
    uint8_t junior_byte = (uint8_t)this->array_divider_chip[note];

    ayQueueStruct buf_data;
    buf_data.numberChip  = chip_number;

    switch( channel ) {      // Записываем ноту в выбранный канал.
    case 0:
            buf_data.reg = 0;  buf_data.data         = junior_byte;
            cfg->array_chip->queueAddElement( &buf_data );

            buf_data.reg = 1;  buf_data.data         = older_byte;
            cfg->array_chip->queueAddElement( &buf_data );
            break;

    case 1:
            buf_data.reg = 2;  buf_data.data         = junior_byte;
            cfg->array_chip->queueAddElement( &buf_data );

            buf_data.reg = 3;  buf_data.data         = older_byte;
            cfg->array_chip->queueAddElement( &buf_data );
            break;

    case 2:
            buf_data.reg = 4;  buf_data.data         = junior_byte;
            cfg->array_chip->queueAddElement( &buf_data );

            buf_data.reg = 5;  buf_data.data         = older_byte;
            cfg->array_chip->queueAddElement( &buf_data );
            break;
    }

    return 0;
}

int AyYmNote::set_volume_channel ( uint8_t chip_number, uint8_t channel, uint8_t volume ) {
    ayQueueStruct buf_data;
    buf_data.numberChip    = chip_number;
    buf_data.data           = volume;

    switch (channel) {
    case 0:
            buf_data.reg = 8;
            break;

    case 1:
            buf_data.reg = 9;
            break;

    case 2:
            buf_data.reg = 10;
            break;
    }

    cfg->array_chip->queueAddElement( &buf_data );

    return 0;
}

#endif
