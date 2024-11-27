#include  "PWM.h"
//**********************************************************************************************
void PWM_LEDInit(u16 arr,u16 psc){
	//Tim3  CH3  ������ӳ�䣨����û����ӳ�䣩  PB0�̵�
    GPIO_InitTypeDef GPIO_InitStructure;              //����GPIO�ṹ��
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    //����TIMx��ʱ���ṹ��
    TIM_OCInitTypeDef TIM_OCInitStructure;            //���嶨ʱ��������ƽṹ��
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                     //ʹ��TIM3ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOBʱ�Ӻ�AFIO����ʱ��
    
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                     //TIM3������ӳ�� TIM3_CH3->PB0��5��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                               //TIM_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                         //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                       //�����������
    GPIO_Init(GPIOB,&GPIO_InitStructure);                                   //��ʼ��GPIOB
    
    TIM_TimeBaseStructure.TIM_Period = arr;                                 //�����Զ���װ�ؼĴ������ڵ�ֵ arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                              //����Ԥ��Ƶֵ psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                          //��ʼ��TIMxʱ�����
    
    //��ʼ��TIM3 Channel2 PWMģʽ     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //ʹ�ܱȽ����
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //�������:TIM����Ƚϼ��Ը�
    TIM_OC3Init(TIM3,&TIM_OCInitStructure);                                 //����Tָ���Ĳ�����ʼ������TIM3 OC3
 
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);                        //ʹ��TIM3��CCR3�ϵ�Ԥװ�ؼĴ���
    TIM_Cmd(TIM3, ENABLE);                                                  //ʹ��TIM3
}

void BreathingLight(){
    u16 pwmValue = 0;
    u8 dir = 0;
    delay_init();

    PWM_LEDInit(999, 719);
 
    while(1){
        if(dir) {//�½�
            if(pwmValue > 100) 
							TIM_SetCompare3(TIM3, --pwmValue);
            else 
							dir = 0;
        } else {//����
            if(pwmValue < 990) 
							TIM_SetCompare3(TIM3, ++pwmValue);
            else 
							dir = 1;
        }
        delay_ms(3);
    }
}


//*****************************************************************************************************
//�������ӵ�pwm��ʼ��  Motor_PWM_Init(7200, 0, 7200, 0);	����Ƶ��PWMƵ�� 72000000/7200=10khz	  
void wheel_PWM_Init(u16 arr, u16 psc, u16 arr2, u16 psc2 ){  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//PWMA  PB7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	GPIOB.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM4 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
	
	//PWMB PB6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
 
   //���ø�����Ϊ�����������,���TIM4 CH1��PWM���岨��	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM4_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period = arr2; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc2; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM3
}

