
/*----------------------------------------*/
/*----OLED显示屏文件 											*/
/*----版本：V1.0														*/
/*----作者：Viking 												*/
/*----时间：T20200416 											*/
/*----注： 																*/
/*----------------------------------------*/

#include "HT66F0185.h"
#include "oled.h"
#include "oled_ku.h"
#define u8 	unsigned char
#define u16 unsigned int
void delay_1us(u8 time)
{
	GCC_DELAY(1);	
}
//--------模仿iic信号----------//
void oled_iic_start(void)
{
	OLED_SDAC = OUT;
	OLED_SCLC = OUT;	
	OLED_SCL  = HIGH;//时钟线设置为高电平
	OLED_SDA  = HIGH;	//数据线设置为高电平
	delay_1us(1);
	OLED_SDA  = LOW;		//数据线设置为低电平
	delay_1us(1);
	OLED_SCL  = LOW;		//时钟线设置为低电平
	delay_1us(1);
}


void oled_iic_stop(void)
{
	OLED_SDAC = OUT;
	OLED_SDA  = LOW;		//数据线设置为低电平
	delay_1us(1);
	OLED_SCL  = HIGH;	//时钟线设置为高电平
	delay_1us(1);
	OLED_SDA  = HIGH;	//数据线设置为高电平
	delay_1us(1);
}


u8 oled_iic_wait_ack(void)
{
	u8 ack;
	OLED_SDAC = OUT;
	OLED_SCL  = LOW;		//时钟线设置为低电平
	delay_1us(1);
	OLED_SDA  = HIGH;	//数据线设置为高电平
	delay_1us(1);
	OLED_SCL  = HIGH;	//时钟线设置为高电平
	delay_1us(1);
	
	OLED_SDAC = IN;
	if(OLED_SDA == 1)	//读取数据线的数据
		ack = 1;
	else
		ack = 0;
	
	OLED_SDAC = OUT;
	OLED_SCL  = LOW;		//时钟线设置为低电平
	delay_1us(1);

	return ack;			//返回读取到的应答信息
}


void oled_write_byte(u8 byte)
{
	u8 i;
	OLED_SDAC = OUT;
	for(i=0;i<8;i++)
	{
		
		OLED_SCL  = LOW;				//数据线设置为低电平
		delay_1us(1);
		
		if(byte & 0x80)
		{				//读取最高位，如果最高位为1，将时钟线设置为高电平
		  	OLED_SDA  = HIGH;		//数据线设置为高电平
		}
		else
		{
			OLED_SDA  = LOW;			//数据线设置为低电平
		}
		byte <<= 1;				//将数据左移1位
		delay_1us(1);
		OLED_SCL  = HIGH;			//时钟线设置为高电平，产生上升沿
		delay_1us(1);
	}
	OLED_SCL  = LOW;				//时钟线设置为低电平
	delay_1us(1);
	
	i=0;
	while(oled_iic_wait_ack()&&i!=255)		//等待应答
	{
		i++;
		GCC_DELAY(12000);//delay 1ms
	}
	
}


void oled_write_cmd(u8 cmd)
{
   oled_iic_start();
   oled_write_byte(0x78);//写入从机地址
   oled_write_byte(0x00);//写入命令
   oled_write_byte(cmd);//写入指令
   oled_iic_stop();
}


void oled_write_dat(u8 dat)
{
   oled_iic_start();
   oled_write_byte(0x78);//写入从机地址
   oled_write_byte(0x40);//写入数据
   oled_write_byte(dat);//写入数据
   oled_iic_stop();
}


void oled_set_pos(u8 x, u8 y)
{ 
	oled_write_cmd(0xb0+y);					//写入页地址
	oled_write_cmd((x&0x0f));				//写入列地址，低半字节
	oled_write_cmd(((x&0xf0)>>4)|0x10);		//写入列地址，高半字节
}   	  


u16 oled_pow(u8 m,u8 n)
{
	u16 result=1;	 
	while(n--)result*=m;    
	return result;
}

/****************************************************
 * 函数名： void oled_showchar(unsigned char x,unsigned char y,unsigned char chr)
 * 	 参数：
 * 			x-----起始列
 * 			y-----起始页
 * 			chr---字符
****************************************************/
void oled_showchar(u8 x,u8 y,u8 chr, u8 Char_Size)
{      	
	u8 c = 0,i = 0;
	c = chr-' ';									//获取字符偏移量
	if(x > 120-1)							//如果列数超出范围，从下一页的第0列开始
	{
		x = 0;
		y = y+2;
	}
	if(Char_Size ==16)
	{
		//oled_set_pos(x,y);			
		//for(i=0;i<8;i++)
		//	oled_write_dat(F8X16[c*16+i]);				//找出字符c的数组位置，先把第一页的列画完
		//oled_set_pos(x,y+1);							//页数+1
		//for(i = 0;i < 8;i++)							//循环8次，占8列
		//	oled_write_dat(F8X16[c*16+i+8]);			//把第二页的列数画完
	}
	else 
	{	
		oled_set_pos(x,y);			
		for(i=0;i<6;i++)
			oled_write_dat(F6x8[c][i]);
	}
}

