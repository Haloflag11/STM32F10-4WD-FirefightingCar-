#ifndef _LINE_H
#define _LINE_H

#define PREDELAY 100
#define POSTDELAY 800

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"
#include "stdio.h"
static float err,last_err;

typedef struct{
	int L1;
	int L2;
	int R2;
	int R1;

}LineStatus;

static float I=0;
static int high_speed_cnt=0;
static int baseSpeed=1200;
static int zeroCnt=0;
static int leftMotorSpeed;
static int rightMotorSpeed;
static int lastLeftMotorSpeed;
static int lastRightMotorSpeed;
static float lastI;
static float lastErr;
static LineStatus* lastStatus;
void LineInit();
void ActionFA();
LineStatus* Line_Detection();
int  CacuPwm();
void CarLineRun();


#endif