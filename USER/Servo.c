#include "Servo.h"
#include "HC-SR04.h"

/*控制超声波模块的舵机转向
 *函数功能：初始化定时器1 的PWM 通道4 - PA11
 */
void ServoPWMInit (){
//1.GPIO初始化配置，定时器1通道4对应IO口为PA11
	GPIO_InitTypeDef GPIO_InitStructure;//创建gpio初始化结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PA端口对应时钟
 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//配置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA11
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//输出速度为50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化端口PA0
	
	//2.定时器配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//创建定时器配置结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能**高级**定时器1时钟
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//初始化结构体参数
	
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;//设置预分频值psc=7200
	TIM_TimeBaseInitStructure.TIM_Period=200-1;//设置自动重装值arr=200
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//初始化定时器
	
	//3.输出比较配置
	TIM_OCInitTypeDef TIM_OCInitStructure;//创建输出比较配置结构体
	TIM_OCStructInit(&TIM_OCInitStructure);//初始化比较配置结构体，方便配置其他定时器
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//设置输出比较模式为PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//有效电平为高电平
	TIM_OCInitStructure.TIM_Pulse=0;//设置比较寄存器值为0，后续在程序中动态修改
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//启用比较输出通道
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);//初始化比较输出配置
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//设置TIMx的PWM主输出为使能
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//设置TIMx的PWM主输出为使能
	
	ServoMid();
}

/*
	TIM_SetCompare4(TIM1,5);//占空比为5/200，舵机角度为0°
	TIM_SetCompare4(TIM1,15);//占空比为15/200，舵机角度为90°
	TIM_SetCompare4(TIM1,25);//占空比为25/200，舵机角度为180°
*/

//向左45°
void ServoLeft(){
	TIM_SetCompare4(TIM1,23);
}

//向权力45°
void ServoRight(){
	TIM_SetCompare4(TIM1,7);
}

//回正
void ServoMid(){
	TIM_SetCompare4(TIM1,15);
}

int BlockDetection(){
		float L1,L2,L3; 

		ServoMid();
		delay_ms(1000);//延时1s
		L1=HC_SR04_GetLenght();//单位cm
		ServoLeft();
		delay_ms(1000);//延时1s
		L2=HC_SR04_GetLenght();//单位cm
		ServoRight();
		delay_ms(1000);//延迟1s
		L3=HC_SR04_GetLenght();//单位cm
		ServoMid();
		delay_ms(500);
		if(L1>50){
			return 1;//直行.
		}else if(L2>50&&L3>50){
			return 3;//右转
		}else if(L2<L3){
            return 0; // 左边障碍,右转
		} else {
            return 2; // 右边障碍,左转
		}
   


}




