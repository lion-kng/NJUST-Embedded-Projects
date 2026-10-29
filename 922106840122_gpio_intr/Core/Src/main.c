#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

// ????
#define BTN_LEFT_PIN        GPIO_PIN_0
#define BTN_RIGHT_PIN       GPIO_PIN_4
#define BTN_UP_PIN          GPIO_PIN_5
#define BTN_DOWN_PIN        GPIO_PIN_1
#define LED_R_PIN           GPIO_PIN_12
#define LED_B_PIN           GPIO_PIN_11
#define BUZZER_PIN          GPIO_PIN_0

#define BTN_GPIO_PORT       GPIOC
#define LED_GPIO_PORT       GPIOF
#define BUZZER_PORT    GPIOB

// ?????
#define SIREN_FREQ_1        800   // ?????
#define SIREN_FREQ_2        1200  // ?????
#define EV_FREQ             1000  // ?????

// ???????
#define ALARM_DURATION      5000  // 5?

#endif /* __MAIN_H */




#include "main.h"
#include "gpio.h"

// ????
volatile uint8_t btn_left_flag = 0;
volatile uint8_t btn_right_flag = 0;
volatile uint8_t btn_up_flag = 0;
volatile uint8_t btn_down_flag = 0;

// ????
void SystemClock_Config(void);


void delay(uint32_t i){
	while(i--);
}


void sound1(void)
{
    uint32_t i =30000;
    uint32_t sound1_start = HAL_GetTick();
    while((HAL_GetTick()-sound1_start)<= 5000)
    {
        HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);
        delay(i);
        HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_SET);
        delay(i);
        i=i-6;
        if(i<=0)
            i=30000;
    }
    HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);
}
void sound2(void)
{
    uint32_t i=6000;
    uint32_t sound2_start = HAL_GetTick();
    while((HAL_GetTick()- sound2_start)<= 5000)
    {
        HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);
        delay(i);
        HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_SET);
        delay(i);
        i=i-6;
        if(i<=0)
            i=6000;
    }
    HAL_GPIO_WritePin(BUZZER_PORT,BUZZER_PIN,GPIO_PIN_RESET);
}


void AmbulanceSiren(void) {
    for (int i = 0; i < 40; i++) {

        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN,GPIO_PIN_SET);
        HAL_Delay(30);
				HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN,GPIO_PIN_RESET);
        HAL_Delay(30);

				HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN,GPIO_PIN_SET);
        HAL_Delay(20);
				HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN,GPIO_PIN_RESET);
        HAL_Delay(20);
    }
}

void ElectricSiren(void) {
    // ????????????“??”?
    for (int i = 0; i < 20; i++) { // ??5?,?????????200ms
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        HAL_Delay(100);
    }
}



int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        if (btn_left_flag)
        {
					
					
					HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_R_PIN);
            btn_left_flag = 0;
            
        }

        if (btn_right_flag)
        {
            HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_B_PIN);
            btn_right_flag = 0;
        }

        if (btn_up_flag)
        {
					
						sound1();
					
            btn_up_flag = 0;
        }

        if (btn_down_flag)
        {
					
						sound2();
            btn_down_flag = 0;
        }
    }
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    HAL_Delay(20); // ????
    if (HAL_GPIO_ReadPin(BTN_GPIO_PORT, GPIO_Pin) == GPIO_PIN_RESET)
    {
        switch (GPIO_Pin)
        {
        case BTN_LEFT_PIN:
            btn_left_flag = 1;
            break;
        case BTN_RIGHT_PIN:
            btn_right_flag = 1;
            break;
        case BTN_UP_PIN:
            btn_up_flag = 1;
            break;
        case BTN_DOWN_PIN:
            btn_down_flag = 1;
            break;
				default: break;
        }
    }
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_UP_PIN);
}


void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_DOWN_PIN);
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


