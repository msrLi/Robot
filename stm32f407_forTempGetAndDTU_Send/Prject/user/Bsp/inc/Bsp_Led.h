#ifndef _BSP_LED_H_
#define _BSP_LED_H_
#include "stm32f4xx.h"
#include "bsp.h"
#define LED_PORT  GPIOD
#define LED_Pin  GPIO_Pin_10

/* 
*		 初始化 LED 引脚 
**/
void Bsp_InitLed(void);
/* 
*   翻转  LED  状态 
*/
void Trage_Led(void);
/* 
*   改变LED状态 
*   sw    1 : LED 点亮
* 				0 ：LED 关闭
**/
void LED_Change(uint8_t sw);
#endif
