#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"
#include "stm32f4xx_hal.h"

extern uint8_t page;
extern uint8_t num;
extern uint8_t auto_count_enabled;

void KEY_scan(void);

#endif
