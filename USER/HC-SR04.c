#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "TIMER.H"

float L=500;

void HC_SR04_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能TIMx时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //禁能jtag
	
	//GPIO初始化 Trig(STM32发送触发信号给HC-SR04)PA15（11）
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出(高低电平均有驱动能力)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//GPIO初始化 Echo(STM32接收回波信号，计算距离)PA12（10）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//定时中断配置
	//选择时钟源
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100-1;//ARR 定时时间0.1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//PSC 1MHz（150HZ以上最好）
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//高级定时器采用，给0
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//解除初始化就进入中断（避免数据出错）
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能定时中断
	
	//配置NVIC 分配和管理中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	//失能定时
	TIM_Cmd(TIM2,DISABLE);
	
	TIM_TimeBaseInitTypeDef timInitStructure;
	NVIC_InitTypeDef                 nvicInitStructure;
	//1.配置定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//2.配置定时器结构体
	timInitStructure.TIM_ClockDivision    = TIM_CKD_DIV1;
	timInitStructure.TIM_CounterMode    = TIM_CounterMode_Up;
	timInitStructure.TIM_Period            = 5000-1;
	timInitStructure.TIM_Prescaler        = 7200-1;
	
	TIM_TimeBaseInit(TIM3, &timInitStructure);
	//3.开启定时器中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	
	nvicInitStructure.NVIC_IRQChannel                    = TIM3_IRQn;
	nvicInitStructure.NVIC_IRQChannelPreemptionPriority    = 1;
	nvicInitStructure.NVIC_IRQChannelSubPriority        = 1;
	nvicInitStructure.NVIC_IRQChannelCmd                = ENABLE;
	
	NVIC_Init(&nvicInitStructure);
}


//计算距离（平均值）
float HC_SR04_GetLenght(void){
	//求解距离，采用平均值最好
	int i=0;//次数变量
	unsigned int t=0;
	float Length=0;
	float Sum=0;
	
	for(i=0;i<1;i++)//计算3次
	{
		//触发信号（触发超声波模块工作）
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
		delay_us(15);//10US以上
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
		
		//获取Echo引脚的电平状态
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==0);//低电平就执行空操作
		
		//读取到高电平
		Timer_On();//打开定时器
		
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)==1);//高电平就执行空操作（直到读取完成）
		
		//低电平（接收结束）
		Timer_Off();//关闭定时器

		t=Timer_Get_Counter();//获取时间
		
		Length=((float)t/58.0);//计算距离
		
		
	}
	
	//计算距离的均值
	return Length;
}

void TIM3_IRQHandler(void){
	
	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				L=HC_SR04_GetLenght();
    }
		
        
}	








