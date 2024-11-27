#include "Fire.h"

/*
从右到左：	D1/A1 - PA2 - ADC2通道2    pa4 ADC2  通道4
					D2/A2 - PA3 - ADC2通道3    
					D3/A3 - PA7 - ADC2通道7    pa6 ADC2  通道6
					D4/A4 - PC0 - ADC2通道10   
					D5/A5 - PC1 - ADC2通道11   pa5 ADC2  通道5
*/

uint16_t Fire1=0;
uint16_t Fire2=0;
uint16_t Fire3=0;
uint16_t Fire4=0;
uint16_t Fire5=0;

void FireInit(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC2, ENABLE);
	
	  //GPIO口的配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	  

	
		//ADC2的配置
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    ADC_DeInit(ADC2);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立工作模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ENABLE;//单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;            
    ADC_InitStructure.ADC_NbrOfChannel =1;//通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1 
    ADC_Init(ADC2,&ADC_InitStructure);
		
    ADC_Cmd(ADC2,ENABLE); //使能ADC2
    ADC_ResetCalibration(ADC2); //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC2)); //等待复位校准结束
    ADC_StartCalibration(ADC2); //开启AD校准
    while(ADC_GetCalibrationStatus(ADC2));//等待校准结束
		
		//野火避障风扇的初始化
	 GPIO_InitTypeDef GPIO_Fan;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_Fan.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Fan.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Fan.GPIO_Pin = GPIO_Pin_1;
   GPIO_Init(GPIOA, &GPIO_Fan);
	 GPIO_SetBits(GPIOA, GPIO_Pin_1);   
	 


}

//野火避障风扇的开启
void Wildfire_Barrier_Fan_Start(void){
		 GPIO_ResetBits(GPIOA, GPIO_Pin_1);   
}

void Wildfire_Barrier_Fan_Stop(void){
		 GPIO_SetBits(GPIOA, GPIO_Pin_1);   
}

/*	获取AD转换的值
		ADC_Channel 指定AD转换的通道，
		返回AD转换的值（范围：0~4095）
 */
uint16_t GetValue(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC2, ADC_Channel, 1, ADC_SampleTime_7Cycles5);	//在每次转换前，根据函数形参灵活更改规则组的通道1
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);					//软件触发AD转换一次
	while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);	//等待EOC标志位，即等待AD转换结束
	return ADC_GetConversionValue(ADC2);					//读数据寄存器，得到AD转换的结果
}




void GetFire5_1(){
		Fire1 = GetValue(ADC_Channel_4);
		Fire3 = GetValue(ADC_Channel_6);
		Fire5 = GetValue(ADC_Channel_5);
}


/*
void alignToFireSource(int Fire1, int Fire3, int Fire5) {
    const int threshold = 0xF00; // 设定一个阈值以确定传感器是否有效地检测到火焰
    int Speed = 500; // 设定电机的转速

    if (Fire3 > threshold && Fire3 > Fire1 && Fire3 > Fire5) {
        // 如果中间传感器有最强的读数，且超过阈值，小车正确对准，无需动作
       Car_Stop();
    } else if (Fire5 > Fire1 && Fire5 > Fire3) {
        Car_SpinLeft(Speed,Speed);
    } else if (Fire1 > Fire5 && Fire1 > Fire3) {
        // 如果右侧传感器读数最高，向右转
        Car_SpinRight(Speed,Speed);
    }
}
*/