
#include"LCD_Draw.h"

/* 
*  ShowChar
*     显示 字符串 
*	  x,y 坐标 
*			buff 要显示的字符串 
**/
void ShowChar(uint8_t x,uint8_t y,const char buff[])
{
	LCD_ShowString(x,y,(const u8 *)buff);
}
/*
*  ShowNumber 
*  	  显示整数
*      * x   y 坐标 
*      num  数字量
**/ 
void ShowNumber(uint8_t x,uint8_t y,int num)
{
	char sbuff[10];
	uint8_t flag=0;
	if(num<0){
		num=0-num;
		flag=1;	
	}
	if(flag==1)
	{
		LCD_ShowString(x,y,(const u8 *)"-");
	}
	myitoa(num,sbuff,10);
	LCD_ShowString(x+8,y,(const u8 *)sbuff);
	
}
/*
*  ShowNumber 
*  	  显示浮点型
*      * x   y 坐标 
*      num  数字量
**/ 
void ShowNumberFloat(uint8_t x,uint8_t y,float num)
{
	char sbuff[20];
	uint8_t flag=0;
	if(num<0){
		num=0-num;
		flag=1;	
	}
	LCD_ShowString(x,y,(const u8 *)"      ");
	if(flag==1)
	{
		LCD_ShowString(x,y,(const u8 *)"-");
		x+=8;
	}
	my_ftoa(num,2,sbuff);
	LCD_ShowString(x,y,(const u8 *)sbuff);
	
}

