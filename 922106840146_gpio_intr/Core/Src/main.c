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
#define BUZZER_GPIO_PORT    GPIOB

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
void play_siren(void);
void play_ev_alarm(void);
void delay_ms(uint32_t ms);


void beep(uint32_t frequency, uint32_t duration) {
    uint32_t period = 10000000 / frequency; // ????(??:??)
    uint32_t high_time = period / 2;       // ?????
    uint32_t low_time = period / 2;        // ?????

    for (uint32_t i = 0; i < duration * 1000 / period; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_Delay(high_time/1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_Delay(low_time/1000);
    }
}


void sound1(void) {
    uint32_t frequencies[] = {800, 1200}; // ?????????
    uint32_t duration = 200;             // ????????(??)
    uint32_t cycles = 10;                // ????

    for (uint32_t j = 0; j < cycles; j++) {
        // ?????
        beep(frequencies[0], duration);
        HAL_Delay(50); // ??????

        // ?????
        beep(frequencies[1], duration);
        HAL_Delay(50); // ??????
    }
}


void sound3(void) {
    uint32_t frequency = 1500; // ??????????
    uint32_t duration = 300;   // ??????(??)
    uint32_t cycles = 5;       // ????

    for (uint32_t j = 0; j < cycles; j++) {
        // ?????????
        beep(frequency, duration);
        HAL_Delay(100); // ??????

        // ?????????,??????????
        beep(frequency + 100, duration);
        HAL_Delay(100); // ??????
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        // BTN_LEFT:????LED
        if (btn_left_flag)
        {
					
					
					HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_R_PIN);
            btn_left_flag = 0;
            
        }

        // BTN_RIGHT:????LED
        if (btn_right_flag)
        {
            HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_B_PIN);
            btn_right_flag = 0;
        }

        // BTN_UP:????????
        if (btn_up_flag)
        {
					
						sound1();
					
            //play_siren();
            btn_up_flag = 0;
        }

        // BTN_DOWN:????????
        if (btn_down_flag)
        {
					
						sound3();
					
            //play_ev_alarm();
            btn_down_flag = 0;
        }
    }
}






// ??????
void play_siren(void)
{
    uint32_t start_time = HAL_GetTick();
    while (HAL_GetTick() - start_time < ALARM_DURATION)
    {
        // ????
        for (uint32_t i = 0; i < 100; i++)
        {
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
            delay_ms(1000000 / (2 * SIREN_FREQ_1));
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
            delay_ms(1000000 / (2 * SIREN_FREQ_1));
        }
        for (uint32_t i = 0; i < 100; i++)
        {
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
            delay_ms(1000000 / (2 * SIREN_FREQ_2));
            HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
            delay_ms(1000000 / (2 * SIREN_FREQ_2));
        }
    }
}

// ??????
void play_ev_alarm(void)
{
    uint32_t start_time = HAL_GetTick();
    while (HAL_GetTick() - start_time < ALARM_DURATION)
    {
        HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_SET);
        delay_ms(1000000 / (2 * EV_FREQ));
        HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
        delay_ms(1000000 / (2 * EV_FREQ));
    }
}

// ??????
void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for (i = 0; i < ms * 1000; i++);
}

// GPIO???


// EXTI??????
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20); // ????
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

// EXTI0??????


// EXTI1??????
//void EXTI1_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(BTN_RIGHT_PIN);
//}

// EXTI2??????
void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_UP_PIN);
}

// EXTI3??????
void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BTN_DOWN_PIN);
}

void SystemClock_Config(void)
{
    // ???????168MHz(????8MHz??)
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
