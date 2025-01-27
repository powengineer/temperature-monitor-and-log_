#ifndef _EEPROM_H_
#define _EEPROM_H_
#include <stdint.h>

#define SLAVE_ADDRESS		0xA0
#define ADDR_ISRTCSET           (uint16_t)0x00
#define ADDR_LOW_THRESHOLD      (uint16_t)0x01
#define ADDR_HIGH_THRESHOLD     (uint16_t)0x02
#define ADDR_OFFSET             (uint16_t)0x08
#define TEMP_OFFSET_BYTE        0
#define DATE_OFFSET_BYTE        1
#define TIME_OFFSET_BYTE        5
#define EEPROM_PAGE_SIZE        128
#define EEPROM_SIZE             32*1024
#define TEMP_SIZE               1
#define TIMESTAMP_DATE_SIZE     4
#define TIMESTAMP_TIME_SIZE     3
#define TOTAL_RECORD_SIZE       8

extern uint16_t guMemAddr;

void recordLogData(void);
int eepromWriteBytes(uint16_t auMemAddr, uint8_t *apData,uint16_t auTxBufferSize);
int eepromReadBytes(uint16_t auMemAddr, uint8_t *apData,uint16_t auRxBufferSize);

#endif /*EEPROM_H_*/



