#include "HT66F0185.h"
#include "oled.h"
#include "common.h"

extern const unsigned char LOGO_HOLTEK[];

#define WDT_Disable_8ms() 	 { _wdtc = 0xa8;}
#define WDT_Disable_32ms()   { _wdtc = 0xa9;}
#define WDT_Disable_128ms()  { _wdtc = 0xaa;}
#define WDT_Disable_512ms()  { _wdtc = 0xab;}
#define WDT_Disable_1024ms() { _wdtc = 0xac;}
#define WDT_Disable_2048ms() { _wdtc = 0xad;}
#define WDT_Disable_4096ms() { _wdtc = 0xae;}
#define WDT_Disable_8192ms() { _wdtc = 0xaf;}
//@-------------LVD config--------------@
#define SET_LVD_DISABLE()       { _lvden = 0; _vbgen = 0;}
#define SET_LVD_LVDIN_EN()      { _lvdc = 0x18;}
#define SET_LVD_2V2_EN()        { _lvdc = 0x19;}
#define SET_LVD_2V4_EN()        { _lvdc = 0x1A;}
#define SET_LVD_2V7_EN()        { _lvdc = 0x1B;}
#define SET_LVD_3V0_EN()        { _lvdc = 0x1C;}
#define SET_LVD_3V3_EN()        { _lvdc = 0x1D;}
#define SET_LVD_3V6_EN()        { _lvdc = 0x1E;}
#define SET_LVD_4V0_EN()        { _lvdc = 0x1F;}

//#define DemoDebug

#ifdef DemoDebug
void fun_OLEDDisplayDebug();
#endif
void Delay_ms(u16 time);

