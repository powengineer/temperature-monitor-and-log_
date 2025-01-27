#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "eeprom.h"
#include "rtc_setting.h"
#include "tempSensor.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c3;
uint16_t guMemAddr;

void recordLogData(void)
{      
    eepromWriteBytes(guMemAddr, &gsTempSensor.temperature, sizeof(gsTempSensor.temperature));
    guMemAddr += sizeof(gsTempSensor.temperature);
    
    eepromWriteBytes(guMemAddr, &sDate.WeekDay, TIMESTAMP_DATE_SIZE);
    guMemAddr += TIMESTAMP_DATE_SIZE;
    
    eepromWriteBytes(guMemAddr, &sTime.Hours, TIMESTAMP_TIME_SIZE);
    guMemAddr += TIMESTAMP_TIME_SIZE;
    
    if (guMemAddr == EEPROM_SIZE)
    {
        guMemAddr = ADDR_OFFSET;
    }
}

int eepromWriteBytes(uint16_t auMemAddr, uint8_t *apData, uint16_t auTxBufferSize)
{    
    uint16_t luIndex = 0;
    uint8_t laDataArr[3];
    
    while (luIndex <auTxBufferSize)
    {
        while(HAL_I2C_IsDeviceReady(&hi2c3, SLAVE_ADDRESS, 10000, 10000) != HAL_OK);
        laDataArr[0] = ((auMemAddr + luIndex) >> 8);
        laDataArr[1] = (uint8_t)(auMemAddr + luIndex);
        laDataArr[2] = apData[luIndex];
	HAL_I2C_Master_Transmit(&hi2c3, SLAVE_ADDRESS, laDataArr, sizeof(laDataArr), 10000);
	luIndex++;
    }
    
    return 0;	
}

int eepromReadBytes(uint16_t auMemAddr, uint8_t *apData,uint16_t auRxBufferSize)
{
    uint8_t luAddr[2];
	
    while (HAL_I2C_IsDeviceReady(&hi2c3, SLAVE_ADDRESS, 10000, 10000) != HAL_OK);
		
    luAddr[0] = auMemAddr >> 8;
    luAddr[1] = (uint8_t)auMemAddr;
		
    HAL_I2C_Master_Transmit(&hi2c3, SLAVE_ADDRESS, luAddr, 2, 10000);
    HAL_I2C_Master_Receive(&hi2c3,  SLAVE_ADDRESS, apData, auRxBufferSize, 10000);		
	
    return 0;
}
	