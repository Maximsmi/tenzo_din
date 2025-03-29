#ifndef ZSC31050_H
#define ZSC31050_H

#include "stm32f0xx_hal.h"

// Адрес устройства ZSC31050
#define ZSC31050_ADDR (0x78 << 1)

// Команды для работы с микросхемой
#define ZSC31050_CMD_START_OM 		0x70
#define ZSC31050_CMD_START_NOM 		0x71
#define ZSC31050_CMD_START_CM 		0x72

#define START_CYC_EEP 	0x01
#define START_CYC_RAM 	0x02
#define READ_RAM0 		0x10
#define READ_RAM1 		0x20
#define READ_EEP0 		0x30
#define READ_EEP1 		0x40
#define CFG_SIF_TO_I2C 	0x52
#define SET_DAC 		0x60
#define COPY_EEP2RAM 	0xC0
#define COPY_RAM2EEP 	0xC3
#define GET_EEP_SIGN 	0xC8
#define GEN_EEP_SIGN 	0xC9
#define GET_RAM_SIGN 	0xCA
#define GEN_RAM_SIGN 	0xCB
#define CLEAR_EEP 		0xCC
#define ROM_VERSION 	0xCF


// Адреса регистров
#define CFGCYC_ADDR 0x16
#define CFGSIF_ADDR 0x17
#define CFGAPP_ADDR 0x18
#define CFGAFE_ADDR 0x19
#define CFGTMP_ADDR 0x1A
#define CFGOUT_ADDR 0x1B
#define ADJREF_ADDR 0x1C

// Команды из таблицы 4.6
#define WRITE_RAM0 0x80
#define WRITE_RAM1 0x90
#define WRITE_EEP0 0xA0
#define WRITE_EEP1 0xB0
#define COPY_RAM2EEP 0xC3
#define COPY_EEP2RAM 0xC0
#define GET_EEP_SIGN 0xC8
#define GEN_EEP_SIGN 0xC9
#define GET_RAM_SIGN 0xCA
#define GEN_RAM_SIGN 0xCB
#define CLEAR_EEP 0xCC
#define ROM_VERSION 0xCF

// Функции для работы с микросхемой ZSC31050
void ZSC31050_Initialize(void);
HAL_StatusTypeDef ZSC31050_I2C_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t size);
HAL_StatusTypeDef ZSC31050_I2C_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t size);
HAL_StatusTypeDef ZSC31050_I2C_SendCommand(I2C_HandleTypeDef *hi2c, uint8_t command);
HAL_StatusTypeDef ZSC31050_ReadTwoBytes(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *value);
#endif // ZSC31050_H
