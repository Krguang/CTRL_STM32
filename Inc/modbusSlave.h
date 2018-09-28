#ifndef __MODBUS_SLAVE_H_
#define  __MODBUS_SLAVE_H_

#include "stm32f1xx_hal.h"
#include "main.h"

uint16_t LocalStatusArray[];
uint16_t localData[];
void modbusPoll();


#endif // !__MODBUS_SLAVE_H_
