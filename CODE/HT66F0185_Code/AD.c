/*
 * @AD 採樣光強
 * @創建時間：T20201028
 * @作者：Viking
 * @版本號：V2.0
 * @注：
 */
#include "common.h"
#include <stdlib.h>





volatile unsigned int G_ADCData;
volatile int G_ADCDataLast;

//工作狀態
volatile unsigned char gu8v_WorkMode;
//光強狀態變量
volatile unsigned char gu8v_GState;
//LED狀態變量
volatile unsigned char gu8v_LEDState;
//指示燈狀態變量
volatile unsigned char gu8v_TipsLEDState;
volatile unsigned char gu8v_TipsLEDStateLast;
//按鍵狀態變量
volatile unsigned char gu8v_KeyState;
//ADC-On 控制位
volatile bit gbv_GADC_Is_On;
//LED 倒計時變量
volatile unsigned int gu8v_LEDTime;
//LED_Is_On
volatile bit gbv_LEDTime_Is_On;
//LED-Light
volatile unsigned char gu8v_LEDLight;
volatile unsigned char gu8v_LEDLight_Temp;
//LED-TimeOutSet
volatile unsigned int gu8v_LEDTimeOutSet;
//TipsLED-Time
volatile unsigned char gu8v_TipsLED_Time;
//Key_FirstOn
volatile bit gbv_Key_Is_NoFirst;
//LED-Is-AutoOff
volatile bit gbv_NightLED_Is_AutoChange;
//HaltTime
volatile unsigned char gu8v_HaltTime;
//Key0
volatile bit gbv_KeyIs0;
//Key1
volatile bit gbv_KeyIs1;
//LVD
volatile bit gbv_LVD_1s;
//LED-Is-HandTurnOut
volatile bit gbv_NightLED_Is_HandTurnOut;
//Tips_NightFindLED
volatile bit Tips_NightFindLED_State;
//G_IS_H_Or_L
volatile bit gbv_G_Is_H;
//GHMin
volatile unsigned int GHMin;
//GLMax
volatile unsigned int GLMax;
//GL1_Data,V3.0 unused
volatile unsigned int GL1_Data;
//GL2_Data,V3.0 unused
volatile unsigned int GL2_Data;
//GL1L2_Data,V3.0 unused
volatile unsigned int GL1L2_Data;
//LEDLight_Is_AutoChange
volatile bit gbv_LEDLight_Is_AutoChange;

//自動校準的時間變量
volatile unsigned char gu8v_LEDAutoCalTime;
volatile unsigned int G_AutoCalADCDataTemp = 0;
volatile bit gbv_AutoCal_GState;
volatile unsigned char gu8v_AutoCalState;

//T20210119
//LED-Is-AutoOffTime
volatile unsigned char  gu8v_NightLED_Is_AutoOff_Time1s;
//T20210128修改判斷GH，GL判斷
void fun_AD_GL_OR_GH()
{
	if( (gbv_GAD_Time_1s && gbv_GADC_Is_On)
		|| gbv_NightLED_Is_AutoChange)
	{
		gbv_GAD_Time_1s = 0;
		G_ADCData = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//1s採集10筆數據
		if(G_ADCData > GHMin)//由L1L2暗切換到切換亮，為GH_State
		{
			gbv_G_Is_H = 1;
			if(G_ADCDataLast <= GL1L2_Data)
			//if(G_ADCDataLast <= (G_ADCData - 300))
			{
				gu8v_GState = G_State_H;
			}
			else
			{
				gu8v_GState = G_State_Hold;
			}
			
		}
		else if(G_ADCData < GLMax)//由亮切換到暗為：GL
		{
			gbv_G_Is_H = 0;
			if(G_ADCDataLast >= GHMin)
			//if(G_ADCDataLast >= (G_ADCData + 300))
			{
				gu8v_GState = G_State_L;
			}
			else
			{
				gu8v_GState = G_State_Hold;
			}
			//
		}
		//如果是倒計時結束后的 2 次的數值判斷為變暗，則狀態為保持：gu8v_GState = G_State_Hold
		if(gbv_NightLED_Is_AutoChange || gbv_NightLED_Is_HandTurnOut)//小夜燈關閉的由亮到暗,AutoOff后設置為1
		{
			gu8v_NightLED_Is_AutoOff_Time1s ++;
			if(gu8v_NightLED_Is_AutoOff_Time1s == 1)//進來兩次，兩秒，可開放為宏定義時間 default:2
			{
				gu8v_NightLED_Is_AutoOff_Time1s = 0;
				gbv_NightLED_Is_AutoChange = 0;		//自動關閉LED標誌位
				gbv_NightLED_Is_HandTurnOut = 0;	//手動關閉LED標誌位
			}
			gu8v_GState = G_State_Hold;	
		}
		G_ADCDataLast = G_ADCData;
	}
}



