#ifndef _Wheel_H
#define _Wheel_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "sys.h"

#define wheel_RCC		 RCC_APB2Periph_GPIOB
#define wheel_Port		 GPIOB
#define Left_wheelA_Pin 	 GPIO_Pin_9
#define Left_wheelB_Pin 	 GPIO_Pin_8

#define Right_wheelA_Pin	 GPIO_Pin_4
#define Right_wheelB_Pin  GPIO_Pin_5

void Wheel_GPIO_Init(void);
void Leftwheel_Go();
void  Leftwheel_Back();
void  Leftwheel_Stop();
void  Rightwheel_Go();
void  Rightwheel_Back();
void Rightwheel_Stop();
void  LeftwheelPWM(int Speed);
void  RightwheelPWM(int Speed);
void Car_Run(int Speed);
void Car_Back(int Speed);
void Car_Stop(void);
void Car_Left(int Speed);
void Car_Right(int Speed);
void Car_SpinLeft(int LeftSpeed, int RightSpeed);
void Car_SpinRight(int LeftSpeed, int RightSpeed);
void Car_Left_Indie(int Speed);
void Car_Right_Indie(int Speed);
#endif
