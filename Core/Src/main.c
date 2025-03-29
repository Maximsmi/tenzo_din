/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "adc.h"
#include "dma.h"
#include "i2c.h"
//#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "zsc31050.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEVICE_NUMBER_ADDR 0x20 // Адрес в EEPROM для хранения номера устройства

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char TxData[70];
uint8_t RxData[16];
int indx = 0;
volatile uint8_t LcdCommand = 0;
volatile uint8_t SetCommand = 0;
volatile uint8_t flag_zsc = 0;
volatile uint8_t flag_nrf = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void sendData(char *data);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void handleLcdCommand(void);
void handleSetCommand(void);
void handleADCCommand(void);
void handleTemperatureCommand(void);
int readADCVoltage(void);
int readInternalTemperature(void);
void handleZSCCommand(char *command);
void handleNRFCommand(char *command);
uint16_t zsc31050_read_register(uint8_t reg, uint8_t bytes);
void zsc31050_write_register(uint8_t reg, uint8_t bytes, uint16_t data);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void sendData(char *data) {
	// Проверяем, что данные не пустые
	if (data == NULL || strlen(data) == 0) {
		return;
	}

	// Устанавливаем GPIO в высокое состояние перед передачей
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

	// Отправляем данные через UART с использованием DMA
	if (HAL_UART_Transmit_DMA(&huart1, (uint8_t*) data, strlen(data)) != HAL_OK) {
		// Если передача не удалась, можно добавить обработку ошибки
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // Сбрасываем GPIO
	}
	// Ожидание завершения передачи
	while (!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)) {
	    // Можно добавить таймаут
	}
	// Дополнительная проверка DMA (опционально)
	//if (HAL_DMA_GetState(huart1.hdmatx) == HAL_DMA_STATE_READY) {
	    // Всё завершено
	//}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {

		// Код после завершения передачи
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		// Очистка DMA буфера
		memset(TxData, 0, sizeof(TxData));
	}
}

// Модифицируем функцию HAL_UARTEx_RxEventCallback для обработки новых команд
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        if (strncmp((char*)RxData, "lcd", 2) == 0) {
            LcdCommand = 1;
        } else if (strncmp((char*)RxData, "set", 3) == 0) {
            SetCommand = 1;
        } else if (strncmp((char*)RxData, "zsc", 3) == 0) {
            flag_zsc = 1;
        } else if (strncmp((char*)RxData, "nrf", 3) == 0) {
            flag_nrf = 1;
        } else if (strncmp((char*)RxData, "exit", 4) == 0) {
            // Выход из режима настроек
            sprintf(TxData, "Exiting settings mode.\r\n");
            sendData(TxData);
            return;
        } else {
            sprintf(TxData, "Unknown command: %s\r\n", (char*)RxData);
            sendData(TxData);
            memset(RxData, 0, sizeof(RxData));
            // Перезапускаем прием данных
            HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 16);
            //HAL_Delay(10);
        }
    }
}

void handleLcdCommand(void) {
	// Действия при получении команды "Lcd"
	sprintf(TxData, "Hello! You sent 'Lcd'.\r\n");
	sendData(TxData);

    // Отправить сообщение "ok"
	sprintf(TxData, "<ok>\r\n");
	sendData(TxData);

    // Прочитать номер устройства из EEPROM
    //uint16_t deviceNumber = read_i2c(DEVICE_NUMBER_ADDR);

    // Отправить сообщение вида "> Nxxx"
    //char message[20];
    //snprintf(TxData, sizeof(TxData), "> N%03d\r\n", deviceNumber);
    //sendData(TxData);

    // Перевод микросхемы в режим NOM
    //uint8_t cmd = ZSC31050_CMD_START_NOM;
    //I2C_Write(ZSC31050_ADDR, &cmd, 1);

    // Чтение данных из регистра "Output Hardware & ROM version to SIF Out Register #1"
    //uint8_t data[2];
    //ZSC31050_ReadRegister(ROM_VERSION, data, 2);

    // Отправить данные через sendData
    //snprintf(message, sizeof(message), "ROM Version: %02X%02X\r\n", data[0], data[1]);
    //sendData(message);


    while (1) {
        // Считать данные с микросхемы ZSC31050
        uint16_t data = 0; // Здесь нужно реализовать чтение данных с микросхемы
        uint8_t checksum = (data / 1000) + (data / 100 % 10) + (data / 10 % 10) + (data % 10);

        // Отправить данные в формате "xxxx,yy"
        snprintf(TxData, sizeof(TxData), "%04d,%02d\r\n", data, checksum);
        sendData(TxData);


        // Ожидать ответа "ok" или "err"
        /*HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, (uint8_t*)message, sizeof(message), HAL_MAX_DELAY);
        if (status == HAL_OK) {
            if (strcmp(TxData, "err") == 0) {
                // Повторить посылку
                sendData(TxData);
            }
            // Если пришла команда "ok", ничего не делаем
        }*/

        // Задержка 2 секунды
        HAL_Delay(2000);
    }
}

