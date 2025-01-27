#ifndef _UART_H_
#define _UART_H_

//define receive and transmit byte and buffer sizes
#define UART_RX_BUFFER_SIZE     10
#define UART_RX_INT_BYTE_SIZE   1
#define UART_TX_BYTE_SIZE       1

//define uart receive packet structure
typedef struct
{
    uint8_t maTempArr[UART_RX_BUFFER_SIZE]; //temporary array receiving bytes
    uint8_t maDataArr[UART_RX_BUFFER_SIZE]; //actual array to process the msg
    uint8_t muIndex;                        //receiving index
    uint8_t muDataLen;                      //received message data length 
}tsUartRcvPack;

//externalize the uart receive packet structure variable so that other files 
//can use it
extern tsUartRcvPack gsUartRcvPack;

//initialize UART receive process
void initUARTReceive(void);
//handle USART3 timer to detect the end of the received msg
void USART3_TIMER_IRQHandler(void);
//UART receive interrupt service routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
//UART receive cancellation interrupt service routine
void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart);
//send UART msg function
void uartSend(uint8_t* apData,uint16_t auDataLen);
#endif /*_UART_H_*/