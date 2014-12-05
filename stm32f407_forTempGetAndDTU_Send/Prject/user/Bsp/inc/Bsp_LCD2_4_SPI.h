
#ifndef Bsp_LCD2_4_SPI_H_
#define Bsp_LCD2_4_SPI_H_
#include "stm32f4xx.h"


/* 
*  name : _InitLCD 
* 			初始化LCD显示屏
* 
**/
void _InitLCD(void);

/*
*   定点函数 
**/
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

/*
*
**/
static void LCD_WR_DATA(int da);
	
void LCD_Clear(u16 Color);
//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowString(u16 x,u16 y,const u8 *p);
#endif 