/**
 * @brief 开启ADC通道，并获取ADC值
 *
 * @param channel ADC采样通道
 * @return unsigned int
 */
unsigned int Drv_GetADC(unsigned char channel)
{
	
	SET_ADC_DATA_0_11();
	SET_ADC_POWER_ON();
	SET_PA6_VREF_VDD();
	SET_ADC_CLOCK_SYS_8();
	switch (channel)
	{
		case ADC_CHANNEL_AN0:	SET_ADC_CHANNEL_AN0(); SET_PB0_AN0();	break;
		case ADC_CHANNEL_AN1:	SET_ADC_CHANNEL_AN1(); SET_PB1_AN1();	break;
		case ADC_CHANNEL_AN2:	SET_ADC_CHANNEL_AN2(); SET_PB2_AN2();	break;
		case ADC_CHANNEL_AN3:	SET_ADC_CHANNEL_AN3(); SET_PA4_AN3();	break;
		case ADC_CHANNEL_AN4:	SET_ADC_CHANNEL_AN4(); SET_PA5_AN4();	break;
		case ADC_CHANNEL_AN5:	SET_ADC_CHANNEL_AN5(); SET_PA6_AN5();	break;
		case ADC_CHANNEL_AN6:	SET_ADC_CHANNEL_AN6(); SET_PA7_AN6();	break;
		case ADC_CHANNEL_AN7:	SET_ADC_CHANNEL_AN7(); SET_PB3_AN7();	break;
		 
		default:	break;
	}
	SET_ADC_STARTCONVERT();
	while (_adbz);
	unsigned int temp;
	temp = _sadol;
	temp += _sadoh*256;
	
	SET_ADC_POWER_OFF();
	CLR_PA6_VREF_VDD();
	
	return temp;
}



/**
 * @brief 开启ADC通道，连续获取N笔ADC，去除最大最小后取平均值
 *
 * @param channel ADC采样通道
 * @return unsigned int
 */
