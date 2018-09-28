#include <stdint.h>

#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

__attribute__ ((section (".rodata_keep")))
static const uint8_t freeRTOSMemoryScheme = configUSE_HEAP_SCHEME;

void vApplicationMallocFailedHook( void ) {
	while(1);
}

void vApplicationStackOverflowHook ( void ) {
	while(1);
}

void prvGetRegistersFromStack ( uint32_t *pulFaultStackAddress ) {
	__attribute__((unused)) volatile uint32_t r0;
	__attribute__((unused)) volatile uint32_t r1;
	__attribute__((unused)) volatile uint32_t r2;
	__attribute__((unused)) volatile uint32_t r3;
	__attribute__((unused)) volatile uint32_t r12;
	__attribute__((unused)) volatile uint32_t lr; 		/// Link register.
	__attribute__((unused)) volatile uint32_t pc;		/// Program counter.
	__attribute__((unused)) volatile uint32_t psr;		/// Program status register.

	r0	=	pulFaultStackAddress[ 0 ];
	r1	=	pulFaultStackAddress[ 1 ];
	r2	=	pulFaultStackAddress[ 2 ];
	r3	=	pulFaultStackAddress[ 3 ];

	r12	=	pulFaultStackAddress[ 4 ];
	lr	=	pulFaultStackAddress[ 5 ];
	pc	=	pulFaultStackAddress[ 6 ];
	psr	=	pulFaultStackAddress[ 7 ];

	for( ;; );
}

