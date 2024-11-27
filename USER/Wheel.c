#include "Wheel.h"

void Wheel_GPIO_Init(void){		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*��������ʱ��*/
	RCC_APB2PeriphClockCmd(wheel_RCC, ENABLE); 
	/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
  	GPIO_InitStructure.GPIO_Pin = Left_wheelA_Pin | Left_wheelB_Pin | Right_wheelA_Pin | Right_wheelB_Pin;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��GPIOB*/
  	GPIO_Init(wheel_Port, &GPIO_InitStructure);		  
	/* �͵�ƽ	*/
	GPIO_ResetBits(wheel_Port, Left_wheelA_Pin | Left_wheelB_Pin | Right_wheelA_Pin | Right_wheelB_Pin);
 	//PB4�ر�JTAG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //����jtag
}


//�����ֵ���ͣ��
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
//�����ֵ���ͣ��
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
//�������ٶ�����0-7200
void  LeftwheelPWM(int Speed){	
	TIM_SetCompare2(TIM4, Speed);
}
//�������ٶ�����0-7200
void  RightwheelPWM(int Speed){
	TIM_SetCompare1(TIM4, Speed);
}	

//������ǰ��
void Car_Run(int Speed){
	Leftwheel_Go();
	Rightwheel_Go();
	LeftwheelPWM(Speed);		  
	RightwheelPWM(Speed);	
}


//����������
void Car_Back(int Speed){
	Leftwheel_Back();
	Rightwheel_Back();

	LeftwheelPWM(Speed);		  
	RightwheelPWM(Speed);	
}

//����ɲס
void Car_Stop(void){
	Leftwheel_Stop();
	Rightwheel_Stop();

	LeftwheelPWM(0);		  
	RightwheelPWM(0);		
}

//��ת���������ٶ�Ϊ0
void Car_Left(int Speed){
	Leftwheel_Go();
	Rightwheel_Go();

	LeftwheelPWM(Speed/3);		  
	RightwheelPWM(Speed);		
}


//��ת���������ٶ�Ϊ0
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


//��ת���������ٶ�Ϊ0
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

//��ʱ����ת����ת���ٶ���ͬʱ��ԭ����ת��
void Car_SpinLeft(int LeftSpeed, int RightSpeed){
	Leftwheel_Back();
	Rightwheel_Go();

	LeftwheelPWM(LeftSpeed);		  
	RightwheelPWM(RightSpeed);		
}


// ˳ʱ����ת����ת���ٶ���ͬʱ��ԭ����ת��
void Car_SpinRight(int LeftSpeed, int RightSpeed){
	Leftwheel_Go();
	Rightwheel_Back();

	LeftwheelPWM(LeftSpeed);		  
	RightwheelPWM(RightSpeed);		
}

