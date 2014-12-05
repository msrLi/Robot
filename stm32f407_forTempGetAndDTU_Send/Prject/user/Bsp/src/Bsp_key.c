
#include"Bsp_key.h"
// uint32_t KeyTimeInit[2];
int8_t KeyFlag=0;

/*
*  void Init_key()
**/
void Init_key(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//Open SYSCFG  clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 | GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);	

	TIM_Cmd(TIM12, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=10000;   //  预分频器   4MHz  // 2400M
	TIM_TimeBaseStructure.TIM_Period=0xffff;		 //  重载寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);	   //	
  TIM_Cmd(TIM12, ENABLE);
	Exit_config();
	NVIC_Config_Exti();
	
}
/* 
*  Exit_config
*  外部中断初始化化
**/
static void Exit_config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;	
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);    //Selects the GPIO pin used as EXTI Line.
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   // 下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	/*  */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource4);    //Selects the GPIO pin used as EXTI Line.
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   // 下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

		/*  */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);    //Selects the GPIO pin used as EXTI Line.
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;   // 下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	/*  */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);    //Selects the GPIO pin used as EXTI Line.
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   // 下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	/*  */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);    //Selects the GPIO pin used as EXTI Line.
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;   // 下降沿中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
}
/* 
*  NVIC_Config_Exti 
*  外部中断初始化 
**/
static void NVIC_Config_Exti(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;


  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*
*  Deal_keyButtom
**/
void Deal_keyButtom(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		LCD_ShowString(100,80,"KEY2 OK");
	}
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		LCD_ShowString(100,100,"KEY4 OK");
	}
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		LCD_ShowString(120,120,"KEY0 OK");
	}
//	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//	{
//		LCD_ShowString(150,150,"KEY1 OK");
//	}
//	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//	{
//		LCD_ShowString(180,180,"KEY2 OK");
//	}
}
void KeyENCDeal(void)
{
	char srbuff[10];
  static uint8_t up,down;

		if(KeyFlag==1)
		{
			up++;
			LCD_ShowString(150,150,"Up");
			myitoa(up,srbuff,10);
			LCD_ShowString(170,150,srbuff);
		}
		else if(KeyFlag==-1)
		{
			down++;
			LCD_ShowString(150,180,"Down");	
			myitoa(down,srbuff,10);
			LCD_ShowString(170,180,srbuff);
		}

}