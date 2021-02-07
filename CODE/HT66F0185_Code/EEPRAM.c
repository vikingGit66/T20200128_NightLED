
#include "common.h"

#define MAX_EEPROM_ADDR	128
#define ERROR 0xff

void V_EEPRAM_Start()
{
	volatile unsigned char data_temp[8];
	data_temp[0] = V_EEPRAM_Read_Byte(EEPRAM_Addr_TipsLEDFlag);
	data_temp[1] = V_EEPRAM_Read_Byte(EEPRAM_Addr_TipsLED);
	data_temp[2] = V_EEPRAM_Read_Byte(EEPRAM_Addr_LEDPowerFlag);
	data_temp[3] = V_EEPRAM_Read_Byte(EEPRAM_Addr_LEDPower);

	data_temp[4] = V_EEPRAM_Read_Byte(EEPRAM_Addr_NightFindLEDFlag);
	data_temp[5] = V_EEPRAM_Read_Byte(EEPRAM_Addr_NightFindLED);
	data_temp[6] = V_EEPRAM_Read_Byte(EEPRAM_Addr_LEDAutoChangeFlag);
	data_temp[7] = V_EEPRAM_Read_Byte(EEPRAM_Addr_LEDAutoChange);
	
	if(data_temp[0] == EEPRAM_TipsLEDFlag)
	{
		//讀取
		gu8v_TipsLEDState = data_temp[1];
		if(gu8v_TipsLEDState > TipsLED_LNum)
		{
			gu8v_TipsLEDState = TipsLED_State_None;
		}
	}
	else
	{
		//初始化
		gu8v_TipsLEDState = TipsLED_State_L1;
		V_EEPRAM_Write_Byte(EEPRAM_Addr_TipsLEDFlag, EEPRAM_TipsLEDFlag);
		V_EEPRAM_Write_Byte(EEPRAM_Addr_TipsLED, gu8v_TipsLEDState);
	}
	
	if(data_temp[2] == EEPRAM_LEDPowerFlag)
	{
		//讀取
		gu8v_LEDLight = data_temp[2];
		if(gu8v_LEDLight > LEDLightNum)
		{
			gu8v_LEDLight = LEDLight_Off;
		}
	}
	else
	{
		//初始化
		gu8v_LEDLight = LEDLight_N1;
		V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDPowerFlag, EEPRAM_LEDPowerFlag);
		V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDPower, gu8v_LEDLight);
	}
	if(data_temp[4] == EEPRAM_NightFindLEDFlag)
	{
		//讀取
		if(data_temp[5] > 1)
		{
			data_temp[5] = 0;
		}
		Tips_NightFindLED_State = data_temp[5];
	}
	else
	{
		//初始化
		Tips_NightFindLED_State = 0;
		V_EEPRAM_Write_Byte(EEPRAM_Addr_NightFindLEDFlag, EEPRAM_NightFindLEDFlag);
		V_EEPRAM_Write_Byte(EEPRAM_Addr_NightFindLED, Tips_NightFindLED_State);
	}
	if(data_temp[6] == EEPRAM_LEDAutoChangeFlag)
	{
		//讀取
		if(data_temp[7] > 1)
		{
			data_temp[7] = 0;
		}
		gbv_LEDLight_Is_AutoChange = data_temp[7];
		
	}
	else
	{
		//初始化
		gbv_LEDLight_Is_AutoChange = 0;
		V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDAutoChangeFlag, EEPRAM_LEDAutoChangeFlag);
		V_EEPRAM_Write_Byte(EEPRAM_Addr_LEDAutoChange, gbv_LEDLight_Is_AutoChange);
	}
}
unsigned char V_EEPRAM_Read_Byte(unsigned char addr)
{
	if (addr > MAX_EEPROM_ADDR)
	{
		return ERROR;
	}
	else
	{
		unsigned char data;
		bit EMI_Protect;
		EMI_Protect = _emi;
		_emi = 0;
		_eea = addr;
		_bp = 0x01;
		_mp1 = 0x40;
		asm("set [0x02].1");	// RDEN
		asm("set [0x02].0");	// RD
		asm("sz  [0x02].0");	// RD
		asm("jmp $-1");
		_iar1 = 0;
		_emi = EMI_Protect;
		data = _eed;
		return data;
	}
}
unsigned char V_EEPRAM_Write_Byte(unsigned char addr, unsigned char data)
{
	if (addr > MAX_EEPROM_ADDR)
	{
		return ERROR;
	}
	else
	{
		bit EMI_Protect;
		EMI_Protect = _emi;
		_emi = 0;
		_mp1 = 0x40;
		_eea = addr;
		_eed = data;
		//_WREN與_WR需在連續的兩個指令週期內完成
		asm("set [0x02].3");	// WREN
		asm("set [0x02].2");	// WR
		asm("sz  [0x02].2");	// WR
		asm("jmp $-1");
		_iar1 = 0; //disable WREN
		_emi = EMI_Protect;
		if (V_EEPRAM_Read_Byte(addr) != data)
		{
			return ERROR;
		}
	}
	return 0x55;
}