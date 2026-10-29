#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"
#include "stdio.h"
#include "./BSP/LCD/drv_lcd.h"     // LCD ?????
#include "./BSP/LCD/rttlogo.h"     // ??? logo ?????

// ?? PWM ???????????(?? volatile ????????????????)
__IO float IC2ValuePWM = 0;
__IO float DutyCyclePWM = 0;
__IO float FrequencyPWM = 0;

// ????PWM???????(????4000)
float duty = 4000;

void SystemClock_Config(void);
// ??? printf ???1(??????)
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
	return ch;
}

// ?????????????(TIM9 ??PWM??)
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM9 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		// ?? PWM ???
		IC2ValuePWM = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		if(IC2ValuePWM != 0)
		{
			// ??? = ????? / ?? * 100%
			DutyCyclePWM = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2)) * 100) / IC2ValuePWM;
			// ?? = ????? / ???
			FrequencyPWM = (HAL_RCC_GetPCLK2Freq()*2/(TIM9->PSC+1)) / IC2ValuePWM;
		}
		else
		{
			DutyCyclePWM = 0;
			FrequencyPWM = 0;
		}
	}
}

// ????????(???????)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	HAL_Delay(10); // ????(10ms)

	// ???????????(???)
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_Pin) == GPIO_PIN_RESET) {
		if (GPIO_Pin == LEFT_Pin) {
			// ??:?????(??400)
			if (duty > 400) {
				duty -= 400;
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, duty); // ??PWM???
			}
		}
		if (GPIO_Pin == RIGHT_Pin) {
			// ??:?????(??7600)
			if (duty < 7600) {
				duty += 400;
				__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, duty); // ??PWM???
			}
		}
	}
}

int main(void)
{
	// ??? HAL ?
	HAL_Init();

	// ???????
	SystemClock_Config();

	// ?????
	MX_GPIO_Init();
	MX_FSMC_Init();
	MX_TIM14_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	MX_TIM9_Init();

	// ?? TIM14 ? TIM2 ??????PWM??
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	// ?? TIM9 ??1?2???????(?????????)
	HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim9, TIM_CHANNEL_2);

	// ??? LCD ??
	drv_lcd_init();

	// ?????
	char DutyCycle[20];
	char Frequency[20];

	// ??,????????
	lcd_clear(WHITE);
	lcd_set_color(WHITE, BLACK);

	while (1)
	{
		// ??????????????? LCD
		sprintf(DutyCycle, "DutyCycle: %.1f", DutyCyclePWM);
		sprintf(Frequency, "Frequency: %.1f", FrequencyPWM);
		lcd_show_string(10, 40, 24, DutyCycle);
		lcd_show_string(10, 90, 24, Frequency);
		HAL_Delay(200);  // ?????200ms
	}
}

// ????????(????????HSE,??PLL???168MHz)
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		Error_Handler();

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
		Error_Handler();
}

// ??????(????????)
void Error_Handler(void)
{
	__disable_irq();
	while (1) {}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	// ??? assert ????
}
#endif