void handleSetCommand(void) {
    sprintf(TxData, "Entered settings mode. Send commands with 'zsc' or 'nrf' prefix.\r\n");
    sendData(TxData);

    while (1) {
        // Проверяем флаги в основном цикле настроек
        if (flag_zsc) {
            handleZSCCommand((char*)RxData);
            flag_zsc = 0;
            memset(RxData, 0, sizeof(RxData));
        }

        if (flag_nrf) {
            handleNRFCommand((char*)RxData);
            flag_nrf = 0;
            memset(RxData, 0, sizeof(RxData));
        }

        // Перезапускаем прием данных
        HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 16);
        HAL_Delay(10);
    }
}

/*
void handleADCCommand(void) {
    int voltage = readADCVoltage();
    sprintf(TxData, "<ok> ADC = %d V\r\n", voltage);
    sendData(TxData);
}

void handleTemperatureCommand(void) {
    int temperature = readInternalTemperature();
    sprintf(TxData, "<ok> T = %d C\r\n", temperature);
    sendData(TxData);
}


int readADCVoltage(void) {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    uint32_t adcValue = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);

    // Переводим значение АЦП в вольты с учетом делителя напряжения
    float voltage = (adcValue * 3.3f / 4095.0f) * (470.0f + 330.0f) / 470.0f;
    return (int)voltage;
}

int readInternalTemperature(void) {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
    uint32_t adcValue = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);

    // Переводим значение АЦП в температуру (примерная формула для STM32F0)
    float vsense = adcValue * 3.3f / 4095.0f;
    float temperature = (vsense - 0.76f) / 0.0025f + 25.0f;
    return (int)temperature;
}
*/

#define MAX_DATA_SIZE            4    // Максимальный размер данных для чтения/записи
#define MAX_RESPONSE_LEN         64    // Максимальная длина ответа
/**
  * @brief Обработчик команд для работы с ZSC31050
  * @param command: строка команды (формат: "zsc<команда>")
  */
