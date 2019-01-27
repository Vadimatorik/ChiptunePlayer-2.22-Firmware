#include "stm32f4xx_hal.h"
#include "ayplayer_mc_hardware.h"
#include "ayplayer_pcb_hardware.h"
#include "FreeRTOS.h"

extern "C" {

extern void xPortSysTickHandler (void);

void HardFault_Handler (void) {
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

void PVD_IRQHandler (void) {
    pwr.irqHandler();
    NVIC_SystemReset();
}

void SysTick_Handler (void) {
    HAL_IncTick();
    xPortSysTickHandler();
}

void TIM6_DAC_IRQHandler (void) {
    interruptAy.clearInterruptFlag();
    ayLow.timerInterruptHandler();
}

void DMA2_Stream5_IRQHandler (void) {
    lcdSpi.irqHandler();
}

void DMA1_Stream4_IRQHandler (void) {
    boardSpi.irqHandler();
}

void DMA2_Stream3_IRQHandler (void) {
    sd.dmaRxHandler();
}

void USART1_IRQHandler (void) {
    usart.irqHandler();
}

void DMA2_Stream7_IRQHandler (void) {
    usart.irqHandler();
}

void CRYP_IRQHandler (void) {
    NVIC_SystemReset();
}

void FPU_IRQHandler (void) {
    NVIC_SystemReset();
}

void __start (void);
void Reset_Handler (void) {
    __start();
}

void assertFailed (uint8_t *file, uint32_t line) {
    (void)file;
    (void)line;
    NVIC_SystemReset();
}

}