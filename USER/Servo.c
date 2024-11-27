#include "Servo.h"
#include "HC-SR04.h"

/*���Ƴ�����ģ��Ķ��ת��
 *�������ܣ���ʼ����ʱ��1 ��PWM ͨ��4 - PA11
 */
void ServoPWMInit (){
//1.GPIO��ʼ�����ã���ʱ��1ͨ��4��ӦIO��ΪPA11
	GPIO_InitTypeDef GPIO_InitStructure;//����gpio��ʼ���ṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PA�˿ڶ�Ӧʱ��
 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//PA11
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//����ٶ�Ϊ50mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ���˿�PA0
	
	//2.��ʱ������
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//������ʱ�����ýṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//ʹ��**�߼�**��ʱ��1ʱ��
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//��ʼ���ṹ�����
	
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;//����Ԥ��Ƶֵpsc=7200
	TIM_TimeBaseInitStructure.TIM_Period=200-1;//�����Զ���װֵarr=200
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);//��ʼ����ʱ��
	
	//3.����Ƚ�����
	TIM_OCInitTypeDef TIM_OCInitStructure;//��������Ƚ����ýṹ��
	TIM_OCStructInit(&TIM_OCInitStructure);//��ʼ���Ƚ����ýṹ�壬��������������ʱ��
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//��������Ƚ�ģʽΪPWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//��Ч��ƽΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_Pulse=0;//���ñȽϼĴ���ֵΪ0�������ڳ����ж�̬�޸�
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//���ñȽ����ͨ��
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);//��ʼ���Ƚ��������
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//����TIMx��PWM�����Ϊʹ��
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//����TIMx��PWM�����Ϊʹ��
	
	ServoMid();
}

/*
	TIM_SetCompare4(TIM1,5);//ռ�ձ�Ϊ5/200������Ƕ�Ϊ0��
	TIM_SetCompare4(TIM1,15);//ռ�ձ�Ϊ15/200������Ƕ�Ϊ90��
	TIM_SetCompare4(TIM1,25);//ռ�ձ�Ϊ25/200������Ƕ�Ϊ180��
*/

//����45��
void ServoLeft(){
	TIM_SetCompare4(TIM1,23);
}

//��Ȩ��45��
void ServoRight(){
	TIM_SetCompare4(TIM1,7);
}

//����
void ServoMid(){
	TIM_SetCompare4(TIM1,15);
}

int BlockDetection(){
		float L1,L2,L3; 

		ServoMid();
		delay_ms(1000);//��ʱ1s
		L1=HC_SR04_GetLenght();//��λcm
		ServoLeft();
		delay_ms(1000);//��ʱ1s
		L2=HC_SR04_GetLenght();//��λcm
		ServoRight();
		delay_ms(1000);//�ӳ�1s
		L3=HC_SR04_GetLenght();//��λcm
		ServoMid();
		delay_ms(500);
		if(L1>50){
			return 1;//ֱ��.
		}else if(L2>50&&L3>50){
			return 3;//��ת
		}else if(L2<L3){
            return 0; // ����ϰ�,��ת
		} else {
            return 2; // �ұ��ϰ�,��ת
		}
   


}




