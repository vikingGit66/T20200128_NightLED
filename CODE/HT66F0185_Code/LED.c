/*
 * @LED 開關
 * @創建時間：T20201028
 * @作者：Viking
 * @版本號：V2.0
 * @注：
 */
#include "common.h"

/*
 * @ 輸出低
 * @
 */
void V_LED1_8_Init()
{
	LED_1_8_I_Level0();
	SET_PC0_LED1();
	SET_PC1_LED2();
	SET_PC2_LED3();
	SET_PC3_LED4();
	SET_PC4_LED5();
	SET_PC5_LED6();
	SET_PC6_LED7();
	SET_PA1_LED8();
}

/*
 * @ 打開
 * @
 */
void V_LED1_8_Work(u8 led_num)
{
	switch(led_num)
	{
		case 1: { LED1_ON(); break;}
		case 2: { LED2_ON(); break;}
		case 3: { LED3_ON(); break;}
		case 4: { LED4_ON(); break;}
		case 5: { LED5_ON(); break;}
		case 6: { LED6_ON(); break;}
		case 7: { LED7_ON(); break;}
		case 8: { LED8_ON(); break;}
		case 55:  { LED_ALL_OFF(); break;}
		case 88:  { LED_ALL_ON();  break;}
		default : { LED_ALL_OFF(); break;}
	}
}
/*
 * @ 輸出低
 * @
 */
void V_LED1_3_Power_Init()
{
	LED_1_3_Power_I_Level0();
	SET_PA5_LED1_POWER();
	SET_PA7_LED2_POWER();
	SET_PB3_LED3_POWER();
}
/*
 * @ 打開
 * @
 */
void V_LED1_3_Power_Work(u8 led_num)
{
	switch(led_num)
	{
		case 1: { LED1_Power_ON(); break;}
		case 2: { LED2_Power_ON(); break;}
		case 3: { LED3_Power_ON(); break;}
		case 55:  { LED_Power_ALL_OFF(); break;}
		case 88:  { LED_Power_ALL_ON();  break;}
		default : { LED_Power_ALL_OFF(); break;}
	}
}


