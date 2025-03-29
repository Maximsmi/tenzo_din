/*
 * zsc31050.c
 *
 *  Created on: Mar 22, 2025
 *      Author: Bk5_460
 */

#include "zsc31050.h"
#include "i2c.h"  // Предполагаем, что у вас есть библиотека для работы с I²C
#include "stm32f0xx_hal.h"  // Для использования HAL_Delay
#include <string.h>
#include <stdio.h>
#include "main.h"

// Указатель на структуру I2C (должен быть инициализирован в main.c)
extern I2C_HandleTypeDef hi2c1;  // Пример для I2C1

void ZSC31050_Initialize(void) {
    // Перевод микросхемы в режим CM
    //ZSC31050_EnterCommandMode();

    // Настройка регистра CFGSIF
    //ZSC31050_ConfigureCFGSIF();

    // Другие настройки регистров
    // ...

    // Запуск измерений
    //uint8_t cmd = START_CYC_EEP;
    //if (I2C_Write(ZSC31050_ADDR, &cmd, 1) != HAL_OK) {
        // Обработка ошибки
    //}
}

/**
  * @brief  Чтение n байт из регистра ZSC31050 по I2C.
  * @param  hi2c: Указатель на структуру I2C (HAL).
  * @param  reg: Адрес регистра для чтения.
  * @param  data: Буфер для принятых данных.
  * @param  size: Количество байт для чтения.
  * @retval HAL статус (HAL_OK в случае успеха).
  */
HAL_StatusTypeDef ZSC31050_I2C_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t size) {
    // Отправка адреса регистра (запись)
    if (HAL_I2C_Master_Transmit(hi2c, ZSC31050_ADDR, &reg, 1, 5000) != HAL_OK) {
		return HAL_ERROR;
    }
/*
	while (HAL_I2C_GetError(hi2c) != HAL_I2C_STATE_READY)
	{
    		return HAL_ERROR;
    }
*/
    // Чтение данных
    for (uint16_t i = 0; i < size; i++) {
        if (HAL_I2C_Master_Receive(hi2c, ZSC31050_ADDR, &data[i], 1, 5000) != HAL_OK) {
            return HAL_ERROR;
        }
    }

    /*while (HAL_I2C_Master_Receive(hi2c,  (uint16_t)ZSC31050_ADDR, data, size, 1000) != HAL_OK) {
        if (HAL_I2C_GetError(hi2c) != HAL_I2C_ERROR_AF)
        {
        		return HAL_ERROR;
        }

    }*/

    return HAL_OK;
}

/**
  * @brief  Запись n байт в регистр ZSC31050 по I2C.
  * @param  hi2c: Указатель на структуру I2C (HAL).
  * @param  reg: Адрес регистра для записи.
  * @param  data: Буфер с данными для отправки.
  * @param  size: Количество байт для записи.
  * @retval HAL статус (HAL_OK в случае успеха).
  */
HAL_StatusTypeDef ZSC31050_I2C_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t size) {
    uint8_t tx_buffer[size + 1];
    tx_buffer[0] = reg; // Первый байт — адрес регистра
    memcpy(&tx_buffer[1], data, size); // Остальные байты — данные

    // Отправка адреса регистра и данных
    if (HAL_I2C_Master_Transmit(hi2c,  ZSC31050_ADDR, tx_buffer, size, 5000) != HAL_OK) {
		return HAL_ERROR;
    }
    /*
    while (HAL_I2C_Master_Transmit(hi2c,  ZSC31050_ADDR, tx_buffer, size, 5000) != HAL_OK) {
        if (HAL_I2C_GetError(hi2c) != HAL_I2C_ERROR_AF) {
        		return HAL_ERROR;
        }
    }
*/
    return HAL_OK;
}

/**
  * @brief  Отправка команды ZSC31050 по I2C.
  * @param  hi2c: Указатель на структуру I2C (HAL).
  * @param  command: Код команды (например, 0x72 для START_CM).
  * @retval HAL статус (HAL_OK в случае успеха).
  */
HAL_StatusTypeDef ZSC31050_I2C_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command) {
    // Отправка команды (1 байт)
    if (HAL_I2C_Master_Transmit(hi2c, ZSC31050_ADDR, &command, 1, 5000) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef ZSC31050_ReadTwoBytes(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *value) {
    uint8_t buffer[2];

    // Чтение двух байт
    if (ZSC31050_I2C_ReadRegister(hi2c, reg, buffer, 2) != HAL_OK) {
        return HAL_ERROR;
    }

    // Преобразование байтов в 16-битное значение (Big Endian)
    *value = (buffer[0] << 8) | buffer[1];

    return HAL_OK;
}
