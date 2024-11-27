#include "Line.h"
#include "Wheel.h"
#include "Servo.h"
void LineInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		//巡线传感器初始化
//左1PC14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//左2PC13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//右1PC15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//右2PB12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

LineStatus* Line_Detection()
{	
static LineStatus lineStatus; // 使用静态变量以保持其值

    lineStatus.L1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == Bit_SET ? 0 : 1;
    lineStatus.L2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET ? 0 : 1;
    lineStatus.R2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == Bit_SET ? 0 : 1;
    lineStatus.R1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET ? 0 : 1;

    return &lineStatus; // 返回指向静态变量的指针
}

void ActionFA(){

	LineStatus* status=Line_Detection();
	
//0000
	if(!status->L1&&!status->L2&&!status->R2&&!status->R1){
		high_speed_cnt=0;
		baseSpeed=1000;
		err = lastErr/4.0;
		zeroCnt++;
		if(zeroCnt>=30){
			Car_Stop();
			zeroCnt=30;
			while(!Line_Detection()->L1&&!Line_Detection()->L2&&!Line_Detection()->R2&&!Line_Detection()->R1);
		}

	}
//1000
	else if(status->L1&&!status->L2&&!status->R2&&!status->R1){
		zeroCnt=0;
		err = 5;
		high_speed_cnt=0;
		baseSpeed=500;
		if((!lastStatus->L1&&lastStatus->L2&&lastStatus->R2&&!lastStatus->R1) ||
			(!lastStatus->L1&&!lastStatus->L2&&!lastStatus->R2&&!lastStatus->R1))
		{	
			err=6;
			baseSpeed=200;
		}
		if(I*I>100){
			baseSpeed=50;
		}
		lastErr=err;
	}
//0001
	else if(!status->L1&&!status->L2&&!status->R2&&status->R1){
		zeroCnt=0;
		err = -5;
		high_speed_cnt=0;
		baseSpeed=500;
		if((!lastStatus->L1&&lastStatus->L2&&lastStatus->R2&&!lastStatus->R1) ||
			(!lastStatus->L1&&!lastStatus->L2&&!lastStatus->R2&&!lastStatus->R1))
		{	
			baseSpeed=200;
			err=-6;
		}
		if(I*I>100){
			baseSpeed=50;
		}

		lastErr=err;

	}
		//1100
	else if(status->L1&&status->L2&&!status->R2&&!status->R1){
		zeroCnt=0;
		Car_Stop();
		high_speed_cnt=0;
		baseSpeed=500;
		if(I*I>100){
			baseSpeed=0;
		}
		 err = 8;
	}
	//0011
	else if(!status->L1&&!status->L2&&status->R2&&status->R1){
		zeroCnt=0;
		Car_Stop();
		high_speed_cnt=0;

		baseSpeed=500;
		if(I*I>100){
			baseSpeed=0;
		}
		 err = -8;

	}
	
	//0100
	else if(!status->L1&&status->L2&&!status->R2&&!status->R1){
		zeroCnt=0;
		Car_Stop();
		high_speed_cnt=0;
		baseSpeed=500;
		if(I*I>100){
			baseSpeed=200;
		}
	 err = 3.5
		;
		if(lastStatus->L1&&!lastStatus->L2&&!lastStatus->R2&&!lastStatus->R1)
		{
			err=5;
		}
		
	}
	//0010
	else if(!status->L1&&!status->L2&&status->R2&&!status->R1){
		zeroCnt=0;
		Car_Stop();
		high_speed_cnt=0;
		baseSpeed=500;
		if(I*I>100){
			baseSpeed=200;
		}
		 err = -3.5;
		if(!lastStatus->L1&&!lastStatus->L2&&!lastStatus->R2&&lastStatus->R1)
		{
			err=-5;
		}
	}
	//1111
	else if (status->L1&&status->L2&&status->R2&&status->R1){
		zeroCnt=0;
		high_speed_cnt=0;
		delay_ms(200);
		Car_Stop();
		delay_ms(100);
		if(!Line_Detection()->L1&&!Line_Detection()->L2&&!Line_Detection()->R2&&!Line_Detection()->R1){
			return;
		}
		delay_ms(200);
		int direction=BlockDetection();
		
		
		switch(direction){
			case 1:baseSpeed=1500;break;
			case 0:	delay_ms(300);
							Car_SpinRight(3000,2000);
							delay_ms(300);
							while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1)){};break;
			case 2:	delay_ms(300);
							Car_SpinLeft(2000,3000);
							delay_ms(300);
							while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1)){};break;
			case 3:delay_ms(300);
							Car_SpinRight(2500,2500);
							delay_ms(400);
							while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1)){};break;
		}
		
		I=0;

	}
	
	//特殊
	
	//0111直角右转
	else if (!status->L1&&status->L2&&status->R2&&status->R1){
		zeroCnt=0;
		Car_Stop();
		if(!status->L1&&status->L2&&status->R2&&status->R1){
			delay_ms(PREDELAY*2);
			Car_Stop();
			delay_ms(200);
			if (!(!Line_Detection()->L1&&!Line_Detection()->L2&&!Line_Detection()->R2&&!Line_Detection()->R1)){
				return;
			}
			Car_SpinRight(3000,1500);
			while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			baseSpeed=300;
		}
		
	}
	/*
	//0111锐角特供
		else if (!status->L1&&status->L2&&status->R2&&status->R1){
		zeroCnt=0;
		Car_Stop();
		if(!status->L1&&status->L2&&status->R2&&status->R1){

			Car_Stop();
			delay_ms(200);
			Car_Left_Indie(3000);
			Car_Right_Indie(-3000);
			while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			baseSpeed=300;
		}
		
	}
		//1110锐角特供
		else if (status->L1&&status->L2&&status->R2&&!status->R1){
		zeroCnt=0;
		Car_Stop();
		if(status->L1&&status->L2&&status->R2&&!status->R1){

			Car_Stop();
			delay_ms(200);
			Car_Left_Indie(-3000);
			Car_Right_Indie(3000);
			while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
			baseSpeed=300;
		}
		
	}
		*/
	//1110直角左转
	else if (status->L1&&status->L2&&status->R2&&!status->R1){
		zeroCnt=0;
		Car_Stop();
		high_speed_cnt=0;
		if(status->L1&&status->L2&&status->R2&&!status->R1){
		delay_ms(PREDELAY*2);
		Car_Stop();
		delay_ms(200);
		if (!(!Line_Detection()->L1&&!Line_Detection()->L2&&!Line_Detection()->R2&&!Line_Detection()->R1)){
			return;
		}
		Car_SpinLeft(1500,3000);
		while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
		baseSpeed=300;
		}
	}
	
		//1010 1011锐角左转
	else if ((status->L1&&!status->L2&&status->R2&&status->R1)||(status->L1&&!status->L2&&status->R2&&!status->R1)){
		zeroCnt=0;
		baseSpeed=200;
		high_speed_cnt=0;
		delay_ms(30);
		if((status->L1&&!status->L2&&status->R2&&status->R1)||(status->L1&&!status->L2&&status->R2&&!status->R1)){
		delay_ms(PREDELAY/3);
		if (!(status->L1&&!status->L2&&status->R2&&status->R1)||(status->L1&&!status->L2&&status->R2&&!status->R1)){
			return;
		}
		Car_Stop();
		delay_ms(300);

		Car_SpinLeft(3000,3000);
		while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
		baseSpeed=300;
		}
	}
			//0101 1101锐角右转
	else if ((!status->L1&&status->L2&&!status->R2&&status->R1)||(status->L1&&status->L2&&!status->R2&&!status->R1)){
		zeroCnt=0;
		baseSpeed=200;
		high_speed_cnt=0;
		delay_ms(15);
		if((!status->L1&&status->L2&&!status->R2&&status->R1)||(status->L1&&status->L2&&!status->R2&&!status->R1)){
		delay_ms(PREDELAY/3);
		if (!((!status->L1&&status->L2&&!status->R2&&status->R1)||(status->L1&&status->L2&&!status->R2&&!status->R1))){
			return;
		}
		Car_Stop();
		delay_ms(300);

		Car_SpinRight(3000,3000);
		while(!(!Line_Detection()->L1&&Line_Detection()->L2&&Line_Detection()->R2&&!Line_Detection()->R1));
		baseSpeed=300;
		}
	}
	
	//正确
	//0110
	else if (!status->L1&&status->L2&&status->R2&&!status->R1){
		zeroCnt=0;
		I=0;
		high_speed_cnt++;
		if(high_speed_cnt<=60){
		baseSpeed=1400;
		}
		else
		if(high_speed_cnt>60){
			baseSpeed=1600				;
		}
		err=0;
	}
		
		lastStatus=status;//记录上一状态
		

		leftMotorSpeed = baseSpeed - CacuPwm();
		rightMotorSpeed = baseSpeed + CacuPwm();
	leftMotorSpeed=leftMotorSpeed<=-1000?-1000:leftMotorSpeed+(leftMotorSpeed<0?-100:100);
		rightMotorSpeed =rightMotorSpeed<=-1000?-1000:rightMotorSpeed;
		
		
	
		leftMotorSpeed=leftMotorSpeed>=2500?2500:leftMotorSpeed+(leftMotorSpeed<0?-100:100);;
		rightMotorSpeed =rightMotorSpeed>=2500?2500:rightMotorSpeed;
	

}
int  CacuPwm()            //pid运算部分
{		
    int output;
		float kp=120,ki=19,kd=18;
		//if(baseSpeed>1600){
		//float kp=150,ki=23,kd=100;
		//}
		//pid需要根据实际情况进行调节
    float P,D;
    P = err;
    I += err;
    D = err - last_err;
    output = kp*P + ki*I + kd*D; 
    last_err = err;
	

    return output;
	

	
}

void CarLineRun(){

	//
	if(Line_Detection()!=lastStatus){ActionFA();}
	Car_Left_Indie(leftMotorSpeed);
	Car_Right_Indie(rightMotorSpeed);
	
}


