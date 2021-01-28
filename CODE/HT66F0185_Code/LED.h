
#ifndef _LED_H_
#define _LED_H_
#include "HT66F0185.h"

#define G_Time_Min_2 2
#define G_Time_Min_5 5
#define G_Time_Min_10 10
#define G_Time_Min_20 20
#define G_Time_Min_30 30
#define G_Time_Min_60 60
#define G_Time_Min_90 90
#define G_Time_Min_120 120

#define LED_1_8_I_Level0()	{ _pcps3 = 0; _pcps2 = 0; _pcps1 = 0; _pcps0 = 0;}
#define LED_1_8_I_Level1()	{ _pcps3 = 0; _pcps2 = 1; _pcps1 = 0; _pcps0 = 1;}
#define LED_1_8_I_Level2()	{ _pcps3 = 1; _pcps2 = 0; _pcps1 = 1; _pcps0 = 0;}
#define LED_1_8_I_Level3()	{ _pcps3 = 1; _pcps2 = 1; _pcps1 = 1; _pcps0 = 1;}
#define LED_1_3_Power_I_Level0()	{ _paps3 = 0; _paps2 = 0; _pbps1 = 0; _pbps0 = 0;}
#define LED_1_3_Power_I_Level1()	{ _paps3 = 1; _paps2 = 0; _pbps1 = 1; _pbps0 = 0;}
#define LED_1_3_Power_I_Level2()	{ _paps3 = 0; _paps2 = 1; _pbps1 = 0; _pbps0 = 1;}
#define LED_1_3_Power_I_Level3()	{ _paps3 = 1; _paps2 = 1; _pbps1 = 1; _pbps0 = 1;}

#define LED1_ON() { _pc = 0x01; _pa1 = 0;}
#define LED2_ON() { _pc = 0x02; _pa1 = 0;}
#define LED3_ON() { _pc = 0x04; _pa1 = 0;}
#define LED4_ON() { _pc = 0x08; _pa1 = 0;}
#define LED5_ON() { _pc = 0x10; _pa1 = 0;}
#define LED6_ON() { _pc = 0x20; _pa1 = 0;}
#define LED7_ON() { _pc = 0x40; _pa1 = 0;}
#define LED8_ON() { _pc = 0x00; _pa1 = 1;}

#define LED_ALL_OFF() {_pc = 0x00; _pa1 = 0;}
#define LED_ALL_ON()  {_pc = 0x7f; _pa1 = 1;}

#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7
#define LED8 8
#define LED_OFF 55
#define LED_ON 88

#define LED1_Power_ON() 	{_pa5 = 1; _pa7 = 0; _pb3 = 0;}
#define LED2_Power_ON() 	{_pa5 = 0; _pa7 = 1; _pb3 = 0;}
#define LED3_Power_ON() 	{_pa5 = 0; _pa7 = 0; _pb3 = 1;}
#define LED_Power_ALL_OFF() {_pa5 = 0; _pa7 = 0; _pb3 = 0;}
#define LED_Power_ALL_ON()  {_pa5 = 1; _pa7 = 1; _pb3 = 1;}

void V_LED1_8_Init();
void V_LED1_8_Work(u8 led_num);
void V_LED1_3_Power_Init();
void V_LED1_3_Power_Work(u8 led_num);

#endif
