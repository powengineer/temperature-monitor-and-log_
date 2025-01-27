
#include "stm32f4xx_hal.h"
#include "common.h"
#include "main.h"
#include "eeprom.h"
#include "tempSensor.h"
#include "rtc_setting.h"
#include "stm32f4xx_hal_rtc.h"


void initGlobalVars (void)
{
    gbIsLogToSend = false;
    gbIsTempRead = false;
    guMemAddr = ADDR_OFFSET;
        
    eepromReadBytes(ADDR_LOW_THRESHOLD, &guLowThreshold, sizeof(guLowThreshold));
    eepromReadBytes(ADDR_HIGH_THRESHOLD, &guHighThreshold, sizeof(guHighThreshold));    
}


void blinkSysLED (void)
{
    static uint32_t system_tic_led = 0;
  
    system_tic_led ++;
  
  if(system_tic_led > SYSTEM_TIMER_TIC)
  {
    HAL_GPIO_TogglePin (SYS_LED_PORT, SYS_LED_PIN);
    
    system_tic_led = 0;
  }
}