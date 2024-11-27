#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

#include "BEEP.h"

void BEEP_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC端口时钟
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化	
	
		GPIO_SetBits(GPIOA,GPIO_Pin_0);		//高电平是关，血泪
}

void BeepON(){
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);		  
}

void BeepOFF(){
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);			
}