#ifndef _RTC_SETTING_H_
#define _RTC_SETTING_H_
   
#define ASCII_ZERO      0x30
#define ASCII_NINE      0x39
#define LINE_FEED       0x0A
#define CR_RETURN       0x0D
   
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;

void getCurrentDateAndTime (void);
void convertDateToStr(char *apDateStr, RTC_DateTypeDef *apDate);
void convertTimeToStr(char *apTimeStr, RTC_TimeTypeDef *apTime);

#endif