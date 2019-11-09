#include "mono_lcd_lib_gdeh0154d27.h"

#ifdef MODULE_MONO_LCD_GDEH0154D27_ENABLED

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace MonoLcd {

GDEH0154D27::GDEH0154D27 ( const GDEH0154D27Cfg* const cfg, uint8_t* const buf ) :
    cfg( cfg ), userBuf( buf ) {
    this->m     = xSemaphoreCreateMutexStatic( &this->mb );
}

/*!
 *	Первый байт отправляется как команда, а последующие как данные.
 */
mc_interfaces::res GDEH0154D27::write ( const uint8_t* const data, uint8_t dataSize ) {
    cfg->cs->reset();

    /// Готовимся передавать команду.
    cfg->dc->reset();

    /// Передаем команду (1 байт).
    mc_interfaces::res r = this->cfg->s->tx( data, 1, 10 );
    if ( r != mc_interfaces::res::err_ok ) {
        return r;
    }

    /// Готовимся передавать данные для команды.
    cfg->dc->set();

    /// Передаем данные.
    r = this->cfg->s->tx( &data[ 1 ], dataSize - 1, 10 );

    cfg->cs->set();

    return r;
}

mc_interfaces::res GDEH0154D27::writeCmd( uint8_t command ) {
    cfg->cs->reset();
    cfg->dc->reset();

    mc_interfaces::res r = this->cfg->s->tx( &command, 1, 10 );
        if ( r != mc_interfaces::res::err_ok ) {
            return r;
        }

    cfg->cs->set();
    return r;
}

void GDEH0154D27::ReadBusy ( void ) {
    while( this->cfg->busy->read() ) {
        vTaskDelay( 100 );
    }
}

mc_interfaces::res GDEH0154D27::writeCmdP1( uint8_t command, uint8_t arg ) {
    //while(isEPD_W21_BUSY == 1);	// wait
    this->ReadBusy();

    cfg->cs->reset();
    cfg->dc->reset();

    mc_interfaces::res r = this->cfg->s->tx( &command, 1, 10 );
    if ( r != mc_interfaces::res::err_ok ) {
        return r;
    }

    cfg->dc->set();

    r = this->cfg->s->tx( &arg, 1, 10 );

    cfg->cs->set();
    return r;
}

mc_interfaces::res GDEH0154D27::setRamArea( uint8_t Xstart,uint8_t Xend,uint8_t Ystart,uint8_t Ystart1,uint8_t Yend,uint8_t Yend1){
    mc_interfaces::res r = mc_interfaces::res::err_ok;

    uint8_t RamAreaX[3];	// X start and end
    uint8_t RamAreaY[5]; 	// Y start and end
    RamAreaX[0] = 0x44;	// command
    RamAreaX[1] = Xstart;
    RamAreaX[2] = Xend;
    RamAreaY[0] = 0x45;	// command
    RamAreaY[1] = Ystart;
    RamAreaY[2] = Ystart1;
    RamAreaY[3] = Yend;
    RamAreaY[4] = Yend1;

    do {
        r = this->write(RamAreaX, sizeof(RamAreaX));
        checkResultAndBreak( r );
        r = this->write(RamAreaY, sizeof(RamAreaY));
    } while ( false );

    return r;
}

mc_interfaces::res GDEH0154D27::setRamPointer(uint8_t addrX,uint8_t addrY,uint8_t addrY1){
    mc_interfaces::res r = mc_interfaces::res::err_ok;

    uint8_t RamPointerX[2];	// default (0,0)
    uint8_t RamPointerY[3];
    RamPointerX[0] = 0x4e;
    RamPointerX[1] = addrX;
    RamPointerY[0] = 0x4f;
    RamPointerY[1] = addrY;
    RamPointerY[2] = addrY1;

    do {
        r = this->write(RamPointerX, sizeof(RamPointerX));
        checkResultAndBreak( r );
        r = this->write(RamPointerY, sizeof(RamPointerY));
    } while ( false );

    return r;
}


mc_interfaces::res GDEH0154D27::reset ( void ) {
    xSemaphoreTake( this->m, portMAX_DELAY );

    mc_interfaces::res r = mc_interfaces::res::err_ok;

    do {
        cfg->cs->set();
        cfg->res->reset();
        vTaskDelay(100);
        cfg->res->set();
        vTaskDelay(100);

        r = this->write(this->GDOControl, sizeof(this->GDOControl));	// Pannel configuration, Gate selection
        checkResultAndBreak( r );
        r = this->write(this->softstart, sizeof(this->softstart));	// X decrease, Y decrease
        checkResultAndBreak( r );
        r = this->write(this->VCOMVol, sizeof(this->VCOMVol));		// VCOM setting
        checkResultAndBreak( r );
        r = this->write(this->DummyLine, sizeof(this->DummyLine));	// dummy line per gate
        checkResultAndBreak( r );
        r = this->write(this->Gatetime, sizeof(this->Gatetime));		// Gage time setting
        checkResultAndBreak( r );
        r = this->write(this->RamDataEntryMode, sizeof(this->RamDataEntryMode));	// X increase, Y decrease
        checkResultAndBreak( r );
        r =  this->setRamArea(0x00,(xDot-1)/8,(yDot-1)%256,(yDot-1)/256,0x00,0x00);	// X-source area,Y-gage area
        checkResultAndBreak( r );
        r =	this->setRamPointer(0x00,(yDot-1)%256,(yDot-1)/256);	// set ram
        checkResultAndBreak( r );
        r = this->write(this->LUTDefault_full, sizeof(this->LUTDefault_full));
    } while ( false );

    xSemaphoreGive( this->m );

    return r;
}


mc_interfaces::res 	GDEH0154D27::fullUpdate(void){
    mc_interfaces::res r;

    do {
        r = this->writeCmdP1(0x22,0xc4);
        checkResultAndBreak( r );
        r = this->writeCmd(0x20);
        checkResultAndBreak( r );
        this->writeCmd(0xff);
    } while ( false );

    return r;
}

mc_interfaces::res 	GDEH0154D27::sendBuffer ( void ) {
    mc_interfaces::res r;

    do {
        r = this->setRamPointer( 0x00,(yDot-1)%256,(yDot-1)/256 );
        checkResultAndBreak( r );
        r = this->writeDispRam( xDot, yDot, this->userBuf );
        checkResultAndBreak( r );
        r = this->fullUpdate();
    } while ( false );

    return r;
}



mc_interfaces::res 	GDEH0154D27::writeDispRam( uint8_t XSize, uint8_t  YSize,		uint8_t  *Dispbuff) {

        if(XSize%8 != 0)
        {
            XSize = XSize+(8-XSize%8);
        }
        XSize = XSize/8;


        this->ReadBusy();



        cfg->cs->reset();
            cfg->dc->reset();	//command write

        uint8_t bufCmd = 0x24;

        mc_interfaces::res r = this->cfg->s->tx( &bufCmd, 1, 10 );
            if ( r != mc_interfaces::res::err_ok ) {
                return r;
            }

            cfg->dc->set();	//data write

         r = this->cfg->s->tx( Dispbuff, YSize * XSize, 100 );
                if ( r != mc_interfaces::res::err_ok ) {
                    return r;
                }



                cfg->cs->set();

                return r;
}



mc_interfaces::res GDEH0154D27::on ( void ) {
    xSemaphoreTake( this->m, portMAX_DELAY );

    mc_interfaces::res r;

    do {
        r = writeCmdP1( 0x22,0xc0 );
        checkResultAndBreak( r );
        r = writeCmd( 0x20 );
    } while ( false );

    xSemaphoreGive( this->m );

    return r;
}

mc_interfaces::res GDEH0154D27::off ( void ) {
    xSemaphoreTake( this->m, portMAX_DELAY );

    mc_interfaces::res r;
    r = mc_interfaces::res::errNotData;
    //r = this->comOut(CMD_DISPLAY_OFF);

    xSemaphoreGive( this->m );

    return r;
}

mc_interfaces::res GDEH0154D27::update ( void ) {
    mc_interfaces::res r;

    xSemaphoreTake( this->m, portMAX_DELAY );

    r = this->sendBuffer();

    xSemaphoreGive( this->m );

    return r;
}

mc_interfaces::res GDEH0154D27::lcdClear ( void ) {
    xSemaphoreTake( this->m, portMAX_DELAY );

    mc_interfaces::res r;

    memset( this->userBuf, 0, 5000 );
    r = this->sendBuffer();

     xSemaphoreGive( this->m );

    return r;
}

void GDEH0154D27::bufClear ( void ) {
    xSemaphoreTake( this->m, portMAX_DELAY );

    memset( this->userBuf, 0, 5000 );

    xSemaphoreGive( this->m );
}

}

#endif
