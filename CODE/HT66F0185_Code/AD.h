

#ifndef _AD_H_
#define _AD_H_

//工作狀態變量
extern volatile unsigned char gu8v_WorkMode;
//光強狀態變量
extern volatile unsigned char gu8v_GState;
//LED狀態變量
extern volatile unsigned char gu8v_LEDState;
//指示燈狀態變量
extern volatile unsigned char gu8v_TipsLEDState;
extern volatile unsigned char gu8v_TipsLEDStateLast;
//按鍵狀態變量
extern volatile unsigned char gu8v_KeyState;
//ADC-On 控制位
extern volatile bit gbv_GADC_Is_On;
//LED 倒計時變量
extern volatile unsigned int gu8v_LEDTime;
//LED_Is_On
extern volatile bit gbv_LEDTime_Is_On;
//LED-Light
extern volatile unsigned char gu8v_LEDLight;
//LED-TimeOutSet
extern volatile unsigned int gu8v_LEDTimeOutSet;
//TipsLED-Time
extern volatile unsigned char gu8v_TipsLED_Time;
//Key_FirstOn
extern volatile bit gbv_Key_Is_NoFirst;
//LED-Is-AutoOff
extern volatile bit gbv_NightLED_Is_AutoChange;
//HaltTime
extern volatile unsigned char gu8v_HaltTime;
//Key0
extern volatile bit gbv_KeyIs0;
//Key1
extern volatile bit gbv_KeyIs1;
//LVD
extern volatile bit gbv_LVD_1s;
//LED-Is-HandTurnOut
extern volatile bit gbv_NightLED_Is_HandTurnOut;
//Tips_NightFindLED
extern volatile bit Tips_NightFindLED_State;
//G_IS_H_Or_L
extern volatile bit gbv_G_Is_H;
//GHMin
extern volatile unsigned int GHMin;
//GLMax
extern volatile unsigned int GLMax;
//GL1_Data,V3.0 unused
extern volatile unsigned int GL1_Data;
//GL2_Data,V3.0 unused
extern volatile unsigned int GL2_Data;
//GL1L2_Data,V3.0 unused
extern volatile unsigned int GL1L2_Data;


//自動校準的時間變量
extern volatile unsigned char gu8v_LEDAutoCalTime;
extern volatile unsigned int G_AutoCalADCDataTemp;
extern volatile bit gbv_AutoCal_GState;
extern volatile unsigned char gu8v_AutoCalState;




//#define TipsLED_OneLED
#define TipsLED_OneToTwoLED_10min//LED指示燈10分鐘一個等級

#define LED_IO 		        _pc
#define LED_IOC		        _pcc
#define LED_StateTips_IOC   _pac1
#define LED_StateTips_IO    _pa1
#define LED_NightFind_IOC   _pac7
#define LED_NightFind_IO    _pa7
#define LEDPower1 		_pb4
#define LEDPower1_IOC 	_pbc4
#define LEDPower2 		_pb3
#define LEDPower2_IOC 	_pbc3
#define Out         0
#define In          1
#define On 			1
#define Off 		0
#define gu8v_TipsLEDTimeOutSet 5 //5s的指示燈

//工作狀態枚舉
#define WorkMode_None            0
#define WorkMode_Auto            1  //自動小夜燈
#define WorkMode_KeyControl1_2   2  //亮度1 && 亮度2
#define WorkMode_NightLEDAutoCal 3  //自動校準模式

//光強狀態枚舉
#define G_State_None 0
#define G_State_H  	 1
#define G_State_L  	 2
#define G_State_Hold 3
//LED狀態枚舉
#define LED_State_None 	0
#define LED_State_On   	1
#define LED_State_Off	2
//LED亮度
#define LEDLight_Off	0
#define LEDLight_N1		1
#define LEDLight_N2		2
#define LEDLight_N3		3
#define LEDLightNum     3

