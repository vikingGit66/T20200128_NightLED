/*
 * @時鐘，小夜燈設置的時間，各個頭文件
 * @創建時間：T20201028-09：36
 * @作者：Viking
 * @版本號：V2.0
 * @注：未添加AD功能
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#define u8 unsigned char
#define u16 unsigned int
#define uchar unsigned char
#define uint unsigned int



//時鐘
#define SYS_CLOCK_FREQ 32000 //4000000
#define _LIRC_

#define LED_TIme_OffSet_s 10 //s//小夜燈開啟時間補償

#define LED_Power_Time_Data 3 //亮3s//設置小夜燈種類，循環退出的時間
#define LED_Power_Time_Data_offset  LED_Power_Time_Data *2/3 //自動補償

#define LED1_8_Time_Data 5 //亮5s//設置小夜開啟時長類，循環退出的時間
#define LED1_8_Time_Data_offset LED1_8_Time_Data *2/3//自動補償

#include "HT66F0185.h"
#include "..\HT66F0185_Code\GPIO.h"
#include "..\HT66F0185_Code\SysClock.h"
#include "..\HT66F0185_Code\LED.h"
#include "..\HT66F0185_Code\RAM.h"
#include "..\HT66F0185_Code\AD.h"
#include "..\HT66F0185_Code\Timer.h"
#include "..\HT66F0185_Code\KeyScan.h"
#include "..\HT66F0185_Code\EEPRAM.h"
#endif
