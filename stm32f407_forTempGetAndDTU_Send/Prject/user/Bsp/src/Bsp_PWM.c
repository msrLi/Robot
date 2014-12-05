
#include"Bsp_PWM.h"
#define ENC_Bin GPIO_Pin_4 
#define M_S_2_ENPin  GPIO_Pin_5
#define StartZhan  0
#define StartZhan1 3000
/* 
*   定时器23  产生PWM 20KHz
*  Time2 CH1  PA15
*				 CH2  PB3
* 			 CH3  PB10
*				 Ch4  PB11
*  Time3 CH1  PC6 
*        CH2  PC7 
*        CH3  PC8 
*        CH4  PC9 
**/
void InitPWMAs20kHz(TIM_TypeDef* TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;	
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
  GPIO_InitStructure.GPIO_Pin=ENC_Bin | M_S_2_ENPin | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	GPIO_SetBits(GPIOB,M_S_2_ENPin);
	GPIO_SetBits(GPIOB,ENC_Bin);
//	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	if(TIMx==TIM3)
	{	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
		/*  config GPIOA_15 as TIME2_CH1 */	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);
		
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
	}else if(TIMx==TIM2){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);	
		/*  config GPIOA_15 as TIME2_CH1 */	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15; //  | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3  | GPIO_Pin_10 | GPIO_Pin_11 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);
		
		GPIO_ResetBits(GPIOB,M_S_2_ENPin);   //  open clock 2 Enable
	}

 /* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 20 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /20 MHz) - 1
                                              
    To get TIM3 output clock at 30 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM3 counter clock / TIM3 output clock) - 1
           = 665
                  
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */  

  /* Compute the prescaler value */	
	TIM_Cmd(TIMx, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=0;   //  预分频器   4MHz
	TIM_TimeBaseStructure.TIM_Period=4000;		 //  重载寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);	   //	

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIMx, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan;

  TIM_OC2Init(TIMx, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan;

  TIM_OC3Init(TIMx, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan;

  TIM_OC4Init(TIMx, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIMx, ENABLE);

  /* TIMx enable counter */
  TIM_Cmd(TIMx, ENABLE);
}

/* 
*   定时器23  产生PWM 20KHz
*  Time4 CH1  PB6
*				 CH2  PB7
* 			 CH3  PB8
*				 Ch4  PB9
*  Time5 CH1  PA0 
*        CH2  PA1
*        CH3  PA2
*        CH4  PA3
**/
void InitPWMAs200Hz(TIM_TypeDef* TIMx)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;	
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
  GPIO_InitStructure.GPIO_Pin=ENC_Bin | M_S_2_ENPin | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	GPIO_SetBits(GPIOB,M_S_2_ENPin);
	GPIO_SetBits(GPIOB,ENC_Bin);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
	if(TIMx==TIM4)
	{	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
		/*  config GPIOA_15 as TIME2_CH1 */	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
		
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		
	}else if(TIMx==TIM5){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
		/*  config GPIOA_15 as TIME2_CH1 */	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 |GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; //  | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
		
		GPIO_ResetBits(GPIOB,M_S_2_ENPin);   //  open clock 2 Enable
	}

  /* Compute the prescaler value */	
	TIM_Cmd(TIMx, DISABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler=41;   //  预分频器   2MHz
	TIM_TimeBaseStructure.TIM_Period=10000;		 //  重载寄存器
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);	   //	

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIMx, &TIM_OCInitStructure);

//  TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan1;

  TIM_OC2Init(TIMx, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan1;

  TIM_OC3Init(TIMx, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = StartZhan1;

  TIM_OC4Init(TIMx, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIMx, ENABLE);

  /* TIMx enable counter */
  TIM_Cmd(TIMx, ENABLE);
}
/* 
*		 Port   选择电机 
*    Speed  速度   正数表示正转   负数表示反转
**/
void ChangeSpeed(uint8_t Port,int32_t Speed)
{
	if(Speed>4000)  Speed=4000;
	if(Speed<-4000) Speed=-4000;
	switch(Port)
	{
		case 1:
			TIM3->CCR1 = Speed >0 ? Speed:0;
		  TIM3->CCR2 = Speed >0 ? 0:-Speed;
			break;
		case 2:
			TIM3->CCR3 = Speed >0 ? Speed:0;
		  TIM3->CCR4 = Speed >0 ? 0:-Speed;
			break;
		case 3:
			TIM2->CCR1 = Speed >0 ? Speed:0;
		  TIM2->CCR2 = Speed >0 ? 0:-Speed;
			break;
		case 4:		
			TIM2->CCR3 = Speed >0 ? Speed:0;
		  TIM2->CCR4 = Speed >0 ? 0:-Speed;
			break;
	}
}
/* 
*	 	 Port   选择舵机接口
*		 angle  转动角度   0 -- 180 
**/
void ChangeAngle(uint8_t Port,int32_t angle)  
{
	float buff;
	uint32_t lons;
	if(angle<0) return;
	buff=4000/180*angle;
	lons=(uint32_t)(buff+1000);
	switch(Port)
	{
		case 1:
			TIM4->CCR1=lons;
			break;
		case 2:
			TIM4->CCR2=lons;
			break;
		case 3:
			TIM4->CCR3=lons;
			break;
		case 4:
			TIM4->CCR4=lons;
			break;
		case 5:
			TIM5->CCR1=lons;
			break;
		case 6:
			TIM5->CCR2=lons;
			break;
		case 7:
			TIM5->CCR3=lons;
			break;
		case 8:
			TIM5->CCR4=lons;
			break;
	}	
}




