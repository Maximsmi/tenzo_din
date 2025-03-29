/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void sendData(char *data);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void handleHiCommand(void);
void handleSetCommand(void);
void MX_I2C1_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NRF24_IRQ_Pin GPIO_PIN_0
#define NRF24_IRQ_GPIO_Port GPIOF
#define NRF24_CE_Pin GPIO_PIN_1
#define NRF24_CE_GPIO_Port GPIOF
#define ADC_POWER_Pin GPIO_PIN_0
#define ADC_POWER_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_1
#define RS485_DE_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_2
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_3
#define RS485_RX_GPIO_Port GPIOA
#define NRF24_NSS_Pin GPIO_PIN_4
#define NRF24_NSS_GPIO_Port GPIOA
#define NRF24_SCK_Pin GPIO_PIN_5
#define NRF24_SCK_GPIO_Port GPIOA
#define NRF24_MISO_Pin GPIO_PIN_6
#define NRF24_MISO_GPIO_Port GPIOA
#define NRF24_MOSI_Pin GPIO_PIN_7
#define NRF24_MOSI_GPIO_Port GPIOA
#define Led_Pin GPIO_PIN_1
#define Led_GPIO_Port GPIOB
#define ZSC31050_SCL_Pin GPIO_PIN_9
#define ZSC31050_SCL_GPIO_Port GPIOA
#define ZSC31050_SDA_Pin GPIO_PIN_10
#define ZSC31050_SDA_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
