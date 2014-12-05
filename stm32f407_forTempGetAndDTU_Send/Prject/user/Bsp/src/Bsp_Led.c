#include"Bsp_Led.h"
/* 
*		 初始化 LED 引脚 
**/
void Bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LED_Pin ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_SetBits(LED_PORT,LED_Pin);  // 
}
/* 
*   翻转  LED  状态 
*/
void Trage_Led(void)
{
	GPIO_ToggleBits(LED_PORT,LED_Pin);
}
/* 
*   改变LED状态 
*   sw    1 : LED 点亮
* 				0 ：LED 关闭
**/
void LED_Change(uint8_t sw)
{
	const BitAction BitVals[2]={Bit_SET,Bit_RESET};
	if(sw>=2)  sw=0;
	GPIO_WriteBit(LED_PORT,LED_Pin,BitVals[sw]);
}