void handleZSCCommand(char *command) {
    if (strlen(command) < 4) {
        sendData("ERROR: Invalid command format\r\n");
        return;
    }

    char response[MAX_RESPONSE_LEN] = {0};
    char cmd_type = command[3]; // 4-й символ - тип команды

    switch (cmd_type) {
    case 'r': { // Чтение регистра
        if (strlen(command) < 6) {

            sendData("ERROR: Invalid read command format\r\n");
            return;
        }

        // Парсинг адреса регистра (2 символа)
        char reg_str[3] = {command[4], command[5], '\0'};
        uint8_t reg = (uint8_t)strtol(reg_str, NULL, 16);

        // Чтение регистра
        uint16_t value;
        if (ZSC31050_ReadTwoBytes(&hi2c1, reg, &value) == HAL_OK) {
        	snprintf(response, MAX_RESPONSE_LEN, "<ans zsc> %02x: %04x\r\n", reg, value);
        	sendData(response);
        } else {
            sendData("ERROR: Read failed\r\n");
        }
        break;
    }
        case 'w': { // Запись в регистр
            if (strlen(command) < 8) {
                sendData("ERROR: Invalid write command format\r\n");
                return;
            }

            // Парсинг адреса регистра (2 символа), количества байт (1 символ) и данных (2 или 4 символа)
            char reg_str[3] = {command[4], command[5], '\0'};
            uint8_t reg = (uint8_t)strtol(reg_str, NULL, 16);
            uint8_t size = command[6] - '0';

            if (size != 1 && size != 2) {
                sendData("ERROR: Invalid size (must be 1 or 2)\r\n");
                return;
            }

            // Парсинг данных
            uint8_t data[2] = {0};
            if (size == 1) {
                char data_str[3] = {command[7], command[8], '\0'};
                data[0] = (uint8_t)strtol(data_str, NULL, 16);
            } else {
                char data_str_high[3] = {command[7], command[8], '\0'};
                char data_str_low[3] = {command[9], command[10], '\0'};
                data[0] = (uint8_t)strtol(data_str_high, NULL, 16);
                data[1] = (uint8_t)strtol(data_str_low, NULL, 16);
            }

            if (ZSC31050_I2C_WriteRegister(&hi2c1, reg, data, size) == HAL_OK) {
                snprintf(response, MAX_RESPONSE_LEN, "<ans zsc> Write OK\r\n");
                sendData(response);
                HAL_Delay(10);
                snprintf(response, MAX_RESPONSE_LEN, "zscr%02x%d\r\n", reg, size);
                //char comm = "zscr"+(char) reg + (char) size + "\0";
                handleZSCCommand (response);
            } else {
                sendData("ERROR: Write failed\r\n");
            }
            break;
        }

        case 'c': { // Отправка команды по номеру (2 символа)
            if (strlen(command) < 6) {
                sendData("ERROR: Invalid command number format\r\n");
                return;
            }

            char cmd_str[3] = {command[4], command[5], '\0'};
            uint8_t cmd = (uint8_t)strtol(cmd_str, NULL, 16);

            if (ZSC31050_I2C_SendCommand(&hi2c1, cmd) == HAL_OK) {
                snprintf(response, MAX_RESPONSE_LEN, "<ans zsc> Command %02x sent\r\n", cmd);
                sendData(response);
            } else {
                sendData("ERROR: Command send failed\r\n");
            }
            break;
        }

        /*case 'c': { // Альтернативные команды (ccm, cnom, com)
            if (strlen(command) < 4) {
                sendData("ERROR: Invalid command format\r\n");
                return;
            }

            if (strncmp(&command[3], "ccm", 3) == 0) {
                // START_CM (0x72)
                if (ZSC31050_I2C_SendCommand(&hi2c1, 0x72) == HAL_OK) {
                    sendData("<ans zsc> START_CM sent\r\n");
                } else {
                    sendData("ERROR: START_CM failed\r\n");
                }
            } else if (strncmp(&command[3], "cnom", 4) == 0) {
                // START_NOM (0x71)
                if (ZSC31050_I2C_SendCommand(&hi2c1, 0x71) == HAL_OK) {
                    sendData("<ans zsc> START_NOM sent\r\n");
                } else {
                    sendData("ERROR: START_NOM failed\r\n");
                }
            } else if (strncmp(&command[3], "com", 3) == 0) {
                // START_OM (0x70)
                if (ZSC31050_I2C_SendCommand(&hi2c1, 0x70) == HAL_OK) {
                    sendData("<ans zsc> START_OM sent\r\n");
                } else {
                    sendData("ERROR: START_OM failed\r\n");
                }
            } else {
                sendData("ERROR: Unknown command\r\n");
            }
            break;
        }*/

        default:
            sendData("ERROR: Unknown command type\r\n");
            break;
    }
}

void handleNRFCommand(char *command) {
    if (command[3] == 'r') {
        sprintf(TxData, "<ans> nrf read\r\n");
        sendData(TxData);
    }
    else if (command[3] == 'w') {
        sprintf(TxData, "<ans> nrf write\r\n");
        sendData(TxData);
    }
}

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
  MX_DMA_Init();
  //MX_ADC_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  //MX_SPI1_Init();
  /* USER CODE BEGIN 2 */


  HAL_UARTEx_ReceiveToIdle_IT(&huart1, RxData, 16);

  // Инициализация ZSC31050
  sprintf(TxData, "Starting ZSC31050 initialization...\r\n");
  sendData(TxData);
  HAL_Delay(20);

  // Инициализация микросхемы ZSC31050
  //ZSC31050_Initialize();

  sprintf(TxData, "complete ZSC31050 initialization...\r\n");
  sendData(TxData);
  HAL_Delay(20);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  if (LcdCommand != 0) {handleLcdCommand();}
	  if (SetCommand != 0) {handleSetCommand();}
	  HAL_Delay(10);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  while (1)
  {
	  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET); // Включаем светодиод
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET); // Включаем светодиод
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_SET); // Включаем светодиод
	  HAL_Delay(200);
	  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET); // Включаем светодиод
	  HAL_Delay(400);

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