//指示燈狀態枚舉
#define TipsLED_State_None	0
#define TipsLED_State_L1	1	
#define TipsLED_State_L2	2
#define TipsLED_State_L3	3
#define TipsLED_State_L4	4
#define TipsLED_State_L5	5
#define TipsLED_State_L6	6
#define TipsLED_State_L7	7
//#define TipsLED_State_L8	8
#define TipsLED_LNum        7
#define Tips_NightFindLED_Off   0 //新變量
#define Tips_NightFindLED_On    1 //新變量

//按鍵狀態枚舉
#define Key_State_None			0
#define Key_State_ShortPress	1
#define Key_State_LongPress		2

#define gu8v_HaltTimeSet5s 5
#define gu8v_HaltTimeSet10s 10
#define gu8v_HaltTimeSet20s 20
#define gu8v_HaltTimeSet30s 30
#define gu8v_HaltTimeSet60s 60
#define gu8v_HaltTimeSet90s 90
#define gu8v_HaltTimeSet120s 120


//自動校準狀態
#define AutoCalState_Is_Finding_H       0
#define AutoCalState_Is_Finded_H        1
#define AutoCalState_Is_Finding_L       2
#define AutoCalState_Is_Finded_L        3
#define AutoCalState_Is_Setting_L1      4
#define AutoCalState_Is_Setting_L2      5
#define AutoCalState_Is_Setting_L1L2    6
#define AutoCalState_Is_LOK             7
#define AutoCalState_Is_OK              8
#define AutoCalState_Is_Failed          9
#define NightAutoCal_GState_Is_L    0
#define NightAutoCal_GState_Is_H    1



#define gu8V_GAD_GHIncData 	200 //default:800
#define gu8V_GAD_GLDecData 	200	//default:800
#define GLMaxDefault		1000//暗條件的最大值，小於最大值都為暗;default:1000
#define GHMinDefault		2500//亮條件的最小值，大於最小值都為亮;default:2500
#define gu8V_GAD_Cnt	  	10	//AD採樣次數
#define gu8V_GAD_channel 	ADC_CHANNEL_AN2 //AD通道


#define SET_ADC_DATA_0_11() { _adrfs = 1;}
#define SET_ADC_POWER_ON()	{ _adcen = 1;}
#define SET_ADC_POWER_OFF()	{ _adcen = 0;}
#define SET_ADC_STARTCONVERT()		{ _start = 0; _start = 1; _start = 0;}   //!< 開始ADC轉換

#define ADC_CHANNEL_AN0	0
#define ADC_CHANNEL_AN1	1
#define ADC_CHANNEL_AN2	2
#define ADC_CHANNEL_AN3	3
#define ADC_CHANNEL_AN4	4
#define ADC_CHANNEL_AN5	5
#define ADC_CHANNEL_AN6	6
#define ADC_CHANNEL_AN7	7

#define SET_ADC_CHANNEL_AN0()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x00;}
#define SET_ADC_CHANNEL_AN1()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x01;}
#define SET_ADC_CHANNEL_AN2()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x02;}
#define SET_ADC_CHANNEL_AN3()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x03;}
#define SET_ADC_CHANNEL_AN4()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x04;}
#define SET_ADC_CHANNEL_AN5()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x05;}
#define SET_ADC_CHANNEL_AN6()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x06;}
#define SET_ADC_CHANNEL_AN7()	{ _sadc1 &= 0x1F; _sadc0 &= 0xF0; _sadc0 |= 0x07;}

#define SET_ADC_CLOCK_SYS_1()	{ _sacks2 = 0; _sacks1 = 0; _sacks0 = 0;}
#define SET_ADC_CLOCK_SYS_2()	{ _sacks2 = 0; _sacks1 = 0; _sacks0 = 1;}
#define SET_ADC_CLOCK_SYS_4()	{ _sacks2 = 0; _sacks1 = 1; _sacks0 = 0;}
#define SET_ADC_CLOCK_SYS_8()	{ _sacks2 = 0; _sacks1 = 1; _sacks0 = 1;}

void fun_AD_GL_OR_GH();
unsigned int Drv_GetADC(unsigned char channel);
unsigned int Drv_GetADC_AVGn(unsigned char channel, unsigned char Cnt);
unsigned int Drv_GetADC_TEMPn(unsigned char channel, unsigned char Cnt);

#endif



