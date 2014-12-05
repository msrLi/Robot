
#include"Bsp_ENC.h"
#define ENC_Bin GPIO_Pin_4 
#define M_S_2_ENPin  GPIO_Pin_5

/*
*  name : BspInitTime5ENC  
*  			初始化定时器5的脉冲累加器
**/
void BspInitTime5ENC(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);	
  /*  config GPIOA_0 and GPIOA_1 as input */	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);

/*  */
  GPIO_InitStructure.GPIO_Pin=ENC_Bin | M_S_2_ENPin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB,M_S_2_ENPin);
	GPIO_ResetBits(GPIOB,ENC_Bin);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0x0;   //  预分频器
	TIM_TimeBaseStructure.TIM_Period=0xFFFFFFFF;		 //  重载寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);	   // 
	
	TIM_EncoderInterfaceConfig(TIM5,\
														 TIM_EncoderMode_TI1,\
														 TIM_ICPolarity_Rising,\
														 TIM_ICPolarity_Rising);
														 
	TIM_ICStructInit(&TIM_ICInitStructure);	
	TIM_ICInitStructure.TIM_ICFilter=0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	TIM_Cmd(TIM5, ENABLE); 
}
/*
*  name : BspInitTime2ENC  
*  			初始化定时器2的脉冲累加器
**/
void BspInitTime2ENC(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
  /*  config GPIOA_15 and GPIOB_3 as input */	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//  | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//  | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);

  GPIO_InitStructure.GPIO_Pin=ENC_Bin | M_S_2_ENPin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB,M_S_2_ENPin);
	GPIO_ResetBits(GPIOB,ENC_Bin);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0x0;   //  预分频器
	TIM_TimeBaseStructure.TIM_Period=0xFFFFFFFF;		 //  重载寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	   // 
	
	TIM_EncoderInterfaceConfig(TIM2,\
														 TIM_EncoderMode_TI1,\
														 TIM_ICPolarity_Rising,\
														 TIM_ICPolarity_Rising);
														 
	TIM_ICStructInit(&TIM_ICInitStructure);	
	TIM_ICInitStructure.TIM_ICFilter=0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); 
}


