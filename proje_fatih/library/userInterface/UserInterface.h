#ifndef _USERINTERFACE_H_
#define _USERINTERFACE_H_

//define the first char of user interface commands
#define REQUEST_COMMAND         'R'
#define LOW_THR_COMMAND         'L'
#define HIGH_THR_COMMAND        'H'

//check the received message whether it valid or not
uint8_t checkReceivedMsg(uint8_t *apData, uint8_t auDataLen);
#endif /*_USERINTERFACE_H_*/