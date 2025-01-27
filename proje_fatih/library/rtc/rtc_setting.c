#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "rtc_setting.h"
#include "stm32f4xx_hal_rtc.h"
#include "main.h"

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
    
void getCurrentDateAndTime (void)
{ 
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void convertDateToStr(char *apDateStr, RTC_DateTypeDef *apDate)
{
    bool lbIsValid = true;
    
    if (apDate->Date < 1 || apDate->Date > 31)
    {
        lbIsValid = false;
    }
    
    else if (apDate->Month < RTC_MONTH_JANUARY || apDate->Month > RTC_MONTH_DECEMBER) 
    {
        lbIsValid = false; 
    }
    
    else if (apDate->Year > 99)
    {
        lbIsValid = false; 
    }
    
    else if (apDate->WeekDay < RTC_WEEKDAY_MONDAY || apDate->WeekDay > RTC_WEEKDAY_SUNDAY) 
    {
        lbIsValid = false; 
    }
    
    if (lbIsValid == false)
    {
      sprintf(apDateStr, "Invalid Date!");
      return;
    }    
      
    sprintf(apDateStr, "%02d.%02d.20%02d ", apDate->Date, apDate->Month, apDate->Year);
    
    switch (sDate.WeekDay)
    {
        case RTC_WEEKDAY_MONDAY:
            strcat(apDateStr, "Monday");
            break;
        
        case RTC_WEEKDAY_TUESDAY:
            strcat(apDateStr, "Tuesday");
            break;
        
        case RTC_WEEKDAY_WEDNESDAY:
            strcat(apDateStr, "Wednesday");
            break;
        
        case RTC_WEEKDAY_THURSDAY:
            strcat(apDateStr, "Thursday");
            break;
        
        case RTC_WEEKDAY_FRIDAY:
            strcat(apDateStr, "Friday");
            break;      
        
        case RTC_WEEKDAY_SATURDAY:
            strcat(apDateStr, "Saturday");
            break;
        
        case RTC_WEEKDAY_SUNDAY:
            strcat(apDateStr, "Sunday");
            break;
            
        default:
            break;
    }
}

void convertTimeToStr(char *apTimeStr, RTC_TimeTypeDef *apTime)
{
    bool lbIsValid = true;
    
    if (apTime->Hours > 23)
    {
        lbIsValid = false;
    }
    
    else if (apTime->Minutes > 59)
    {
        lbIsValid = false; 
    }
    
    if (apTime->Seconds > 59)
    {
        lbIsValid = false; 
    }
    
    if (lbIsValid == false)
    {
      sprintf(apTimeStr, "Invalid Time!");
      return;
    }    
      
    sprintf(apTimeStr, "%02d:%02d:%02d", apTime->Hours, apTime->Minutes, apTime->Seconds);    
}
  
	