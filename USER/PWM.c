#include  "PWM.h"
//**********************************************************************************************
void PWM_LEDInit(u16 arr,u16 psc){
	//Tim3  CH3  部分重映射（或者没有重映射）  PB0绿灯
    GPIO_InitTypeDef GPIO_InitStructure;              //定义GPIO结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;    //定义TIMx定时器结构体
    TIM_OCInitTypeDef TIM_OCInitStructure;            //定义定时器脉宽调制结构体
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);                     //使能TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//使能GPIOB时钟和AFIO复用时钟
    
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);                     //TIM3部分重映射 TIM3_CH3->PB0（5）
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                               //TIM_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                         //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;                       //配置输出速率
    GPIO_Init(GPIOB,&GPIO_InitStructure);                                   //初始化GPIOB
    
    TIM_TimeBaseStructure.TIM_Period = arr;                                 //设置自动重装载寄存器周期的值 arr=value-1
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                              //设置预分频值 psc=value-1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);                          //初始化TIMx时间基数
    
    //初始化TIM3 Channel2 PWM模式     
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //使能比较输出
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //输出极性:TIM输出比较极性高
    TIM_OC3Init(TIM3,&TIM_OCInitStructure);                                 //根据T指定的参数初始化外设TIM3 OC3
 
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);                        //使能TIM3在CCR3上的预装载寄存器
    TIM_Cmd(TIM3, ENABLE);                                                  //使能TIM3
}

void BreathingLight(){
    u16 pwmValue = 0;
    u8 dir = 0;
    delay_init();

    PWM_LEDInit(999, 719);
 
    while(1){
        if(dir) {//下降
            if(pwmValue > 100) 
							TIM_SetCompare3(TIM3, --pwmValue);
            else 
							dir = 0;
        } else {//上升
            if(pwmValue < 990) 
							TIM_SetCompare3(TIM3, ++pwmValue);
            else 
							dir = 1;
        }
        delay_ms(3);
    }
}


//*****************************************************************************************************
//两边轮子的pwm初始化  Motor_PWM_Init(7200, 0, 7200, 0);	不分频。PWM频率 72000000/7200=10khz	  
void wheel_PWM_Init(u16 arr, u16 psc, u16 arr2, u16 psc2 ){  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//PWMA  PB7
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
	
	//PWMB PB6
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
 
   //设置该引脚为复用输出功能,输出TIM4 CH1的PWM脉冲波形	GPIOB.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM4_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr2; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc2; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIM3
}

