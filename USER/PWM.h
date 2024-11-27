#ifndef _PWM_H
#define _PWM_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"


void PWM_LEDInit(u16 arr,u16 psc);
void BreathingLight();
void wheel_PWM_Init(u16 arr, u16 psc, u16 arr2, u16 psc2 );

#endif