void main()
 {
	// 判斷是否為上電復位或者非正常情況下的復位
	// 如果是上電復位，執行上電復位初始化，反之執行WDT溢出初始化
	if (_to == 0 || _pdf ==0)
	{
		// System
		WDT_Disable_512ms();
		V_GPIOPowerOnInit();
		fun_KeyInit();
		V_RamInit();
		V_Timer_Init();
		
		V_EEPRAM_Start();//gu8v_LEDLight = LEDLight_N1;//gu8v_TipsLEDState = TipsLED_State_L1;
		gu8v_WorkMode = WorkMode_Auto;
		gu8v_LEDState = LED_State_Off;
		
		//LED_Tips1_7_INIT
		LED_IOC = Out;//OUT
		LED_IO  = 0x00;//Off
		//LED_StateTips
		LED_StateTips_IOC = Out;//手動模式指示燈
		LED_StateTips_IO  = Off;
		//LEDPower
		LEDPower1_IOC = Out;
		LEDPower1  	  = Off;
		LEDPower2_IOC = Out;
		LEDPower2     = Off;
		//暗時找燈指示燈
		LED_NightFind_IOC = Out;
		LED_NightFind_IO  = Off;
		Tips_NightFindLED_State = Tips_NightFindLED_On;

		#ifdef DemoDebug
		//OLED-Debug
		oled_init();
		oled_showphoto(0, 0,128,8,LOGO_HOLTEK);
		Delay_ms(2000);
		oled_clear();
		oled_shownum(0,0,0,1,12);
		oled_shownum(0,1,1,1,12);
		oled_shownum(0,2,2,1,12);
		oled_shownum(0,3,3,1,12);
		oled_shownum(0,4,4,1,12);
		oled_shownum(0,5,5,1,12);
		oled_shownum(0,6,6,1,12);
		oled_shownum(0,7,7,1,12);
		oled_showstring(60,5,"SDA_PD1",12);
		oled_showstring(60,6,"SCL_PD2",12);
		oled_showstring(60,7,"AN2-G-1s",12);
		#endif
	}
	else
	{
		// WDT溢出復位初始化
		GCC_CLRWDT();
		
	}
		
	//主循環
	while(1)
	{
		GCC_CLRWDT();
		fun_KeyScan();
		fun_AD_GL_OR_GH();
		
		//LVD
		/*if(gbv_LVD_1s)
		{
			gbv_LVD_1s = 0;
			gu16v_LVD_1H ++;
			if(gu16v_LVD_1H == 3600)
			{
				gu16v_LVD_1H = 0;

				SET_LVD_3V3_EN();
				GCC_DELAY(2000);//8MHz:1ms	
				if (_lvdo)//gbv_Islvd = 1;
				{
					//Beep響函數：_pa5
					//Beep_On();
				}
				else//gbv_Islvd = 0;
				{
					
				}
				SET_LVD_DISABLE();
			}
		}*/
		

		//休眠控制
		//當休眠時間大於default值時，進入休眠，此時由看門狗1s喚醒，后進入AD判斷光亮度，休眠時間保持為default值
		//若此有按鍵按下則，休眠時間置位為0，在default時間內，打開TimeBase0，可實現按鍵的掃描
		if(gu8v_HaltTime >= gu8v_HaltTimeSet30s)//default:30s
		{
			//Power Down
			TimeBase0_INT_Off();
			GCC_CLRWDT();
			GCC_HALT();
			TimeBase0_INT_On();
			if(gbv_KeyIs0)
			{
				gbv_KeyIs0 = 0;
				gu8v_HaltTime = 0;
			}
			else
			{
				gu8v_HaltTime = gu8v_HaltTimeSet30s;//default:30s
			}
		}
		
		//LED顯示界面UI設計
		//LED顯示資源：8+1 LED 
		//時間等級：1-7：燈亮的個數代表時間的等級；紅、綠、黃、藍、紅、綠、黃
		//夜間尋找燈 8 ：通過隱藏按鍵開啟和關閉（節省功耗):2s內連續按下3次KEY2，則切換狀態，在黑的情況下執行
		//手動自動模 9 ：式提示燈：當按下燈后，若為手動模式打開的燈，則提示為手動模式，需要手動關閉
		switch (gu8v_WorkMode)
		{
			case WorkMode_Auto:
				gbv_GADC_Is_On = 1;//打開ADC
				LED_StateTips_IO = Off;
				if(gu8v_KeyState == Key_State_LongPress)
				{
					gu8v_KeyState = Key_State_None;
					gu8v_WorkMode = WorkMode_KeyControl1_2;
					gu8v_LEDState = LED_State_Off;			//狀態切換，關燈
				}
				if(gu8v_KeyState == Key_State_ShortPress)	//狀態短按，設置自動時間
				{
					gu8v_KeyState = Key_State_None;
					gu8v_TipsLED_Time = 0;					//LED提示燈復位0
					
					//如果不是首次按下，則自動時間++，寫入EEPRAM
					if(gbv_Key_Is_NoFirst)//5s后更新為首次按下
					{
						gu8v_TipsLEDState ++;
						V_EEPRAM_Write_Byte(EEPRAM_Addr_TipsLEDFlag, EEPRAM_TipsLEDFlag);
						V_EEPRAM_Write_Byte(EEPRAM_Addr_TipsLED, gu8v_TipsLEDState);
						gu8v_LEDTime = 0;
						if(gu8v_TipsLEDState > TipsLED_LNum)
						{
							gu8v_TipsLEDState = TipsLED_State_L1;
						}
						gu8v_LEDState = LED_State_Off;
					}
					else//首次按下若燈關則開
					{
						gu8v_TipsLEDStateLast = 0x55;//強制更改，以顯示當前時間狀態
						gbv_Key_Is_NoFirst = 1;

						gbv_NightLED_Is_HandTurnOut = 1;
						//第一次按下則燈取反，若為開，則倒計時關閉
						if(gu8v_LEDState == LED_State_Off)
						{
							gu8v_LEDState = LED_State_On;//開燈
							gu8v_LEDTime = 0;//LED倒計時的時間
							gbv_LEDTime_Is_On = 1;//打開倒計時
						}
						else if(gu8v_LEDState == LED_State_On)
						{
							gu8v_LEDState = LED_State_Off;
						}
					}			
				}
				//光強變化進入相對應的倒計時，主要是控制LED
				switch(gu8v_GState)//光強狀態變量
				{
					case G_State_H://由暗變亮：關燈
						gu8v_LEDState = LED_State_Off;
						gu8v_LEDTime = 0;//LED倒計時的時間
						gbv_LEDTime_Is_On = 0;//關閉倒計時
						break;
					case G_State_L://由亮變暗：開燈
						if(gu8v_LEDTimeOutSet != 0)//防止軟件出錯
						{
							gu8v_LEDState = LED_State_On;
						}
						gu8v_LEDTime = 0;//LED倒計時的時間
						gbv_LEDTime_Is_On = 1;//打開倒計時
						break;
					case G_State_Hold://若燈為開，則倒計時關燈
						if(gu8v_LEDState == LED_State_On)
						{
							if(gbv_LEDTime_Is_On == 1)
							{
								if(gu8v_LEDTime > gu8v_LEDTimeOutSet)//時間到
								{
									gu8v_LEDTime = 0;				
									gbv_LEDTime_Is_On = 0;
									gu8v_LEDState = LED_State_Off;
									gbv_NightLED_Is_AutoOff = 1;//小夜燈時間到，自動關閉，GL不成立，G狀態為Hold
								}
							}
						}
						break;
					case G_State_None:
					default :
						break;
				}
				switch(gu8v_LEDState)//判斷燈的狀態
				{
					case LED_State_Off : //關燈
						LEDPower1 = Off; 
						LEDPower2 = Off;
					break;
					case LED_State_On  : //開燈
						switch(gu8v_LEDLight)//判斷亮度：N1(default)，N2，N3
						{
							//由兩個IO口控制三種亮度，通過不同的電阻實現不同的亮度
							case LEDLight_Off:
							case LEDLight_N1:  LEDPower1 = On; 	LEDPower2 = Off;	break;
							case LEDLight_N2:  LEDPower1 = Off; LEDPower2 = On;		break;
							case LEDLight_N3:  LEDPower1 = On; 	LEDPower2 = On;		break;
						}
						break;
					default :break;
				}
				break;


			//為手動控制模式
			case WorkMode_KeyControl1_2:
				gbv_GADC_Is_On = 0;//關閉ADC
				if(gu8v_KeyState == Key_State_LongPress)
				{
					gu8v_KeyState = Key_State_None;
					gu8v_WorkMode = WorkMode_Auto;
					LEDPower1 = Off;
					LEDPower2 = Off;				//狀態切換，關燈，且回傳亮度至自動模式
				}
				if(gu8v_KeyState == Key_State_ShortPress)//設置亮度，寫入EEPRAM
				{
					gu8v_KeyState = Key_State_None;
					gu8v_LEDLight ++;
					V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDPowerFlag, EEPRAM_LEDPowerFlag);
					V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDPower, gu8v_LEDLight);
					if(gu8v_LEDLight > LEDLightNum)
					{
						gu8v_LEDLight = LEDLight_Off;
					}
				}
				switch(gu8v_LEDLight)
				{
					case LEDLight_Off: LEDPower1 = Off; LEDPower2 = Off; 	LED_StateTips_IO = Off;break;
					case LEDLight_N1:  LEDPower1 = On; 	LEDPower2 = Off; 	LED_StateTips_IO = On;break;
					case LEDLight_N2:  LEDPower1 = Off; LEDPower2 = On; 	LED_StateTips_IO = On;break;
					case LEDLight_N3:  LEDPower1 = On; 	LEDPower2 = On;		LED_StateTips_IO = On;break;
					default: break;
				}
				break;

			case WorkMode_None:
			default:
				break;
		}

		if(gu8v_TipsLEDStateLast != gu8v_TipsLEDState)
		{
			switch(gu8v_TipsLEDState)//指示燈狀態變量
			{
				#ifdef TipsLED_OneLED
				case TipsLED_State_None:LED_IO = 0x00;gu8v_LEDTimeOutSet = 00;break;//指示燈關閉
				case TipsLED_State_L1: LED_IO = 0x01; gu8v_LEDTimeOutSet = 2*60;break;//需要關閉其他的LED，只打開相對應的LED
				case TipsLED_State_L2: LED_IO = 0x02; gu8v_LEDTimeOutSet = 5*60;break; 
				case TipsLED_State_L3: LED_IO = 0x04; gu8v_LEDTimeOutSet = 10*60;break;  
				case TipsLED_State_L4: LED_IO = 0x08; gu8v_LEDTimeOutSet = 15*60;break; 
				case TipsLED_State_L5: LED_IO = 0x10; gu8v_LEDTimeOutSet = 20*60;break; 
				case TipsLED_State_L6: LED_IO = 0x20; gu8v_LEDTimeOutSet = 25*60;break; 
				case TipsLED_State_L7: LED_IO = 0x40; gu8v_LEDTimeOutSet = 30*60;break; 
				#endif
				#ifdef TipsLED_OneToTwoLED_10min//LED指示燈10分鐘一個等級
				case TipsLED_State_None:LED_IO = 0x00;gu8v_LEDTimeOutSet = 00;break;//指示燈關閉
				case TipsLED_State_L1: LED_IO = 0x01; gu8v_LEDTimeOutSet = 10*60;break;//需要關閉其他的LED，只打開相對應的LED
				case TipsLED_State_L2: LED_IO = 0x03; gu8v_LEDTimeOutSet = 20*60;break; 
				case TipsLED_State_L3: LED_IO = 0x07; gu8v_LEDTimeOutSet = 30*60;break;  
				case TipsLED_State_L4: LED_IO = 0x0f; gu8v_LEDTimeOutSet = 40*60;break; 
				case TipsLED_State_L5: LED_IO = 0x1f; gu8v_LEDTimeOutSet = 50*60;break; 
				case TipsLED_State_L6: LED_IO = 0x3f; gu8v_LEDTimeOutSet = 60*60;break; 
				case TipsLED_State_L7: LED_IO = 0x7f; gu8v_LEDTimeOutSet = 70*60;break; 
				#endif
				default :
					break;
			}
			if(gu8v_TipsLED_Time > gu8v_TipsLEDTimeOutSet)
			{
				gu8v_TipsLED_Time = 0;
				gu8v_TipsLEDStateLast = gu8v_TipsLEDState;
				LED_IO = 0x00;
				gbv_Key_Is_NoFirst = 0;
			}
		}
		else
		{
			gu8v_TipsLED_Time = 0;
		}
		
		switch(Tips_NightFindLED_State)
		{
			case Tips_NightFindLED_Off : 
				break;
			case Tips_NightFindLED_On :
				if(gbv_G_Is_H)
				{
					//指示燈關
					LED_NightFind_IO = Off;
				}
				else
				{
					//指示燈開
					if(gu8v_WorkMode == WorkMode_Auto)
					{
						if(gu8v_LEDState == LED_State_Off)
							LED_NightFind_IO = On;
						if(gu8v_LEDState == LED_State_On)
							LED_NightFind_IO = Off;
					}
					else if(gu8v_WorkMode == WorkMode_KeyControl1_2)
					{
						if(gu8v_LEDLight == LEDLight_Off)
							LED_NightFind_IO = On;
						else
							LED_NightFind_IO = Off;
					}
				}
				break;
			default :
				break;
		}


		switch(gu8v_KeyState)//按鍵狀態變量
		{
			case Key_State_None:
				break;
			case Key_State_ShortPress:
				break;
			case Key_State_LongPress:
				break;
			default :
				break;
		}
		#ifdef DemoDebug
		//OLED-Debug
		fun_OLEDDisplayDebug();	
		#endif	
	}	
}
#ifdef DemoDebug
void fun_OLEDDisplayDebug()
{
	static unsigned char i;
	if(gbv_GAD_OLEDDispayDebug_Time_1s )
	{
		if(gbv_GADC_Is_On == 1)
		{
			gbv_GAD_OLEDDispayDebug_Time_1s = 0;
			//Display光ADData
			oled_shownum(0,i,Drv_GetADC_AVGn(ADC_CHANNEL_AN2,10),5,12);
			oled_shownum(100,0,i,1,12);
			i++;	
			if(i > 7)
			{
				i = 0;
			}
		}
		oled_shownum(100,2,gu8v_LEDTimeOutSet/60,2,12);	
	}
}
#endif
void Delay_ms(u16 time)
{
	u16 i;
	for(i = time; i > 0; i--)	
	{
		GCC_DELAY(3000);	
	}
}

