#include "STM32F10x.h"
#include <cmsis_os.h>
#include "STM32F10x_gpio.h"
#include "STM32F10x_rcc.h"

void delay(int ms) {
    for (int i = 0; i < 6666 * ms; i++) {}
}

void led_thread1(void const *argument) // Flash LED 1
{
    for (;;) {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        delay(300);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        delay(300);
    }
}

void led_thread2(void const *argument) // Flash LED 2

{
    for (;;) {
        GPIO_SetBits(GPIOA, (GPIO_Pin_1));
        delay(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        delay(500);
    }
}

/*----------------------------------------------------------------------------
 Define the thread handles and thread parameters
 *---------------------------------------------------------------------------*/

osThreadId main_ID, led_ID1, led_ID2, led_ID3;
osThreadDef(led_thread2, osPriorityNormal,1, 0);
osThreadDef(led_thread1, osPriorityNormal,1, 0);

int main(void) {
    GPIO_InitTypeDef LAB_6_GPIO_A; // New struct for GPIO_InitStruct
    // void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
    // 1064 stm32f10x_rcc.h
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
    //352 stm32f10x_gpio.h


    GPIO_StructInit(&LAB_6_GPIO_A);
    LAB_6_GPIO_A.GPIO_Pin =
            GPIO_Pin_0 |
            GPIO_Pin_1;
    LAB_6_GPIO_A.GPIO_Mode = GPIO_Mode_Out_PP;
    LAB_6_GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LAB_6_GPIO_A);
    osKernelInitialize();                    // initialize CMSIS-RTOS
    led_ID1 = osThreadCreate(osThread(led_thread1), NULL);
    led_ID2 = osThreadCreate(osThread(led_thread2), NULL);
    osKernelStart();                         // start thread execution
    while (1) { ; }
}
