#include "Power.h"



void Power_init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;
 
    /*V_BAT*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立工作模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel =1;
    ADC_Init(ADC1,&ADC_InitStructure);
    ADC_Cmd(ADC1,ENABLE); //使能ADC1
    ADC_ResetCalibration(ADC1); //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
    ADC_StartCalibration(ADC1); //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));//等待校准结束
}

//获取某个通道的ADC值
float Get_Adc(uint8_t ch)
{
    uint16_t adc;
    float Vbat;
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

//将通道0的值计算电池电压值
float Power_Get_VBAT(void)
{
    float adc;
    adc = Get_Adc(0);
    adc = adc + Get_Adc(0);
    adc = adc + Get_Adc(0);
    adc = adc + Get_Adc(0);
    adc = adc / 4;
    float g_VBat  = (adc * 3) * 3.3 / 4096 ;//电阻1/3分压
    return g_VBat;
}