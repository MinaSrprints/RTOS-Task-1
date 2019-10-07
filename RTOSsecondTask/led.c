/*
 * led.c
 *
 *  Created on: ???/???/????
 *      Author: Mina
 */

#include "led.h"

void LED_Init(void)
{
	DIO_SetPinDirection(LEDPIN,OUTPUT);
	DIO_WritePin(LEDPIN,LOW);
}

void LED_Toggle(void)
{
	PORTA^=(1<<1);
}
