/*
 * @時基配置
 * @創建時間：T20201028
 * @作者：Viking
 * @版本號：V2.0
 * @注：
 */
#include "common.h"
volatile bit gbv_GAD_Time_1s = 0;
volatile bit gbv_GAD_OLEDDispayDebug_Time_1s = 0;
/*
 * @ 時基時間初始化1s
 * @
 */
void V_Timer_Init()
{
	TimeBase0_8ms_Init();
	TimeBase1_1s_Init();//
	TimeBase0_INT_On();//開啟中斷
	TimeBase1_INT_On();
	_tbck = 0;
	_tbon = 1;
}
//@----------Timebase0 中斷入口函數-----------@
DEFINE_ISR(TIMEBASE0_ISR, 0x01c)
{
	 // User Code
	 gbv_8ms_Using_Key_Scan = 1;
}
//@----------Timebase1 中斷入口函數-----------@
DEFINE_ISR(TIMEBASE1_ISR, 0x020)
{
    // User Code
	gbv_LVD_1s = 1;
    gbv_GAD_Time_1s = 1;  
    gbv_GAD_OLEDDispayDebug_Time_1s = 1;  
    gu8v_LEDTime ++;
	gu8v_TipsLED_Time ++;
	gu8v_HaltTime ++;
}

