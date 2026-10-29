
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"
#include "stdio.h"
#include "stm32f4xx_hal.h"
#include "LCD/drv_lcd.h"
#include "LCD/rttlogo.h"


uint8_t hours = 8, minutes = 10, seconds = 0;
uint8_t adjust_mode = 0; // 0:???,1:???,2:???,3:???

#define LEFT_KEY_PIN GPIO_PIN_0
#define LEFT_KEY_GPIO_PORT GPIOC
#define DOWN_KEY_PIN GPIO_PIN_1
#define DOWN_KEY_GPIO_PORT GPIOC
#define RIGHT_KEY_PIN GPIO_PIN_4
#define RIGHT_KEY_GPIO_PORT GPIOC

#define BTN_LEFT_PIN        GPIO_PIN_0
#define BTN_RIGHT_PIN       GPIO_PIN_4
#define BTN_UP_PIN          GPIO_PIN_5
#define BTN_DOWN_PIN        GPIO_PIN_1
#define BTN_GPIO_PORT       GPIOC

#define LED_R_PIN           GPIO_PIN_12
#define LED_GPIO_PORT       GPIOF

void UpdateTime(void)
{
    seconds++;
		if (seconds >= 60)
		{
			seconds = 0;
			minutes++;
			if (minutes >= 60)
			{
        minutes = 0;
        hours++;
        if (hours >= 24)
        {
            hours = 0;
        }
			}
		}
	


}

void DisplayTimeOnLCD(void)
{
    char time_str[9]; // ??????????????,??"08:10:00"
    
    // ????????
    sprintf(time_str, "%02d:%02d:%02d", hours, minutes, seconds);
    // ?LCD????????
    lcd_show_string(10, 69, 32, time_str);
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {		
        // ?????????
        UpdateTime();
        // ??????????LCD?
        DisplayTimeOnLCD();
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
					hours = (hours + 1) % 24;
				//HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_R_PIN);
            break;
        case BTN_RIGHT_PIN:
            seconds = (seconds + 1) % 60;
            break;
        case BTN_DOWN_PIN:
            minutes = (minutes + 1) % 60;
            break;
				default: break;
        }
    }
}


// void KeyScan(void)
// {
//     if (HAL_GPIO_ReadPin(LEFT_KEY_GPIO_PORT, LEFT_KEY_PIN) == GPIO_PIN_SET)
//     {
//         HAL_Delay(1); // ??
//         if (HAL_GPIO_ReadPin(LEFT_KEY_GPIO_PORT, LEFT_KEY_PIN) == GPIO_PIN_SET)
//         {
// 					hours = (hours + 1) % 24;
//         }
//     }
//     // ??????????
//     // ...
// 		if (HAL_GPIO_ReadPin(DOWN_KEY_GPIO_PORT, DOWN_KEY_PIN) == GPIO_PIN_SET)
//     {
//         HAL_Delay(1); // ??
//         if (HAL_GPIO_ReadPin(DOWN_KEY_GPIO_PORT, DOWN_KEY_PIN) == GPIO_PIN_SET)
//         {
// 					minutes = (minutes + 1) % 60;
//         }
//     }
// 		if (HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_PORT, RIGHT_KEY_PIN) == GPIO_PIN_SET)
//     {
//         HAL_Delay(1); // ??
//         if (HAL_GPIO_ReadPin(RIGHT_KEY_GPIO_PORT, RIGHT_KEY_PIN) == GPIO_PIN_SET)
//         {
// 					seconds = (seconds + 1) % 60;
//         }
//     }
// }





void SystemClock_Config(void);
// fprintf()???
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
	return ch;
}
int main(void)
{

  HAL_Init();
	MX_GPIO_Init();
	
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
  SystemClock_Config();
  
  MX_FSMC_Init();
  MX_TIM14_Init();
	MX_TIM6_Init();
  //MX_USART1_UART_Init();
  HAL_TIM_Base_Start_IT(&htim14);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim6); // ???????
	drv_lcd_init();		// ???LCD

		lcd_clear(WHITE);
		lcd_set_color(WHITE, BLACK);
		//lcd_show_string(10,69+16+24+10,32, "Contact :22102600@qq.com");
		//lcd_draw_line(0, 69 + 16 + 24 + 32, 240, 69 + 16 + 24 + 32);
	
	
	
	  char time_str[9]; // ??????????????,??"08:10:00"
    
    // ????????
  sprintf(time_str, "%02d:%02d:%02d", hours, minutes, seconds);
	
	lcd_show_string(10, 69, 32, time_str);
  while (1)
  {
		
		
		//KeyScan(); // ????
		
		
		
  }
  /* USER CODE END 3 */
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

