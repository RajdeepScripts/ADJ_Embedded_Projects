/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  */
/* USER CODE END Header */

#include "main.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rxByte;             // 1 byte from interrupt
uint8_t rxData[50];         // buffer
uint8_t rxIndex = 0;        // index
/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  // Start UART interrupt RX
  HAL_UART_Receive_IT(&huart1, &rxByte, 1);

  char msg[] = "HELLO_FROM_MASTER_STM32\n";

  while (1)
  {
      HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 100);
      HAL_Delay(1000);
  }
}

/* -------------------------------------------------------------------------- */
/* UART RX Complete Callback                                                  */
/* -------------------------------------------------------------------------- */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (rxByte == '\n')
        {
            rxData[rxIndex] = '\0';

            // Echo back the received message
            HAL_UART_Transmit(&huart1, rxData, strlen((char*)rxData), 100);

            rxIndex = 0;  // reset buffer
        }
        else
        {
            rxData[rxIndex++] = rxByte;

            if (rxIndex >= sizeof(rxData))
                rxIndex = 0; // prevent overflow
        }

        HAL_UART_Receive_IT(&huart1, &rxByte, 1);  // restart RX
    }
}

/* -------------------------------------------------------------------------- */
/* Clock Configuration                                                        */
/* -------------------------------------------------------------------------- */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/* -------------------------------------------------------------------------- */
/* UART1 Init (PA9 TX, PA10 RX)                                              */
/* -------------------------------------------------------------------------- */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* -------------------------------------------------------------------------- */
/* GPIO Init (Port A enabled)                                                */
/* -------------------------------------------------------------------------- */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* -------------------------------------------------------------------------- */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
