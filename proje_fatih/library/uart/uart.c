/*include standard C library files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
//#include <assert.h>

/*include MCU HAL files*/
#include "stm32f4xx_hal.h"

/*include user defined files*/
#include "uart.h"
#include "common.h"

tsUartRcvPack gsUartRcvPack;

void initUARTReceive(void)
{	
    HAL_TIM_Base_Start_IT(&htim3);
    __HAL_TIM_DISABLE(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    HAL_UART_Receive_IT(&huart3, gsUartRcvPack.maTempArr, UART_RX_INT_BYTE_SIZE);	
}

void USART3_TIMER_IRQHandler(void)
{  
  __HAL_TIM_DISABLE(&htim3);
  __HAL_TIM_SET_COUNTER(&htim3, 0);  
  
  if(gsUartRcvPack.muIndex > 0)
  {
      memcpy(gsUartRcvPack.maDataArr, gsUartRcvPack.maTempArr, gsUartRcvPack.muIndex);
      gsUartRcvPack.muDataLen = gsUartRcvPack.muIndex;      
      gsUartRcvPack.muIndex = 0;
      HAL_UART_AbortReceive_IT(&huart3);
  }      
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  __HAL_UART_CLEAR_FLAG(&huart3,UART_FLAG_RXNE);
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);  
  
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_ENABLE(&htim3);
  
  gsUartRcvPack.muIndex++; 
      
  if(gsUartRcvPack.muIndex == UART_RX_BUFFER_SIZE)
  {
      gsUartRcvPack.muIndex = 0;        
  }
  
  HAL_UART_Receive_IT(&huart3, &gsUartRcvPack.maTempArr[gsUartRcvPack.muIndex], UART_RX_INT_BYTE_SIZE);
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(&huart3, &gsUartRcvPack.maTempArr[gsUartRcvPack.muIndex], UART_RX_INT_BYTE_SIZE);
}

void uartSend(uint8_t* apData, uint16_t auDataLen)
{        
    uint16_t k;                
        
    if (auDataLen > 0)
    { 
        k=0;
       
        while(apData[k])
        {                  
           HAL_UART_Transmit(&huart3, &apData[k], UART_TX_BYTE_SIZE, HAL_MAX_DELAY);                
           k++;
        }               
    }                
}
        
          







