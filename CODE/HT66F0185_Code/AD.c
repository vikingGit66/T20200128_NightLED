/*
 * @AD ��ӹ⏊
 * @�����r�g��T20201028
 * @���ߣ�Viking
 * @�汾̖��V2.0
 * @ע��
 */
#include "common.h"
#include <stdlib.h>





volatile unsigned int G_ADCData;
volatile int G_ADCDataLast;

//������B
volatile unsigned char gu8v_WorkMode;
//�⏊��B׃��
volatile unsigned char gu8v_GState;
//LED��B׃��
volatile unsigned char gu8v_LEDState;
//ָʾ����B׃��
volatile unsigned char gu8v_TipsLEDState;
volatile unsigned char gu8v_TipsLEDStateLast;
//���I��B׃��
volatile unsigned char gu8v_KeyState;
//ADC-On ����λ
volatile bit gbv_GADC_Is_On;
//LED ��Ӌ�r׃��
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

//�Ԅ�У�ʵĕr�g׃��
volatile unsigned char gu8v_LEDAutoCalTime;
volatile unsigned int G_AutoCalADCDataTemp = 0;
volatile bit gbv_AutoCal_GState;
volatile unsigned char gu8v_AutoCalState;

//T20210119
//LED-Is-AutoOffTime
volatile unsigned char  gu8v_NightLED_Is_AutoOff_Time1s;
//T20210128�޸��Д�GH��GL�Д�
void fun_AD_GL_OR_GH()
{
	if( (gbv_GAD_Time_1s && gbv_GADC_Is_On)
		|| gbv_NightLED_Is_AutoChange)
	{
		gbv_GAD_Time_1s = 0;
		G_ADCData = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//1s��10�P����
		if(G_ADCData > GHMin)//��L1L2���ГQ���ГQ������GH_State
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
		else if(G_ADCData < GLMax)//�����ГQ�����飺GL
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
		//����ǵ�Ӌ�r�Y����� 2 �εĔ�ֵ�Д���׃�����t��B�鱣�֣�gu8v_GState = G_State_Hold
		if(gbv_NightLED_Is_AutoChange || gbv_NightLED_Is_HandTurnOut)//Сҹ���P�]����������,AutoOff���O�Þ�1
		{
			gu8v_NightLED_Is_AutoOff_Time1s ++;
			if(gu8v_NightLED_Is_AutoOff_Time1s == 1)//�M��ɴΣ����룬���_�Ş�궨�x�r�g default:2
			{
				gu8v_NightLED_Is_AutoOff_Time1s = 0;
				gbv_NightLED_Is_AutoChange = 0;		//�Ԅ��P�]LED���Iλ
				gbv_NightLED_Is_HandTurnOut = 0;	//�ք��P�]LED���Iλ
			}
			gu8v_GState = G_State_Hold;	
		}
		G_ADCDataLast = G_ADCData;
	}
}



/**
 * @brief ����ADCͨ��������ȡADCֵ
 *
 * @param channel ADC����ͨ��
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
 * @brief ����ADCͨ����������ȡN��ADC��ȥ�������С��ȡƽ��ֵ
 *
 * @param channel ADC����ͨ��
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
 * @brief ����ADCͨ����������ȡN(N<=10)��ADC��ȡ���gֵ
 *
 * @param channel ADC����ͨ��
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
	//ð������
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