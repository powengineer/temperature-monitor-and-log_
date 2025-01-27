#ifndef _COMMON_H_
#define _COMMON_H_

#define SYS_LED_PORT            GPIOD              
#define SYS_LED_PIN             GPIO_PIN_12   
#define SYSTEM_TIMER_TIC        500

#define LED_RED_PORT            GPIOD
#define LED_RED_PIN             GPIO_PIN_11

#define LED_GREEN_PORT          GPIOD
#define LED_GREEN_PIN           GPIO_PIN_9

extern UART_HandleTypeDef       huart3;
extern TIM_HandleTypeDef        htim3;

void blinkSysLED (void);
void initGlobalVars (void);
#endif /*_COMMON_H_*/