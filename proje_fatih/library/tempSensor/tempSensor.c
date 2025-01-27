/*include standard C library files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/*include MCU - operating system interface files*/
#include "cmsis_os.h"

/*include user defined files*/
#include "tempSensor.h"   

//declare temperature sensor global variable
tsTempSensor gsTempSensor;  
bool gbIsTempRead;

/**
 * @brief configure dht11 struct with given parameter
 * @param htim TIMER for calculate delays ex:&htim2
 * @param port GPIO port ex:GPIOA
 * @param pin GPIO pin ex:GPIO_PIN_2
 * @param dht struct to configure ex:&dht
 */
void initTempSensor(tsTempSensor *apTempSensor, TIM_HandleTypeDef *htim, GPIO_TypeDef* port, uint16_t pin)
{
    apTempSensor->htim = htim;
    apTempSensor->port = port;
    apTempSensor->pin = pin;
}

/**
 * @brief set DHT pin direction with given parameter
 * @param dht struct for dht
 * @param pMode GPIO Mode ex:INPUT or OUTPUT
 */
void setTempSensorMode(tsTempSensor *apTempSensor, uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    //sensor mode is output
    if(mode == OUTPUT)
    {
        GPIO_InitStruct.Pin = apTempSensor->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(apTempSensor->port, &GPIO_InitStruct);
    }
    //sensor mode is input    
    else if(mode == INPUT)
    {
	GPIO_InitStruct.Pin = apTempSensor->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(apTempSensor->port, &GPIO_InitStruct);
    }
}

/**
 * @brief reads dht11 value
 * @param dht struct for dht11
 * @return 1 if read ok 0 if something wrong in read
 */
uint8_t readTemp(tsTempSensor *apTempSensor)
{
    uint16_t mTime1 = 0, mTime2 = 0, mBit = 0;
    uint8_t tempVal = 0;
    uint8_t mData[40];

    //set sensor mode to output
    setTempSensorMode(apTempSensor, OUTPUT);
    //set pin direction as input
    HAL_GPIO_WritePin(apTempSensor->port, apTempSensor->pin, GPIO_PIN_RESET);
    //wait 18 ms in Low state
    osDelay(18);
        
    //disable all interupts to read the sensor properly    
    __disable_irq();
    //start the timer reserved for the sensor
    HAL_TIM_Base_Start(apTempSensor->htim);
    //set sensor mode to input
    setTempSensorMode(apTempSensor, INPUT);
	
    //set timer counter to zero
    __HAL_TIM_SET_COUNTER(apTempSensor->htim, 0);				
    while(HAL_GPIO_ReadPin(apTempSensor->port, apTempSensor->pin) == GPIO_PIN_SET)
    {
        if((uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim) > 500)
        {
            __enable_irq();
            return 0;
        }
    }
    
    //set timer counter to zero
    __HAL_TIM_SET_COUNTER(apTempSensor->htim, 0);				
    while(HAL_GPIO_ReadPin(apTempSensor->port, apTempSensor->pin) == GPIO_PIN_RESET)
    {
        if((uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim) > 500)
        {
            __enable_irq();
            return 0;
        }
    }
	
    mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim);
    
    __HAL_TIM_SET_COUNTER(apTempSensor->htim, 0);				
    while(HAL_GPIO_ReadPin(apTempSensor->port, apTempSensor->pin) == GPIO_PIN_SET)
    {
        if((uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim) > 500)
        {
            __enable_irq();
            return 0;
        }
    }
    
    mTime2 = (uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim);

    //if answer is wrong return
    if(mTime1 < 75 && mTime1 > 85 && mTime2 < 75 && mTime2 > 85)
    {
	__enable_irq();
	return 0;
    }

    for (int j = 0; j < 40; j++)
    {
	__HAL_TIM_SET_COUNTER(apTempSensor->htim, 0);
	while(HAL_GPIO_ReadPin(apTempSensor->port, apTempSensor->pin) == GPIO_PIN_RESET)
        {
	    if((uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim) > 500)
            {
	        __enable_irq();
		return 0;
	    }
	}
	
        __HAL_TIM_SET_COUNTER(apTempSensor->htim, 0);
	while(HAL_GPIO_ReadPin(apTempSensor->port, apTempSensor->pin) == GPIO_PIN_SET)
        {
	    if((uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim) > 500)
            {
	        __enable_irq();
	        return 0;
	    }
	}
	
        mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(apTempSensor->htim);

	//check pass time in high state
	//if pass time 25uS set as LOW
	if(mTime1 > 20 && mTime1 < 30)
	{
	    mBit = 0;
	}
        
	else if(mTime1 > 60 && mTime1 < 80) //if pass time 70 uS set as HIGH
	{
	    mBit = 1;
	}

	//set i th data in data buffer
	mData[j] = mBit;

    }

    HAL_TIM_Base_Stop(apTempSensor->htim); //stop timer
    __enable_irq(); //enable all interrupts

    //get temp value from data buffer
    for (int i = 16; i < 24; i++)
    {
	tempVal += mData[i];
	tempVal = tempVal << 1;
    }
		
    tempVal = tempVal >> 1;
    apTempSensor->temperature = tempVal;
    
    return 1;
}