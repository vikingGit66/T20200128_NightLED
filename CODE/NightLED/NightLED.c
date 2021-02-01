
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

//@ROM: 67% - 49%
//@RAM: 26% - 26%
//#define DemoDebug

#ifdef DemoDebug
void fun_OLEDDisplayDebug();
#endif

void main()
 {
	// �Д��Ƿ����늏�λ���߷�������r�µď�λ
	// �������늏�λ��������늏�λ��ʼ������֮����WDT�����ʼ��
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
		GHMin = GHMinDefault;
		GLMax = GLMaxDefault;
		gu8v_LEDState = LED_State_Off;
		
		//LED_Tips1_7_INIT
		LED_IOC = Out;//OUT
		LED_IO  = 0x00;//Off
		//LED_StateTips
		LED_StateTips_IOC = Out;//�ք�ģʽָʾ��
		LED_StateTips_IO  = Off;
		//LEDPower
		LEDPower1_IOC = Out;
		LEDPower1  	  = Off;
		LEDPower2_IOC = Out;
		LEDPower2     = Off;
		//���r�ҟ�ָʾ��
		LED_NightFind_IOC = Out;
		LED_NightFind_IO  = Off;
		Tips_NightFindLED_State = Tips_NightFindLED_On;


		GHMin = GHMinDefault;
		GLMax = GLMaxDefault;
		GL1_Data = GL1Default;
		GL2_Data = GL1L2Default;
		GL1L2_Data = GL1L2Default;
				
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
		// WDT�����λ��ʼ��
		GCC_CLRWDT();
		
	}
		
	//��ѭ�h
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
					//Beep푺�����_pa5
					//Beep_On();
				}
				else//gbv_Islvd = 0;
				{
					
				}
				SET_LVD_DISABLE();
			}
		}*/
		

		//���߿���
		//�����ߕr�g���defaultֵ�r���M�����ߣ��˕r�ɿ��T��1s���ѣ����M��AD�Д�����ȣ����ߕr�g���֞�defaultֵ
		//�����а��I���t�����ߕr�g��λ��0����default�r�g�ȣ����_TimeBase0���Ɍ��F���I�Ē���
		if(gu8v_HaltTime >= gu8v_HaltTimeSet30s)//���ߕr�g��default:30s
		{
			//Power Down
			TimeBase0_INT_Off();//�P�]���I���趨�r���Д�
			GCC_CLRWDT();
			GCC_HALT();//�M������
			TimeBase0_INT_On();//���_���I���趨�r��
			if(gbv_KeyIs0)//����鰴�I���ѣ��t���ߕr�g��ʼ����0
			{
				gbv_KeyIs0 = 0;
				gu8v_HaltTime = 0;
			}
			else//����鿴�T�����ѣ��t���ߕr�g���֞�defaultֵ
			{
				gu8v_HaltTime = gu8v_HaltTimeSet30s;//default:30s
			}
		}
		else
		{
			//NC
		}
		
		
		//LED�@ʾ����UI�OӋ
		//LED�@ʾ�YԴ��8+1 LED 
		//�r�g�ȼ���1-7�������Ă�������r�g�ĵȼ����t���G���S���{���t���G���S
		//ҹ�g���ҟ� 8 ��ͨ�^�[�ذ��I�_�����P�]����ʡ����):2s���B�m����3��KEY2���t�ГQ��B���ںڵ���r����
		//�ք��Ԅ�ģ 9 ��ʽ��ʾ�����������������ք�ģʽ���_�ğ����t��ʾ���ք�ģʽ����Ҫ�ք��P�]
		switch (gu8v_WorkMode)
		{
			//�Ԅ�ģʽУ�ʣ����Ԅ�ģʽ�£��L��8���M��У��ģʽ����ˮ��ѭ�h2s
			//�����Д�GH��GL��GL1-L2(V3.0�汾��ʹ��)���ɹ���30s�r�g����ȫ��2s���˳������Ԅ�ģʽ
			//�Ԅ�У�ʺ��豣��GL1��GL2��GL1L2��GLMax���x����Сֵ����GLMax
			case WorkMode_NightLEDAutoCal://�Ԅ�У��ģʽ
				//ROM:2% , RAM:2%
				
				gu8v_HaltTime = 0;
				LED_IO = 0x01;
				volatile unsigned char i;
				for(i = 0; i < 20; i++)//2s
				{
					GCC_CLRWDT();
					Delay_ms(100);//�ӕr100ms
					LED_IO <<= 1;
					if(LED_IO == 0x00)
						LED_IO = 0x01;
				}
				LED_IO = 0x00;
				volatile unsigned char LEDIO_Temp = 0;
				while(gu8v_LEDAutoCalTime < 30)//30s
				{
					GCC_CLRWDT();
					Delay_ms(100);//�ӕr100ms
					LEDIO_Temp = LED_IO;
					LED_IO = 0x00;
					G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
					if(G_AutoCalADCDataTemp >= GHMin)//��
					{
						GCC_CLRWDT();
						Delay_ms(200);//�ӕr100ms
						G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
						if(G_AutoCalADCDataTemp >= GHMin)
							gbv_AutoCal_GState = NightAutoCal_GState_Is_H;
					}
					else if(G_AutoCalADCDataTemp <= GLMax)//��
					{
						GCC_CLRWDT();
						Delay_ms(200);//�ӕr100ms
						G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
						if(G_AutoCalADCDataTemp <= GLMax)//��
							gbv_AutoCal_GState = NightAutoCal_GState_Is_L;
					}
					LED_IO = LEDIO_Temp;
					switch(gu8v_AutoCalState)
					{
						case AutoCalState_Is_Finding_H :
							if(gbv_AutoCal_GState == NightAutoCal_GState_Is_H)
							{
								gu8v_AutoCalState = AutoCalState_Is_Finded_H;
								GHMin = G_AutoCalADCDataTemp - 300;
								LED_IO = 0x00;
							}
							else //��ʾ�_�������Ƅ�
							{
								LED_IO <<= 1;
								if(LED_IO == 0x00)
									LED_IO = 0x01;
							}
							
							break;
						case AutoCalState_Is_Finded_H :
							gu8v_AutoCalState = AutoCalState_Is_Finding_L;
							break;
						case AutoCalState_Is_Finding_L :
							if(gbv_AutoCal_GState == NightAutoCal_GState_Is_L)
							{
								gu8v_AutoCalState = AutoCalState_Is_Finded_L;
								GLMax = G_AutoCalADCDataTemp + 500;
								LED_IO = 0x00;
							}
							else//��ʾ�P�������W�q
							{
								LED_IO = ~LED_IO;
							}
							break;
						case AutoCalState_Is_Finded_L :
							gu8v_AutoCalState = AutoCalState_Is_Setting_L1;
							break;
						case AutoCalState_Is_Setting_L1 :
							gu8v_AutoCalState = AutoCalState_Is_Setting_L2;
							LEDPower1 = On;
							LEDPower2 = Off;
							GCC_CLRWDT();
							Delay_ms(1000);//�ӕr1000ms
							G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
							GL1_Data = G_AutoCalADCDataTemp + 500;
							break;

						case AutoCalState_Is_Setting_L2 :
							gu8v_AutoCalState = AutoCalState_Is_Setting_L1L2;
							LEDPower1 = Off;
							LEDPower2 = On;
							GCC_CLRWDT();
							Delay_ms(1000);//�ӕr1000ms�����Hʹ�����ӕr���L1s
							G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
							GL2_Data = G_AutoCalADCDataTemp + 500;
							break;

						case AutoCalState_Is_Setting_L1L2 :
							gu8v_AutoCalState = AutoCalState_Is_LOK;
							LEDPower1 = On;
							LEDPower2 = On;
							GCC_CLRWDT();
							Delay_ms(1000);//�ӕr1000ms�����Hʹ�����ӕr���L1s
							G_AutoCalADCDataTemp = Drv_GetADC_AVGn(gu8V_GAD_channel,gu8V_GAD_Cnt);//��10�P��������ƽ��ֵ
							GL1L2_Data = G_AutoCalADCDataTemp + 500;
							break;
						case AutoCalState_Is_LOK :
							if(GHMin > GLMax)
							{
								gu8v_AutoCalState = AutoCalState_Is_OK;
							}
							else
							{
								gu8v_AutoCalState = AutoCalState_Is_Failed;
							}
							if((GL1_Data < GL2_Data) && (GL2_Data < GL1L2_Data))//���������F��
							{
								
							}
							
							break;
						case AutoCalState_Is_OK :
							gu8v_LEDAutoCalTime = 30;
							LED_IO  		= 0xff;//Off
							LED_StateTips_IO= On;
							LEDPower1  	  	= On;
							LEDPower2     	= On;
							LED_NightFind_IO= On;
							Delay_ms(2000);
							LED_IO  = 0x00;//Off
							LED_StateTips_IO  = Off;
							LEDPower1  	  = Off;
							LEDPower2     = Off;
							LED_NightFind_IO  = Off;
							break;
						case AutoCalState_Is_Failed :
							gu8v_LEDAutoCalTime = 31;
						default:
							break;
					}
					if((!KEYCAL))
					{
						Delay_ms(10);
						if((!KEYCAL))
						{
							gu8v_LEDAutoCalTime = 30;
							LED_IO  = 0x00;//Off
							LED_StateTips_IO  = Off;
							LEDPower1  	  = Off;
							LEDPower2     = Off;
							LED_NightFind_IO  = Off;
							
							GHMin = GHMinDefault;
							GLMax = GLMaxDefault;
							GL1_Data = GL1Default;
							GL2_Data = GL1L2Default;
							GL1L2_Data = GL1L2Default;
						}
					}
				}
				if(gu8v_LEDAutoCalTime == 31)
				{
					LED_IO  = 0x00;//Off
					LED_StateTips_IO  = Off;
					LEDPower1  	  = Off;
					LEDPower2     = Off;
					LED_NightFind_IO  = Off;
					
					GHMin = GHMinDefault;
					GLMax = GLMaxDefault;
					GL1_Data = GL1Default;
					GL2_Data = GL1L2Default;
					GL1L2_Data = GL1L2Default;
				}
				gu8v_AutoCalState = AutoCalState_Is_Finding_H;
				gu8v_WorkMode = WorkMode_Auto;
				
				break;

			case WorkMode_Auto:
				gbv_GADC_Is_On = 1;//���_ADC
				LED_StateTips_IO = Off;
				if(gu8v_KeyState == Key_State_LongPress)
				{
					gu8v_KeyState = Key_State_None;

					gu8v_LEDState = LED_State_Off;					//��B�ГQ���P��
					gu8v_LEDTime  = 0;								//LED��Ӌ�r��ʼ����0
					gu8v_TipsLED_Time = gu8v_TipsLEDTimeOutSet + 1;	//�P�]ָʾ��
					gu8v_LEDLight = LEDLight_N1;					//�״��M���ք�ģʽ���ȳ�ʼ����1
					
					volatile unsigned int gu8v_NightLEDAutoCalKeyTime = 0;
					//���Ԅ�ģʽ�£��L��8s���MȥУ��ģʽ
					while( (!KEYCAL) &&  (gu8v_NightLEDAutoCalKeyTime <= 8000/10))//8000/10  8s
					{
						gu8v_NightLEDAutoCalKeyTime++;
						GCC_DELAY(20000);//10ms
						GCC_CLRWDT();
					}
					if( gu8v_NightLEDAutoCalKeyTime <= 8000/10)//С�8s�ГQ���ք�ģʽ
					{
						gu8v_WorkMode = WorkMode_KeyControl1_2;
					}
					else//���8s�ГQ��У��ģʽ
					{
						gu8v_WorkMode = WorkMode_NightLEDAutoCal;
						gu8v_LEDAutoCalTime = 0;//У��ģʽ�r�g׃����ʼ����0
					}

				}
				if(gu8v_KeyState == Key_State_ShortPress)			//��B�̰����O���Ԅӕr�g
				{
					gu8v_KeyState = Key_State_None;
					gu8v_TipsLED_Time = 0;							//LED��ʾ���λ0
					
					//��������״ΰ��£��t�Ԅӕr�g++������EEPRAM
					if(gbv_Key_Is_NoFirst)//5s������״ΰ���
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
					else//�״ΰ��������P�t�_
					{
						gu8v_TipsLEDStateLast = 0x55;//���Ƹ��ģ�ָʾ���@ʾ��ǰ�r�g��B
						gbv_Key_Is_NoFirst = 1;

						gbv_NightLED_Is_HandTurnOut = 1;
						//��һ�ΰ��t��ȡ���������_���t��Ӌ�r�P�]
						if(gu8v_LEDState == LED_State_Off)
						{
							gu8v_LEDState = LED_State_On;//�_��
							gu8v_LEDTime = 0;//LED��Ӌ�r�ĕr�g
							gbv_LEDTime_Is_On = 1;//���_��Ӌ�r
						}
						else if(gu8v_LEDState == LED_State_On)
						{
							gu8v_LEDState = LED_State_Off;
						}
					}			
				}
				//�⏊׃���M���������ĵ�Ӌ�r����Ҫ�ǿ���LED
				switch(gu8v_GState)//�⏊��B׃��
				{
					case G_State_H://�ɰ�׃�����P��
						gu8v_LEDState = LED_State_Off;
						gu8v_LEDTime = 0;//LED��Ӌ�r�ĕr�g
						gbv_LEDTime_Is_On = 0;//�P�]��Ӌ�r
						gbv_NightLED_Is_AutoChange = 1;
						break;
					case G_State_L://����׃�����_��
						if(gu8v_LEDTimeOutSet != 0)//��ֹܛ�����e
						{
							gu8v_LEDState = LED_State_On;
						}
						gu8v_LEDTime = 0;//LED��Ӌ�r�ĕr�g
						gbv_LEDTime_Is_On = 1;//���_��Ӌ�r
						gbv_NightLED_Is_AutoChange = 1;
						break;
					case G_State_Hold://�������_���t��Ӌ�r�P��
						if(gu8v_LEDState == LED_State_On)
						{
							if(gbv_LEDTime_Is_On == 1)
							{
								if(gu8v_LEDTime > gu8v_LEDTimeOutSet)//�r�g��
								{
									gu8v_LEDTime = 0;				
									gbv_LEDTime_Is_On = 0;
									gu8v_LEDState = LED_State_Off;
									gbv_NightLED_Is_AutoChange = 1;//Сҹ���r�g�����Ԅ��P�]��GL��������G��B��Hold
								}
							}
						}
						break;
					case G_State_None:
					default :
						break;
				}
				switch(gu8v_LEDState)//�Д����Ġ�B
				{
					case LED_State_Off : //�P��
						LEDPower1 = Off; 
						LEDPower2 = Off;
					break;
					case LED_State_On  : //�_��
						switch(gu8v_LEDLight)//�Д����ȣ�N1(default)��N2��N3
						{
							//�Ƀɂ�IO�ڿ������N���ȣ�ͨ�^��ͬ����茍�F��ͬ������
							case LEDLight_Off:
							case LEDLight_N1:  LEDPower1 = On; 	LEDPower2 = Off;	break;
							case LEDLight_N2:  LEDPower1 = Off; LEDPower2 = On;		break;
							case LEDLight_N3:  LEDPower1 = On; 	LEDPower2 = On;		break;
						}
						break;
					default :break;
				}
				
				if(gu8v_TipsLEDStateLast != gu8v_TipsLEDState)
				{
					switch(gu8v_TipsLEDState)//ָʾ����B׃��
					{
						#ifdef TipsLED_OneLED
						case TipsLED_State_None:LED_IO = 0x00;gu8v_LEDTimeOutSet = 00;break;//ָʾ���P�]
						case TipsLED_State_L1: LED_IO = 0x01; gu8v_LEDTimeOutSet = 2*60;break;//��Ҫ�P�]������LED��ֻ���_��������LED
						case TipsLED_State_L2: LED_IO = 0x02; gu8v_LEDTimeOutSet = 5*60;break; 
						case TipsLED_State_L3: LED_IO = 0x04; gu8v_LEDTimeOutSet = 10*60;break;  
						case TipsLED_State_L4: LED_IO = 0x08; gu8v_LEDTimeOutSet = 15*60;break; 
						case TipsLED_State_L5: LED_IO = 0x10; gu8v_LEDTimeOutSet = 20*60;break; 
						case TipsLED_State_L6: LED_IO = 0x20; gu8v_LEDTimeOutSet = 25*60;break; 
						case TipsLED_State_L7: LED_IO = 0x40; gu8v_LEDTimeOutSet = 30*60;break; 
						#endif
						#ifdef TipsLED_OneToTwoLED_10min//LEDָʾ��10���һ���ȼ�
						case TipsLED_State_None:LED_IO = 0x00;gu8v_LEDTimeOutSet = 00*60;break;//ָʾ���P�]
						case TipsLED_State_L1: LED_IO = 0x01; gu8v_LEDTimeOutSet = 03*60;break;//��Ҫ�P�]������LED��ֻ���_��������LED
						case TipsLED_State_L2: LED_IO = 0x03; gu8v_LEDTimeOutSet = 10*60;break; 
						case TipsLED_State_L3: LED_IO = 0x07; gu8v_LEDTimeOutSet = 20*60;break;  
						case TipsLED_State_L4: LED_IO = 0x0f; gu8v_LEDTimeOutSet = 30*60;break; 
						case TipsLED_State_L5: LED_IO = 0x1f; gu8v_LEDTimeOutSet = 40*60;break; 
						case TipsLED_State_L6: LED_IO = 0x3f; gu8v_LEDTimeOutSet = 50*60;break; 
						case TipsLED_State_L7: LED_IO = 0x7f; gu8v_LEDTimeOutSet = 60*60;break; 
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
					else
					{
						//NC
					}
				}

				break;


			//���քӿ���ģʽ
			case WorkMode_KeyControl1_2:
				if(Tips_NightFindLED_State == Tips_NightFindLED_Off)
				{
					gbv_GADC_Is_On = 0;//�P�]ADC
				}
				else if(Tips_NightFindLED_State == Tips_NightFindLED_On)
				{
					gbv_GADC_Is_On = 1;//���_ADC
				}
				
				if(gu8v_KeyState == Key_State_LongPress)
				{
					gu8v_KeyState = Key_State_None;
					gu8v_WorkMode = WorkMode_Auto;
					LEDPower1 = Off;
					LEDPower2 = Off;				//��B�ГQ���P�����һ؂��������Ԅ�ģʽ
					gu8v_TipsLEDStateLast = 0x55;//���Ƹ��ģ�ָʾ���@ʾ��ǰ�r�g��B
					gu8v_TipsLED_Time = 0;//ָʾ���r�g��ʼ����0
				}
				if(gu8v_KeyState == Key_State_ShortPress)//�O�����ȣ�����EEPRAM
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
		switch(Tips_NightFindLED_State)
		{
			case Tips_NightFindLED_Off : 
				break;
			case Tips_NightFindLED_On :
				if(gbv_G_Is_H)
				{
					//ָʾ���P
					LED_NightFind_IO = Off;
				}
				else
				{
					//ָʾ���_
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
					else
					{
						LED_NightFind_IO = Off;
					}
					
				}
				break;
			default :
				break;
		}


		switch(gu8v_KeyState)//���I��B׃��
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
		switch(gu8v_WorkMode)
		{
			case WorkMode_NightLEDAutoCal:
				oled_showstring(60,4,"Auto-Cal",12);
				break;
			case WorkMode_Auto :
				oled_showstring(60,4,"AutoWork",12);
				break;
			default :
				break;
		}
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
			//Display��ADData
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


