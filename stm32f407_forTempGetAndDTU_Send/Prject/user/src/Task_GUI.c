#include"Task_GUI.h"
// #include"GUIDEMO.h"
//#include<songti16.c>
//#include<SongTi24.c>
//#include<songti48.c>
//#include<zhandian16.c>
//#include"LCD_interface.h"
//#include"Bsp_Touch_SPI.h"
//#include"GUI_DraweSing.h"
#include "MainTaskGUI.h"
#include"time.h"
#include"stdio.h"
#include"stdlib.h"
void AppTaskGUI(void *p_arg)
{
//	uint8_t i;
	OS_ERR      err;
  OSTimeGet(&err);	
	(void) p_arg;
	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);   // 开启CRC 功能
	
	//Lcd_Init();   // LCD 初始化 
	
  //Touch_Init();  // 显示屏初始化 
	
 	
  //WM_SetCreateFlags(WM_CF_MEMDEV); // 开启 窗口 内存允许
  //GUI_Init();	    // ＧＵＩ开始 
	//GUI_Clear();    // 清屏 
	// FistPageDrawLine();
	// CPU_SR_ALLOC();
	// CPU_CRITICAL_ENTER(); // 禁止中断
	//backWIn_init();  //调用显示屏显示界面
  //Draw_init();
// 	GUI_CURSOR_Show();    //显示指针 
   //pageChangFlag=0;
	 while(1)
	 {
		 BSP_OS_TimeDly(200);
	 }
}
void AppTaskGUIDrawing(void *p_arg)
{
	p_arg=p_arg;
	while(1)
	{
		
	}
	 
}
void AppTaskGUIRefresh(void *p_arg)
{
  (void)p_arg;
	while(1)
	{
		  // GUI_Exec();             //    GUI 切换
		  BSP_OS_TimeDly(300);
	}
}