/****************************************************
 * 函数名： void oled_shownumber(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size)
 * 	 参数：
 * 			x-----起始列
 * 			y-----起始页
 * 			num---显示的数字
 * 			len---数字的长度
 * 			size--显示数字的大小，在这里用12
****************************************************/
void oled_shownum(u8 x,u8 y,u16 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;			
	
	for(t=0;t<len;t++)    //只能显示到256,超过256重复回到1 ？？？
	{
		temp=(num/oled_pow(10,len-t-1))%10;    //可以学习这行代码
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				//oled_showchar(x+(size2/2)*t,y,' ',size2);//高位为0时不显示
				oled_showchar(x+(size2/2)*t,y,'0',size2);//高位为0时显示0
				continue;
			}else enshow=1; 
		 	 
		}
	 	oled_showchar(x+(size2/2)*t,y,temp+'0',size2);
		
	}
} 

/****************************************************
 * 函数名： void oled_showstring(unsigned char x,unsigned char y,unsigned char *str)
 * 	 参数：
 * 			x-----起始列
 * 			y-----起始页
 * 			*str--字符指针
****************************************************/
void oled_showstring(u8 x,u8 y,char *str,u8 size)
{
	u8 j=0;
	while (str[j]!='\0')
	{		
		oled_showchar(x,y,str[j],size);
		x+=8;
		if(x>120)
		{
			x=0;y+=2;
		}
			j++;
	}
}

void oled_showphoto(u8 x0, u8 y0,u8 x1, u8 y1,const u8 BMP[])
{ 	
 	unsigned int j=0;
 	unsigned char x,y;
  
 	if(y1%8==0) 
		y=y1/8;
 	else 
		y=y1/8+1;

	for(y=y0;y<y1;y++)
	{
		oled_set_pos(x0,y);
		for(x=x0;x<x1;x++)
			oled_write_dat(BMP[j++]);
	}
} 

/****************************************************
 * 函数名： void oled_init(void)
 * 	 参数：
 * 			无
 *   作用：初始化oled
****************************************************/
void oled_init(void)
{
	//oled_gpio_init();		//gpio口初始化
 	OLED_SDAC = OUT;
	OLED_SCLC = OUT;
	
	
//	_papu4 = 1;
//	_papu5 = 1;
	
	
	oled_write_cmd(0xAE);	//关闭显示

	oled_write_cmd(0x00);	//设置低列地址
	oled_write_cmd(0x10);	//设置高列地址
	oled_write_cmd(0x40);	//设置起始行地址
	oled_write_cmd(0xB0);	//设置页地址

	oled_write_cmd(0x81);	//对比度设置，可设置亮度
	oled_write_cmd(0xFF);	//265

	oled_write_cmd(0xA1);	//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址
	oled_write_cmd(0xA6);	//正常显示；0xa7逆显示

	oled_write_cmd(0xA8);	//设置驱动路上（16~64）
	oled_write_cmd(0x3F);	//64duty
	
	oled_write_cmd(0xC8);	//重映射模式，COM[N-1]~COM0扫描

	oled_write_cmd(0xD3);	//设置显示偏移
	oled_write_cmd(0x00);	//无偏移
	
	oled_write_cmd(0xD5);	//设置震荡器分频
	oled_write_cmd(0x80);	//使用默认值
	
	oled_write_cmd(0xD9);	//设置Pre-Charge Period
	oled_write_cmd(0xF1);	//使用官方推荐值
	
	oled_write_cmd(0xDA);	//设置com pin configuartion
	oled_write_cmd(0x12);	//使用默认值
	
	oled_write_cmd(0xDB);	//设置Vcomh，可调节亮度（默认）
	oled_write_cmd(0x40);	//使用官方推荐值
	
	oled_write_cmd(0x8D);	//设置OLED电荷泵
	oled_write_cmd(0x14);	//开显示
	
	oled_write_cmd(0xAF);	//开启OLED面板显示
	oled_clear();			//清屏
	oled_set_pos(0,0);		//设置数据写入的起始行、列
}  

/****************************************************
 * 函数名： void oled_clear(void) 
 * 	 参数：
 * 			无
 * 	 作用：清屏
****************************************************/
void oled_clear(void)  
{  
	unsigned char i,n;
	for(i=0;i<8;i++)  
	{  
		oled_write_cmd(0xb0+i);	//从0~7页依次写入
		oled_write_cmd(0x00);		//列低地址
		oled_write_cmd(0x10);		//列高地址
		for(n=0;n<128;n++)
			oled_write_dat(0);	//写入0清屏
	}
}
void oled_clear_hang(u8 hang)  
{  
	u8 n;
	if(hang <= 7 && hang >= 0)
	{	
		oled_write_cmd(0xb0+hang);	//从0~7页依次写入
		oled_write_cmd(0x00);		//列低地址
		oled_write_cmd(0x10);		//列高地址
		for(n=0;n<128;n++)
			oled_write_dat(0);	//写入0清屏
	}
}




