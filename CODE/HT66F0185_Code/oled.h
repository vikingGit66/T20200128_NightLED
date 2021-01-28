#ifndef _OLED_H_
#define _OLED_H_
#include "HT66F0185.h"

#define OLED_SCLC  _pdc2
#define OLED_SDAC  _pdc1

#define OLED_SCL   _pd2
#define OLED_SDA   _pd1

#define OUT 0
#define IN 1
#define LOW 0
#define HIGH 1

void delay_1us(unsigned char);
void oled_iic_start();
void oled_iic_stop();
unsigned char oled_iic_wait_ack();
void oled_write_byte(unsigned char);
void oled_write_cmd(unsigned char);
void oled_write_dat(unsigned char);
void oled_set_pos(unsigned char, unsigned char);
unsigned int oled_pow(unsigned char,unsigned char);
void oled_showchar(unsigned char ,unsigned char ,unsigned char , unsigned char);
void oled_shownum(unsigned char,unsigned char,unsigned int,unsigned char,unsigned char);
void oled_showstring(unsigned char x,unsigned char y,char *str,unsigned char size);
void oled_showphoto(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
void oled_init();
void oled_clear();
void oled_clear_hang(unsigned char);
#endif