unsigned int Drv_GetADC_AVGn(unsigned char channel, unsigned char Cnt)
{
	SET_ADC_DATA_0_11();
	SET_ADC_POWER_ON();
	SET_PA6_VREF_VDD();
	SET_ADC_CLOCK_SYS_8();
	switch (channel)
	{
		case ADC_CHANNEL_AN0:	SET_ADC_CHANNEL_AN0(); SET_PB0_AN0();	break;
		case ADC_CHANNEL_AN1:	SET_ADC_CHANNEL_AN1(); SET_PB1_AN1();	break;
		case ADC_CHANNEL_AN2:	SET_ADC_CHANNEL_AN2(); SET_PB2_AN2();	break;
		case ADC_CHANNEL_AN3:	SET_ADC_CHANNEL_AN3(); SET_PA4_AN3();	break;
		case ADC_CHANNEL_AN4:	SET_ADC_CHANNEL_AN4(); SET_PA5_AN4();	break;
		case ADC_CHANNEL_AN5:	SET_ADC_CHANNEL_AN5(); SET_PA6_AN5();	break;
		case ADC_CHANNEL_AN6:	SET_ADC_CHANNEL_AN6(); SET_PA7_AN6();	break;
		case ADC_CHANNEL_AN7:	SET_ADC_CHANNEL_AN7(); SET_PB3_AN7();	break;
		default:	break;
	}

	unsigned char i;
	unsigned int AdcData;
	unsigned long AdcData_Sum = 0;
	unsigned int ADCData_Max = 0x00;
	unsigned int ADCData_Min = 0xffff;
	GCC_DELAY(100);
	for ( i = 0; i < Cnt; i++)
	{
		SET_ADC_STARTCONVERT();
		while (_adbz);
		AdcData = _sadol;
		AdcData += _sadoh*256;
		AdcData_Sum += AdcData;
		if (AdcData > ADCData_Max)
		{
			ADCData_Max = AdcData;
		}
		if (AdcData < ADCData_Min)
		{
			ADCData_Min = AdcData;
		}
	}
	AdcData = (AdcData_Sum - ADCData_Max - ADCData_Min) / (Cnt - 2);
	SET_ADC_POWER_OFF();
	return AdcData;
}

/**
 * @brief 开启ADC通道，连续获取N(N<=10)笔ADC，取中間值
 *
 * @param channel ADC采样通道
 * @return unsigned int
 */
unsigned int Drv_GetADC_TEMPn(unsigned char channel, unsigned char Cnt)
{
	SET_ADC_DATA_0_11();
	SET_ADC_POWER_ON();
	SET_PA6_VREF_VDD();
	SET_ADC_CLOCK_SYS_8();
	switch (channel)
	{
		case ADC_CHANNEL_AN0:	SET_ADC_CHANNEL_AN0(); SET_PB0_AN0();	break;
		case ADC_CHANNEL_AN1:	SET_ADC_CHANNEL_AN1(); SET_PB1_AN1();	break;
		case ADC_CHANNEL_AN2:	SET_ADC_CHANNEL_AN2(); SET_PB2_AN2();	break;
		case ADC_CHANNEL_AN3:	SET_ADC_CHANNEL_AN3(); SET_PA4_AN3();	break;
		case ADC_CHANNEL_AN4:	SET_ADC_CHANNEL_AN4(); SET_PA5_AN4();	break;
		case ADC_CHANNEL_AN5:	SET_ADC_CHANNEL_AN5(); SET_PA6_AN5();	break;
		case ADC_CHANNEL_AN6:	SET_ADC_CHANNEL_AN6(); SET_PA7_AN6();	break;
		case ADC_CHANNEL_AN7:	SET_ADC_CHANNEL_AN7(); SET_PB3_AN7();	break;
		default:	break;
	}

	unsigned char i,j;
	unsigned int AdcData[10];
	unsigned int AdcDataTemp;
	GCC_DELAY(100);
	for ( i = 0; i < Cnt; i++)
	{
		SET_ADC_STARTCONVERT();
		while (_adbz);
		AdcData[i] = _sadol;
		AdcData[i] += _sadoh*256;
	}
	//冒泡排序
	for(j=0;j<(Cnt-1);j++)
	{
		for(i=0;i<(Cnt-1-j);i++)
		{
			if( AdcData[i] > AdcData[i+1])
			{
				AdcDataTemp = AdcData[i];
				AdcData[i] = AdcData[i+1];
				AdcData[i+1] = AdcDataTemp;
			}
		}
	}
	if(Cnt % 2 == 0)
	{
		AdcDataTemp =  AdcData[i/2];
	}
	else
	{
		AdcDataTemp = (AdcData[i/2] + AdcData[i/2 + 1]) / 2;
	}
	SET_ADC_POWER_OFF();
	return AdcDataTemp;
}