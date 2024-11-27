#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

#include "LED.h"

const uint8_t rainbow_colors[][3] = {
    {1, 0, 0},  
    {1, 1, 0},  
    {0, 1, 0},  
    {0, 1, 1},  
    {0, 0, 1},  
    {1, 0, 1},  
    {1, 0, 0}   
};

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    if(red)   GPIO_SetBits(GPIOB, GPIO_Pin_1);   
    else      GPIO_ResetBits(GPIOB, GPIO_Pin_1);

    if(green) GPIO_SetBits(GPIOB, GPIO_Pin_0);   
    else      GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    if(blue)  GPIO_SetBits(GPIOA, GPIO_Pin_7);   
    else      GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}



void colorCycle(uint16_t delay_time)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        setColor(rainbow_colors[i][0], rainbow_colors[i][1], rainbow_colors[i][2]);
        delay_ms(delay_time);
    }
}

void testRed(void)
{
    while(1)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);   
        delay_ms(1000);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1); 
        delay_ms(1000);
    }
}


/* 
int main(void)
{
    uint8_t fanState = 0;
    uint32_t fanTimer = 0;
    GPIO_Configuration();
    
    delay_init();

    while (1)
    {
        colorCycle(1000); 

        fanState = !fanState;
        setFan(fanState ? ENABLE : DISABLE);

        if (fanTimer >= 300)
        {
            fanTimer = 0;
            fanState = !fanState;
            setFan(fanState ? ENABLE : DISABLE);
        }
        fanTimer++;
        delay_ms(10); 
    }
}
*/
