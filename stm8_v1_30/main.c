/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include"SHT10.h"
#include<stdio.h>
#include<SHT10_01.h>
// #include"Usart.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


float TempResult;   // 存放 温度数据
float HumResult;   // 存放 湿度数据
float TempSend[2];
float HumpSedn[2];
uint8_t addressUsart;  // 节点地址 存放 变量
uint8_t ReceFLag;     //  串口接受中断使能
uint8_t GetUart1Data[4];
uint8_t sendBuff[10];  // 发送数据缓冲 
uint16_t TempValue;
uint16_t HumValue;
static void init_usart1(void);
void Send(uint8_t dat);
void SendSting(char * str);
void recerve_usart_intdeal(void);
void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0 )
  {   
    if(ReceFLag && GetUart1Data[0]==addressUsart)
      return;  //如果 接收到 地址信息  结束延时 判断等待输出信息
    nCount--;
  }
}
uint8_t daBuff;
uint16_t clockTime;
BitStatus bytes;
void main(void)
{   
  uint8_t i,fgs;

  /* Initialize I/Os in Output Mode */
  for(i=100;i>0;i--)
  {
    i=i;
  }
  init_usart1();   // 输入判断
  
  ReceFLag=0;
  sendBuff[0]=0x0A; // ID 
  sendBuff[1]=0x23;
  sendBuff[2]=0x35;
  sendBuff[3]=0xff;
  sendBuff[4]=0xff;
  sendBuff[5]=0;
  sendBuff[6]=0xA0;
  fgs=0;
  sendBuff[7]=0xFA;
  for(i=0;i<5;i++)
  {
    sendBuff[5]+=sendBuff[i];
  }
  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);// 输出 模式 用于Trag 
  GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);// 输出 模式 用于Trag 
  GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
  // GPIO_WriteLow(GPIOA, GPIO_PIN_3);                       // Let it is LowLave
  
  clockTime=CLK_GetClockFreq();
  //GPIO_Init(GPIOD,GPIO_PIN_2,GPIO_MODE_IN_PU_IT);    
  GPIO_Init(GPIOA,GPIO_PIN_1,GPIO_MODE_IN_PU_NO_IT);       // 输入包含  上拉  中断 
  // EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA,EXTI_SENSITIVITY_RISE_ONLY); 
  // CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,DISABLE);
  enableInterrupts();
  fgs=0;
  while (1)
  {
//    bytes=GPIO_ReadInputPin(GPIOA,GPIO_PIN_1);
//    if(bytes==RESET)
//    {
//      UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//      // enableInterrupts();
//    }else{
//      UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
//      // disableInterrupts();
//    }
//////    if(ReceFLag==1)
//////    {
////////      if(GPIO_ReadInputPin(GPIOA,GPIO_PIN_1)==RESET)
////////      {
//////        GPIO_WriteReverse(GPIOA,GPIO_PIN_3);     
//////        for(i=0;i<8;i++)
//////        {  
//////            Send(sendBuff[i]);
//////        }        
////////      }
//////      UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
//////      ReceFLag=0;
//////    }
    
    bytes=GPIO_ReadInputPin(GPIOA,GPIO_PIN_1);
    if(bytes==RESET && fgs==0)    
    {
        fgs=1;  
        GPIO_WriteReverse(GPIOA,GPIO_PIN_3);  
        for(i=0;i<8;i++)
        {  
          Send(sendBuff[i]);
         }
    }else if(bytes!=RESET)
    {
      fgs=0;
    }
    
   }
  
}
void SendTX(void)
{
  uint8_t i;
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,ENABLE);
  for(i=0;i<8;i++)
  {  
    
     Send(sendBuff[i]);
  }
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1,DISABLE);
}
static void init_usart1(void)
{
  UART1_DeInit();
  /* 波特率 115200 8位数据  1个停止位 。  无奇偶校验  */
  UART1_Init((u32)115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,\
           UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
// UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
 // UART1_ClearITPendingBit(UART1_IT_RXNE);     // 清理中断标志位 
}
void recerve_usart_intdeal(void)
{
  uint8_t i;
  if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_2)!=RESET) 
  {
    ReceFLag=0;
    return ;
  } 
  for(i=0;i<8;i++)
  {
     Send(sendBuff[i]);
  }
  ReceFLag=0;
}
void Send(uint8_t dat)
{
  while(( UART1_GetFlagStatus(UART1_FLAG_TXE)==RESET));	
    UART1_SendData8(dat);	
}
void SendSting(char * str)
{
  while(*str++)
  {
    Send(*str);
  }
}
///*
// *   fputc
// */
//int fputc(int ch, FILE *f)
//{
//	/* ??Printf?úèY·￠íù′??ú */
//    UART1_SendData8((uint8_t) ch);
//    while(( UART1_GetFlagStatus(UART1_FLAG_TXE)!=SET));	
//    return (ch);
//}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

