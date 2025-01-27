/*include standard C library header files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//#include <assert.h>

/*include MCU specific files*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/*include user defined files*/
#include "userInterface.h"
#include "uart.h"
#include "tempSensor.h"
#include "common.h"
#include "main.h"
#include "rtc_setting.h"
#include "eeprom.h"

uint8_t checkReceivedMsg(uint8_t *apData, uint8_t auDataLen)
{  
    uint8_t luResponseMsgLen = 0;
    bool lbIsValid = true;

    if(auDataLen == 0) 
    {
      return 0;
    }

    switch(*apData)
    {
        case REQUEST_COMMAND:
            if (apData[1] == '\n' && apData[2] == '\r')
            {
                gbIsLogToSend = true;
            }
            else    
            {
                lbIsValid = false;
            }        
            break;
      
        case HIGH_THR_COMMAND:
            if (apData[1] <= ASCII_NINE && apData[1] >= ASCII_ZERO)
            {
                if (apData[2] <= ASCII_NINE && apData[2] >= ASCII_ZERO)
                {
                    if (apData[3] == LINE_FEED && apData[4] == CR_RETURN)
                    {
                        uint8_t luHighThreshold = (apData[1] - '0') * 10 + (apData[2] - '0');
                        
                        if (luHighThreshold > guLowThreshold)
                        {
                            guHighThreshold = luHighThreshold;
                            eepromWriteBytes(ADDR_HIGH_THRESHOLD, &guHighThreshold, sizeof(guHighThreshold)); 
                        }
                
                        else
                        {
                            lbIsValid = false;
                        } 
                    }
                }
             }
      
             else      
             {
                 lbIsValid = false;
             }
             break;
      
         case LOW_THR_COMMAND:
             if (apData[1] <= ASCII_NINE && apData[1] >= ASCII_ZERO)
             {
                 if (apData[2] <= ASCII_NINE && apData[2] >= ASCII_ZERO)
                 {
                     if (apData[3] == LINE_FEED && apData[4] == CR_RETURN)
                     {
                         uint8_t luLowThreshold = (apData[1] - '0') * 10 + (apData[2] - '0');
                
                         if (luLowThreshold < guHighThreshold)
                         {
                             guLowThreshold = luLowThreshold;
                             eepromWriteBytes(ADDR_LOW_THRESHOLD, &guLowThreshold, sizeof(guLowThreshold));
                         }
                
                         else
                         {
                             lbIsValid = false;
                         } 
                      }
                  }
              }
              
              else
              {
                  lbIsValid = false;
              }
              break;
      
          default:      
              lbIsValid = false;
              break;
    }
        
    if (lbIsValid == false)
    {
        sprintf(s_print_data, "Invalid command!");
        luResponseMsgLen = strlen(s_print_data);
    } 

    return luResponseMsgLen;
}


