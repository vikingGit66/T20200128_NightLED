#ifndef _EEPRAM_H_
#define _EEPRAM_H_

#define EEPRAM_Addr_TipsLEDFlag 	    0x00
#define EEPRAM_Addr_TipsLED 		    0x01
#define EEPRAM_Addr_LEDPowerFlag 	    0x02
#define EEPRAM_Addr_LEDPower 		    0x03
#define EEPRAM_Addr_NightFindLEDFlag    0x04
#define EEPRAM_Addr_NightFindLED        0x05
#define EEPRAM_Addr_LEDAutoChangeFlag   0x06
#define EEPRAM_Addr_LEDAutoChange       0x07

#define EEPRAM_TipsLEDFlag 		        0xaa
#define EEPRAM_LEDPowerFlag 	        0x55
#define EEPRAM_NightFindLEDFlag         0x5a
#define EEPRAM_LEDAutoChangeFlag        0xa5

void V_EEPRAM_Start();
unsigned char V_EEPRAM_Write_Byte(unsigned char addr, unsigned char data);
unsigned char V_EEPRAM_Read_Byte(unsigned char addr);

#endif



