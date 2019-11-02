#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"

void __start ();
extern void xPortSysTickHandler ();

void HardFault_Handler () {
    __asm volatile
    (
    " tst lr, #4												\n"
    " ite eq													\n"
    " mrseq r0, msp												\n"
    " mrsne r0, psp												\n"
    " ldr r1, [r0, #24]											\n"
    " ldr r2, handler2_address_const							\n"
    " bx r2														\n"
    " handler2_address_const: .word prvGetRegistersFromStack	\n"
    );
}

void PVD_IRQHandler () {
    NVIC_SystemReset();
}

void SysTick_Handler () {
    HAL_IncTick();
    xPortSysTickHandler();
}

void CRYP_IRQHandler () {
    NVIC_SystemReset();
}

void FPU_IRQHandler () {
    NVIC_SystemReset();
}

void Reset_Handler () {
    __start();
}
