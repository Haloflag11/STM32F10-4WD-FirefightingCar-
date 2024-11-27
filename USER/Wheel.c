#include "Wheel.h"

void Wheel_GPIO_Init(void){		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(wheel_RCC, ENABLE); 
	/*选择要控制的GPIOB引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = Left_wheelA_Pin | Left_wheelB_Pin | Right_wheelA_Pin | Right_wheelB_Pin;	
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*调用库函数，初始化GPIOB*/
  	GPIO_Init(wheel_Port, &GPIO_InitStructure);		  
	/* 低电平	*/
	GPIO_ResetBits(wheel_Port, Left_wheelA_Pin | Left_wheelB_Pin | Right_wheelA_Pin | Right_wheelB_Pin);
 	//PB4关闭JTAG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //禁能jtag
}


//左两轮的启停退
void Leftwheel_Go(){
	GPIO_SetBits(wheel_Port, Left_wheelA_Pin); 
	GPIO_ResetBits(wheel_Port, Left_wheelB_Pin);
}

void  Leftwheel_Back(){
	GPIO_ResetBits(wheel_Port, Left_wheelA_Pin); 
	GPIO_SetBits(wheel_Port, Left_wheelB_Pin);
}

void  Leftwheel_Stop(){
	GPIO_ResetBits(wheel_Port, Left_wheelA_Pin); 
	GPIO_ResetBits(wheel_Port, Left_wheelB_Pin);
}
//右两轮的启停退
void  Rightwheel_Go()	{
	GPIO_SetBits(wheel_Port, Right_wheelA_Pin); 
	GPIO_ResetBits(wheel_Port, Right_wheelB_Pin);
}

void  Rightwheel_Back(){
	GPIO_ResetBits(wheel_Port, Right_wheelA_Pin); 
	GPIO_SetBits(wheel_Port, Right_wheelB_Pin);
}

void Rightwheel_Stop(){
	GPIO_ResetBits(wheel_Port, Right_wheelA_Pin); 
	GPIO_ResetBits(wheel_Port, Right_wheelB_Pin);
}
//左两轮速度设置0-7200
void  LeftwheelPWM(int Speed){	
	TIM_SetCompare2(TIM4, Speed);
}
//右两轮速度设置0-7200
void  RightwheelPWM(int Speed){
	TIM_SetCompare1(TIM4, Speed);
}	

//四轮往前肿
void Car_Run(int Speed){
	Leftwheel_Go();
	Rightwheel_Go();
	LeftwheelPWM(Speed);		  
	RightwheelPWM(Speed);	
}


//四轮往后溜
void Car_Back(int Speed){
	Leftwheel_Back();
	Rightwheel_Back();

	LeftwheelPWM(Speed);		  
	RightwheelPWM(Speed);	
}

//四轮刹住
void Car_Stop(void){
	Leftwheel_Stop();
	Rightwheel_Stop();

	LeftwheelPWM(0);		  
	RightwheelPWM(0);		
}

//左转：左两轮速度为0
void Car_Left(int Speed){
	Leftwheel_Go();
	Rightwheel_Go();

	LeftwheelPWM(Speed/3);		  
	RightwheelPWM(Speed);		
}


//右转：右两轮速度为0
void Car_Right(int Speed){
	Leftwheel_Go();
	Rightwheel_Go();

	LeftwheelPWM(Speed);		  
	RightwheelPWM(Speed/3);		
}

void Car_Left_Indie(int Speed){
	if(Speed>=0){
	Leftwheel_Go();
	LeftwheelPWM(Speed);
	}
	else{
	Leftwheel_Back();
	LeftwheelPWM(-Speed);
	}
}


//右转：右两轮速度为0
void Car_Right_Indie(int Speed){
	if(Speed>=0){
	Rightwheel_Go();
	RightwheelPWM(Speed);
	}
	else{
	Rightwheel_Back();
	RightwheelPWM(-Speed);
	}
}

//逆时针旋转（自转；速度相同时，原地旋转）
void Car_SpinLeft(int LeftSpeed, int RightSpeed){
	Leftwheel_Back();
	Rightwheel_Go();

	LeftwheelPWM(LeftSpeed);		  
	RightwheelPWM(RightSpeed);		
}


// 顺时针旋转（自转；速度相同时，原地旋转）
void Car_SpinRight(int LeftSpeed, int RightSpeed){
	Leftwheel_Go();
	Rightwheel_Back();

	LeftwheelPWM(LeftSpeed);		  
	RightwheelPWM(RightSpeed);		
}

