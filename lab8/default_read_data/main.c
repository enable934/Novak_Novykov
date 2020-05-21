#include "STM32F10x.h"
#include <cmsis_os.h>
#include "STM32F10x_gpio.h"
#include "STM32F10x_rcc.h"

osThreadId led_ID1;
osThreadId led_ID2;
osThreadId button_ID1;
int counter = 100;
char novykov[] = {'-', ' ', '*', ' ', '-', '-', '-', ' ', '*', '*', '*', '-', ' ', '-', '*', '-', '-', ' ', '-',
                  '*', '-', ' ', '-', '-', '-', ' ', '*', '*', '*', '-'};

char novak[] = {'-', ' ', '*', ' ', '-', '-', '-', ' ', '*', '*', '*', '-', ' ', '*', '-', '*', '-'};

int length1 = sizeof(novykov) / sizeof(char);
int length2 = sizeof(novak) / sizeof(char);

int buttonPressed(void)
{
    if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_0))
        return 1;
    else
        return 0;
}

/*----------------------------------------------------------------------------
 Define the thread handles and thread parameters
 *---------------------------------------------------------------------------*/
void led_thread1(void const *argument) // Flash LED 1
{
    for (int j = 0;;j++) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);

        char element = novykov[j % length1];

        if (element == '*') {
            GPIO_SetBits(GPIOA, GPIO_Pin_0);
            osDelay(counter);
        } else if (element == '-') {
            GPIO_SetBits(GPIOA, GPIO_Pin_0);
            osDelay(counter * 2);
        } else if (element == ' ') {
            GPIO_ResetBits(GPIOA, GPIO_Pin_0);
            osDelay(counter * 4);
        }
        if (j % length1 == length1 - 1) {
            osDelay(5000);
        };
    }
}

void led_thread2(void const *argument) // Flash LED 2
{
    for (int j = 0;;j++) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);

        char element = novak[j % length1];

        if (element == '*') {
            GPIO_SetBits(GPIOA, GPIO_Pin_1);
            osDelay(counter);
        } else if (element == '-') {
            GPIO_SetBits(GPIOA, GPIO_Pin_1);
            osDelay(counter * 2);
        } else if (element == ' ') {
            GPIO_ResetBits(GPIOA, GPIO_Pin_1);
            osDelay(counter * 4);
        }
        if (j % length2 == length2 - 1) {
            osDelay(5000);
        };
    }
}

void button_thread1(void const *argument) // button handler
{
    for (;;) {
        if(buttonPressed())
        {
            if (counter > 1000)
                counter = 100;
            else
                counter = counter + 100;
        }
    }
}

osThreadId main_ID, led_ID1, led_ID2, button_ID1;
osThreadDef(led_thread2, osPriorityNormal,1, 0);
osThreadDef(led_thread1, osPriorityNormal,1, 0);
osThreadDef(button_thread1, osPriorityNormal,1, 0);

int main(void) {
    GPIO_InitTypeDef LAB_6_GPIO_A; // New struct for GPIO_InitStruct
    GPIO_InitTypeDef LAB_6_GPIO_BUTTON; // New struct for GPIO_InitStruct

    // void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);


    GPIO_StructInit(&LAB_6_GPIO_A);
    GPIO_StructInit(&LAB_6_GPIO_BUTTON);
    LAB_6_GPIO_A.GPIO_Pin =
            GPIO_Pin_0 |
            GPIO_Pin_1;
    LAB_6_GPIO_BUTTON.GPIO_Pin =
            GPIO_Pin_0;
    LAB_6_GPIO_A.GPIO_Mode = GPIO_Mode_Out_PP;
    LAB_6_GPIO_A.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LAB_6_GPIO_A);
    LAB_6_GPIO_BUTTON.GPIO_Mode =  GPIO_Mode_IPU;
    LAB_6_GPIO_BUTTON.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &LAB_6_GPIO_BUTTON);
    osKernelInitialize();                    // initialize CMSIS-RTOS
    led_ID2 = osThreadCreate(osThread(led_thread2), NULL);
    led_ID1 = osThreadCreate(osThread(led_thread1), NULL);
    button_ID1 = osThreadCreate(osThread(button_thread1), NULL);
    osKernelStart(); // start thread execution

    while (1) { ; }
}
