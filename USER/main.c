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
#include "Power.h"
#include "Line.h"
#include "Servo.h"
#include "Fire.h"

extern float L;

extern uint16_t Fire1;
extern uint16_t Fire2;
extern uint16_t Fire3;
extern uint16_t Fire4;
extern uint16_t Fire5;


float volt;
static uint16_t fireCnt=0;


int main(void){
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	delay_init();
	HC_SR04_Init();
	LED_Init();
	BlueToothInit();
	Wheel_GPIO_Init();
	wheel_PWM_Init(7200,0,7200,0);
	uart_init(9600);
	LineInit();
	Power_init();
  volt = Power_Get_VBAT();
	
	BEEP_Init();
	
	ServoPWMInit();//舵机
	
	FireInit();
	L=50;//初始化
	
	while(1){
		//L1:
		//lengthCnt++;
		//if(lengthCnt==50)
		//{
			//L=HC_SR04_GetLenght();
			//lengthCnt=0;
		//}
		
		if(fireCnt==30){
			GetFire5_1();
			fireCnt=0;
		}else{
			fireCnt++;
		}

		if(L < 23){//距离过小
			Car_Stop();
			setColor(1,0,0);
			delay_ms(3000);
			for(int i=0;i<3;i++){//鸣笛
			BeepON();
			delay_ms(100);
			BeepOFF();
			delay_ms(L*2);
			}
			Car_SpinRight(2500,2500);//调头
			delay_ms(400);
			while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			
		}else if(Fire1>0xE00||Fire3>0xE00||Fire5>0xE00){
			BeepON();
			setColor(1,1,0);
			Car_Stop();
			delay_ms(1000);
			int dir=Fire5>Fire1?0:1;
			if(dir==0){
				Car_SpinLeft(1500,1500);
				while(Fire3<0xFF0){
					GetFire5_1();			
				}

			}else{
				Car_SpinRight(1500,1500);
				while(Fire3<0xFF0){GetFire5_1();}
			}
			Car_Run(1200);
			delay_ms(1600);
			Car_Stop();
			Wildfire_Barrier_Fan_Start();
			for(int i=0;i<8;i++){
				delay_ms(50000);
			}
			Wildfire_Barrier_Fan_Stop();
			BeepOFF();
			Car_Back(1200);
			delay_ms(1600);
			if(dir==0){
				Car_SpinRight(1500,1500);
				while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			}else{
				Car_SpinLeft(1500,1500);
				while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			}
			Fire1=0;Fire3=0;Fire5=0;
		}
		else{  
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
			setColor(0,1,0);
			ActionFA();
			CarLineRun();
			Send_SensorData(L, volt , 0,0,0,0);
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
		}
	
		
		/*
		if(L < 20){
		BeepON();
		delay_ms(100);
		BeepOFF();
	  delay_ms(L*20);
		}
		*/
		
			
		

		


	}
	
}
