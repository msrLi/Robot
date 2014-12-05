#include"Task_DtuDely.h"
//#define MaxSendBuffLen  400      // 串口发送 最大缓冲数据数量
//OS_SEM  Usart1RecerveSem;       // 串口1  信号量 
//OS_SEM  Usart3RecerveSem;       //  串口3 同步信号量
//uint8_t rece1_buff[Max_date1];  // 串口数据缓冲 
//uint16_t USART1_RX_STA=0;     //串口接收结束标志和 接收数量存放位置
//uint8_t DTUOkFlag=0;          // DTU 状态 指示标志 

uint32_t congdad;

void AppTaskDTUDealy(void *p_arg)
{
	OS_ERR     err;
  uint8_t times;
//	GPIO_InitTypeDef GPIO_InitStructure;
  (void) p_arg;
	  
	/* 创建 信号量 用与同步两个进程 */
	// 用于同步  cn=0;
//	OSSemCreate((OS_SEM    *)&Usart3RecerveSem,
//							(CPU_CHAR  *)"Usart3RecerveSem",
//							(OS_SEM_CTR )0,
//							(OS_ERR    *)&err);	

	// Usart3_init();
//	Usart3RecerveStart=0;
	while(1)
	{
//		OSSemPend((OS_SEM *)&Usart3RecerveSem,
//					(OS_TICK )0,
//					(OS_OPT  )OS_OPT_PEND_BLOCKING,
//					(CPU_TS  )0,
//					(OS_ERR *)&err);     	
//		 //开启中断 

		OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) 1,
                  (CPU_INT32U) 400,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);
//		while(1)
//		{
//			congdad++;
//			times++; 
//			if(times==0xfe)
//			{
//				times=0;
//			}
//		}		
	}
}

