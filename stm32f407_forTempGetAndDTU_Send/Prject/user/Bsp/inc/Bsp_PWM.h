
#ifndef _Bsp_PWM_H_
#define _Bsp_PWM_H_
#include"stm32f4xx.h"
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
void InitPWMAs20kHz(TIM_TypeDef* TIMx);
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
void InitPWMAs200Hz(TIM_TypeDef* TIMx);
/* 
*		 Port   选择电机 
*    Speed  速度   正数表示正转   负数表示反转
*          <-4000,0,4000>  
**/
void ChangeSpeed(uint8_t Port,int32_t Speed);
/* 
*	 	 Port   选择舵机接口
*		 angle  转动角度
**/
/* 
*	 	 Port   选择舵机接口
*		 angle  转动角度
**/
void ChangeAngle(uint8_t Port,int32_t angle);
#endif 
