/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "nrf24l01.h"
#include <stdio.h>
#include "liquidcrystal_i2c.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define SIZE_TX_BUF 4
#define SIZE_RX_BUF 6

nrf24 nrfTx,nrfRx;
uint8_t status_tx;
uint8_t tx_data[SIZE_TX_BUF];

uint8_t tx_address[] = {0xEA, 0xDD, 0xCC, 0xBB, 0xAA};

uint16_t potvalue =0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void txinit()
{
nrfTx.CE_port = NRF_CE_GPIO_Port;
nrfTx.CE_pin = NRF_CE_Pin;
nrfTx.CSN_port = NRF_CSN_GPIO_Port;
nrfTx.CSN_pin = NRF_CSN_Pin;
nrfTx.hSPIx = &hspi1;
NRF24_Init(&nrfTx);
NRF24_Set_TxAddress(&nrfTx, tx_address);
}

/*Receiver için ayarlamalar*/
/*void rxinit()
{
nrfRx.CE_port = NRF_CE_GPIO_Port;
nrfRx.CE_pin = NRF_CE_Pin;
nrfRx.CSN_port = NRF_CSN_GPIO_Port;
nrfRx.CSN_pin = NRF_CSN_Pin;
nrfRx.hSPIx = &hspi1;
NRF24_Init(&nrfRx);
NRF24_Set_DataRate(&nrfRx, _250KBS);
NRF24_Set_PALevel(&nrfRx, HIGH);
NRF24_Set_RxPipe(&nrfRx, rx_address, 0, SIZE_RX_BUF);
NRF24_Set_Mode(&nrfRx, RX_MODE);
}*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HD44780_Init(2);
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("NRF24-e");
  /*Transmitter için Ayarlamalar*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_ADC_Start(&hadc1);
	  potvalue = HAL_ADC_GetValue(&hadc1);
	  /*uint16_t servo_value = (potvalue * 180) / 4095;*/
	  if (potvalue < 500)
	      {
	        tx_data[0] = 0;
	        tx_data[1] = 0;
	        tx_data[2] = 0;
	      }
	      else if (potvalue < 1000)
	      {
	        tx_data[0] = 1;
	        tx_data[1] = 0;
	        tx_data[2] = 0;
	      }
	      else if (potvalue < 1500)
	      {
	        tx_data[0] = 1;
	        tx_data[1] = 1;
	        tx_data[2] = 0;
	      }
	      else
	      {
	        tx_data[0] = 1;
	        tx_data[1] = 1;
	        tx_data[2] = 1;
	      }

	  tx_data[3] = HAL_GPIO_ReadPin(Boton4_GPIO_Port, Boton4_Pin);
	  txinit();
	  NRF24_Set_Mode(&nrfTx, TX_MODE);
	  status_tx = NRF24_Transmit(&nrfTx, tx_data, sizeof(tx_data));
	  if(status_tx == 1)
	  {
	      NRF24_Set_Mode(&nrfTx, STANDBY);
	      HAL_GPIO_TogglePin(LedOk_GPIO_Port, LedOk_Pin);  // LED yanıp söner

	  }
	  if(tx_data[3] == 1)
	  {

		  HD44780_SetCursor(0,1);
		  HD44780_PrintStr("GPS GELDI");


	  }
	  HAL_Delay(20);



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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar(*ptr++);
  }
  return len;
}
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
