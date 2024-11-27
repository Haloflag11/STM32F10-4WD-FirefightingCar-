#ifndef _POWER_H
#define _POWER_H
#include "stm32f10x.h"                  // Device header
#include "TIMER.H"
#include "HC-SR04.H"
#include "delay.h"
#include "LED.h"
#include "BEEP.h"
#include "usart.h"
#include "bluetooth.h"
#include "PWM.h"
#include "Wheel.h"

void Power_init(void);
float Get_Adc(uint8_t ch);
float Get_Adc(uint8_t ch);
float Power_Get_VBAT(void);

#endif
