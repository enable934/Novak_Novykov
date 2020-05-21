#include "STM32F10x.h"
#include <cmsis_os.h>
#include "STM32F10x_gpio.h"
#include "STM32F10x_rcc.h"
#include "STM32F10x_exti.h"

osThreadId led_ID1;
osThreadId button_ID1;
int counter = 340;

void led_thread1(void const *argument) // Flash LED A01
{
    for (int j = 0;;j++) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        osDelay(counter);
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        osDelay(counter);
    }
}

void EXTI0_IRQHandler(void) {
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Do your stuff when PB0 is changed */
        if (counter > 1000)
            counter = 100;
        else
            counter = counter + 100;

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

osThreadId led_ID1, led_ID2, button_ID1;
osThreadDef(led_thread1, osPriorityNormal,1, 0);

int main(void) {
    GPIO_InitTypeDef LAB_6_GPIO_A; // New struct for GPIO_InitStruct
    GPIO_InitTypeDef LAB_6_GPIO_BUTTON; // New struct for GPIO_InitStruct

    // void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);


    GPIO_StructInit(&LAB_6_GPIO_A);
    GPIO_StructInit(&LAB_6_GPIO_BUTTON);
    LAB_6_GPIO_A.GPIO_Pin = GPIO_Pin_1;
    LAB_6_GPIO_BUTTON.GPIO_Pin =
            GPIO_Pin_0;
    LAB_6_GPIO_A.GPIO_Mode = GPIO_Mode_Out_PP;
    LAB_6_GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LAB_6_GPIO_A);
    LAB_6_GPIO_BUTTON.GPIO_Mode =  GPIO_Mode_IPU;
    LAB_6_GPIO_BUTTON.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &LAB_6_GPIO_BUTTON);
    osKernelInitialize();                    // initialize CMSIS-RTOS
    led_ID1 = osThreadCreate(osThread(led_thread1), NULL);


    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    /* Connect EXTI Line0 to PB0 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

    /* PD0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
    osKernelStart(); // start thread execution
    while (1) { ; }
}
