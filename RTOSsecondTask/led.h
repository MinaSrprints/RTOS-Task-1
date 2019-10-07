/*
 * led.h
 *
 *  Created on: ???/???/????
 *      Author: Mina
 */

#ifndef LED_H_
#define LED_H_
#include "DIO_operations.h"
#include "Memory_map.h"
#define LEDPIN  PinA1

extern void LED_Toggle(void);
extern void LED_Init(void);
#endif /* LED_H_ */
