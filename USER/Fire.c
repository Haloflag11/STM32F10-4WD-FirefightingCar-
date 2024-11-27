#include "Fire.h"

/*
���ҵ���	D1/A1 - PA2 - ADC2ͨ��2    pa4 ADC2  ͨ��4
					D2/A2 - PA3 - ADC2ͨ��3    
					D3/A3 - PA7 - ADC2ͨ��7    pa6 ADC2  ͨ��6
					D4/A4 - PC0 - ADC2ͨ��10   
					D5/A5 - PC1 - ADC2ͨ��11   pa5 ADC2  ͨ��5
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
	
	  //GPIO�ڵ�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//PA4
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5
    GPIO_Init(GPIOA, &GPIO_InitStructure); 	  

	
		//ADC2������
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    ADC_DeInit(ADC2);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //��������ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //��ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ENABLE;//����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;            
    ADC_InitStructure.ADC_NbrOfChannel =1;//ͨ������Ϊ1������ɨ��ģʽ�£�����Ҫָ������1�������ڷ�ɨ��ģʽ�£�ֻ����1 
    ADC_Init(ADC2,&ADC_InitStructure);
		
    ADC_Cmd(ADC2,ENABLE); //ʹ��ADC2
    ADC_ResetCalibration(ADC2); //ʹ�ܸ�λУ׼
    while(ADC_GetResetCalibrationStatus(ADC2)); //�ȴ���λУ׼����
    ADC_StartCalibration(ADC2); //����ADУ׼
    while(ADC_GetCalibrationStatus(ADC2));//�ȴ�У׼����
		
		//Ұ����Ϸ��ȵĳ�ʼ��
	 GPIO_InitTypeDef GPIO_Fan;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   GPIO_Fan.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Fan.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Fan.GPIO_Pin = GPIO_Pin_1;
   GPIO_Init(GPIOA, &GPIO_Fan);
	 GPIO_SetBits(GPIOA, GPIO_Pin_1);   
	 


}

//Ұ����Ϸ��ȵĿ���
void Wildfire_Barrier_Fan_Start(void){
		 GPIO_ResetBits(GPIOA, GPIO_Pin_1);   
}

void Wildfire_Barrier_Fan_Stop(void){
		 GPIO_SetBits(GPIOA, GPIO_Pin_1);   
}

/*	��ȡADת����ֵ
		ADC_Channel ָ��ADת����ͨ����
		����ADת����ֵ����Χ��0~4095��
 */
uint16_t GetValue(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC2, ADC_Channel, 1, ADC_SampleTime_7Cycles5);	//��ÿ��ת��ǰ�����ݺ����β������Ĺ������ͨ��1
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);					//�������ADת��һ��
	while (ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);	//�ȴ�EOC��־λ�����ȴ�ADת������
	return ADC_GetConversionValue(ADC2);					//�����ݼĴ������õ�ADת���Ľ��
}




void GetFire5_1(){
		Fire1 = GetValue(ADC_Channel_4);
		Fire3 = GetValue(ADC_Channel_6);
		Fire5 = GetValue(ADC_Channel_5);
}


/*
void alignToFireSource(int Fire1, int Fire3, int Fire5) {
    const int threshold = 0xF00; // �趨һ����ֵ��ȷ���������Ƿ���Ч�ؼ�⵽����
    int Speed = 500; // �趨�����ת��

    if (Fire3 > threshold && Fire3 > Fire1 && Fire3 > Fire5) {
        // ����м䴫��������ǿ�Ķ������ҳ�����ֵ��С����ȷ��׼�����趯��
       Car_Stop();
    } else if (Fire5 > Fire1 && Fire5 > Fire3) {
        Car_SpinLeft(Speed,Speed);
    } else if (Fire1 > Fire5 && Fire1 > Fire3) {
        // ����Ҳഫ����������ߣ�����ת
        Car_SpinRight(Speed,Speed);
    }
}
*/