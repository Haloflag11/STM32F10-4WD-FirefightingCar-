#ifndef _FIRE_H
#define _FIRE_H

#include "stm32f10x.h"      
#include "delay.h"
#include "usart.h"
 
extern uint16_t Fire1;
extern uint16_t Fire2;
extern uint16_t Fire3;
extern uint16_t Fire4;
extern uint16_t Fire5;

void FireInit(void);
uint16_t GetValue(uint8_t ADC_Channel);
void GetFire5_1();
void Wildfire_Barrier_Fan_Start(void);
void Wildfire_Barrier_Fan_Stop(void);
void alignToFireSource(int Fire1, int Fire3, int Fire5);
#endif