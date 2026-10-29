
#include "main.h"
#include "usart.h"
#include "gpio.h"




void SystemClock_Config(void);

void delay_us(uint32_t nus){
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD+1;
    ticks=nus*(SystemCoreClock/1000000);
    told=SysTick->VAL;
    while(1){
        tnow=SysTick->VAL;
        if(tnow!=told){
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;
        }
    }
}


void USART1_SendData(uint8_t data) {
    while (!(USART1->SR & USART_SR_TXE));
	
    USART1->DR = data;
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendData(*str++);
    }
}
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


	uint16_t i,KeyVal=0;
	uint8_t PreLine[3]={2,0,1};
	char txBuffer[16]={'K','e','y','V','a','l',':',' '};
	
	
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
	
	
  while (1)
  {
    /* USER CODE END WHILE */

		for(i=0;i<3;i++){
			if((GPIOE->IDR&0X0F)!=0X0F){
				//delay
				delay_us(16000);
				if((GPIOE->IDR&0X0F)!=0X0F){
					switch(GPIOE->IDR&0X0F){
						case 0x0e:KeyVal=4*PreLine[i]+1;break;
						case 0x0d:KeyVal=4*PreLine[i]+1;break;
						case 0x0b:KeyVal=4*PreLine[i]+1;break;
						case 0x07:KeyVal=4*PreLine[i]+1;break;
						default:break;
					}
					txBuffer[8]=KeyVal;
					USART1_SendString(txBuffer);
				}
			}
			GPIOE->ODR=~(1<<(i+4));
		}
		//GPIOF->ODR=~((KeyVal /10<<4)+KeyVal %10);
		
		
		
    /* USER CODE BEGIN 3 */
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
