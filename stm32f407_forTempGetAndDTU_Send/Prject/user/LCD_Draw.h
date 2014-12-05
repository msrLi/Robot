
#ifndef LCD_Draw_H_
#define LCD_Draw_H_
#include"stm32f4xx.h"
#include"bsp.h"

/* 
*  ShowChar
*     显示 字符串 
*	  x,y 坐标 
*			buff 要显示的字符串 
**/
void ShowChar(uint8_t x,uint8_t y,const char buff[]);
/*
*  ShowNumber 
*  	  显示整数
*      * x   y 坐标 
*      num  数字量
**/ 
void ShowNumber(uint8_t x,uint8_t y,int num);
/*
*  ShowNumber 
*  	  显示浮点型
*      * x   y 坐标 
*      num  数字量
**/ 
void ShowNumberFloat(uint8_t x,uint8_t y,float num);
#endif 
