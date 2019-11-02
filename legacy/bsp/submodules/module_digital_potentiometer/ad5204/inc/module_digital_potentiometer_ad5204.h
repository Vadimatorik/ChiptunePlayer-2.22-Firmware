/*

@startuml

DigitalPotentiometer.AD5204		--o			mc_interfaces.Pin
DigitalPotentiometer.AD5204		--o			mc_interfaces.SpiMaster8Bit




namespace DigitalPotentiometer {

class AD5204 {
	{field}-	const Ad5204StaticCfg*			const cfg
	__Constructor__
	{method}+	AD5204 ( const Ad5204StaticCfg*		const cfg )
	__Public methods__
	{method}+	void				connectOn			( void )
	{method}+	void				connectOff			( void )
	{method}+	EC_AD5204_ANSWER	setValue			( uint8_t	chipNumber,\n\t\t\t\t\t\t\t\t  uint8_t	reg,\n\t\t\t\t\t\t\t\t  uint8_t	value )
	{method}+	EC_AD5204_ANSWER	readValue			( int8_t	chipNumber,\n\t\t\t\t\t\t\t\t  uint8_t& 	returnResultData )
}

}




@enduml

*/

#pragma once

#include "project_config.h"

#ifdef MODULE_DIGITAL_POTENTIOMETER_ENABLED

#include "module_digital_potentiometer_ad5204_struct.h"

namespace DigitalPotentiometer {

enum class EC_AD5204_ANSWER {
    OK = 0,
    ARG_ERROR = 1,
    SPI_ERROR = 2
};

class AD5204 {
public:
    AD5204 (const Ad5204StaticCfg *const cfg);

public:
    void shdnSet (void);
    
    void shdnReset (void);
    
    EC_AD5204_ANSWER setValue (uint8_t chipNumber,
                               uint8_t reg,
                               uint8_t value);
    
    EC_AD5204_ANSWER readValue (uint8_t chipNumber,
                                uint8_t reg,
                                uint8_t &returnResultData);

private:
    const Ad5204StaticCfg *const cfg;
};

}

#endif
