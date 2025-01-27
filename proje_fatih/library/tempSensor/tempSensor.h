#ifndef _TEMPSENSOR_H_
#define _TEMPSENSOR_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define OUTPUT          1
#define INPUT           0
#define MAX_TEMP        50

typedef struct
{  
    GPIO_TypeDef* port;	
    uint16_t pin; 
    TIM_HandleTypeDef *htim; 
    uint8_t temperature; 
}tsTempSensor;

void initTempSensor(tsTempSensor *apTempSensor, TIM_HandleTypeDef *htim, GPIO_TypeDef* port, uint16_t pin);
void setTempSensorMode(tsTempSensor *apTempSensor, uint8_t mode);
uint8_t readTemp(tsTempSensor *apTempSensor);

extern tsTempSensor gsTempSensor; 
extern bool gbIsTempRead;

#endif /* _TEMPSENSOR_H_ */
