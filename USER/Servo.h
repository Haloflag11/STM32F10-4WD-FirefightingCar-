#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "stdio.h"

void ServoPWMInit();
void ServoLeft();
void ServoRight();
void ServoMid();
int BlockDetection();
#endif