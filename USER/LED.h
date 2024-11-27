#ifndef _LED_H
#define _LED_H

void LED_Init(void);
void setColor(uint8_t red, uint8_t green, uint8_t blue);
void colorCycle(uint16_t delay_time);
void testRed(void); 

#endif

