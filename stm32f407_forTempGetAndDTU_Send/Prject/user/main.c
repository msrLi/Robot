/**
  ******************************************************************************
  * @file    Audio_playback_and_record/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    28-October-2011
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
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include"includes.h"
#include"bsp.h"
extern uint32_t ItUp,ItDown;
/** @addtogroup STM32F4-Discovery_Audio_Player_Recorder
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OS_TCB AppDateConnectedTCB;    // 任务时间块 
static CPU_STK AppTaskDateConnectedStk[APP_CFG_TASK_DATE_CONNECTED_STK_SIZE];  // 任务堆栈

static OS_TCB AppDateDtuDealyTCB;    // 任务时间块 
static CPU_STK AppTaskDateDtuDealyStk[APP_CFG_TASK_DATE_DTUDEALY_STK_SIZE];  // 任务堆栈

static OS_TCB AppTaskStartTCB;    // 任务时间块 
static CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];   // 任务堆栈 

static  OS_TCB   AppTaskGUITCB;   // 
static  CPU_STK  AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE];

// static  OS_TCB   AppTaskGUIDrawingTCB;
// static  CPU_STK  AppTaskGUIDrawingStk[APP_CFG_TASK_GUIDRAWING_STK_SIZE];

static  OS_TCB   AppTaskGUIRefreshTCB;
static  CPU_STK  AppTaskGUIRefreshStk[APP_CFG_TASK_GUIRefresh_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void AppTaskCreate (void);
static void AppTaskStart(void *p_arg);
void  AppTaskCreate(void)
{
	OS_ERR      err;	 // 
  /* 创建GUI 任务*/
//	OSTaskCreate((OS_TCB       *)&AppTaskGUITCB,              
//               (CPU_CHAR     *)"App Task GUI",
//               (OS_TASK_PTR   )AppTaskGUI, 
//               (void         *)0,
//               (OS_PRIO       )APP_CFG_TASK_GUI_PRIO,
//               (CPU_STK      *)&AppTaskGUIStk[0],
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE / 10,
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUI_STK_SIZE,
//               (OS_MSG_QTY    )0,
//               (OS_TICK       )8,
//               (void         *)0,
//               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//               (OS_ERR       *)&err);
	/* 创建 画图 GUI  任务 */ 
//	OSTaskCreate((OS_TCB       *)&AppTaskGUIDrawingTCB,              
//               (CPU_CHAR     *)"App Task GUI Drawing",
//               (OS_TASK_PTR   )AppTaskGUIDrawing, 
//               (void         *)0,
//               (OS_PRIO       )APP_CFG_TASK_GUIDrawing_PRIO,
//               (CPU_STK      *)&AppTaskGUIDrawingStk[0],
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUIDRAWING_STK_SIZE / 10,
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUIDRAWING_STK_SIZE,
//               (OS_MSG_QTY    )0,
//               (OS_TICK       )8,
//               (void         *)0,
//               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//               (OS_ERR       *)&err);							 
	/**************创建GUI刷新任务*********************/			 
//	OSTaskCreate((OS_TCB       *)&AppTaskGUIRefreshTCB,              
//               (CPU_CHAR     *)"App Task GUIRefresh",
//               (OS_TASK_PTR   )AppTaskGUIRefresh, 
//               (void         *)0,
//               (OS_PRIO       )APP_CFG_TASK_GUIRefresh_PRIO,
//               (CPU_STK      *)&AppTaskGUIRefreshStk[0],
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE / 10,
//               (CPU_STK_SIZE  )APP_CFG_TASK_GUIRefresh_STK_SIZE,
//               (OS_MSG_QTY    )0,
//               (OS_TICK       )8,
//               (void         *)0,
//               (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//               (OS_ERR       *)&err);
	/**************创建串口读取温度湿度处理函数********/
	OSTaskCreate((OS_TCB *) &AppDateConnectedTCB,
							(CPU_CHAR *)"App Task Date Connected",
							(OS_TASK_PTR) AppTaskDateConnected,      //函数入口
							(void *) 0,
							(OS_PRIO ) APP_CFG_TASK_DATE_CONNECTED_PRIO,
							(CPU_STK * )&AppTaskDateConnectedStk[0],
							(CPU_STK_SIZE ) APP_CFG_TASK_DATE_CONNECTED_STK_SIZE/10,
							(CPU_STK_SIZE ) APP_CFG_TASK_DATE_CONNECTED_STK_SIZE,
							(OS_MSG_QTY)0,
							(OS_TICK)  0,
							(void *) 0,
							(OS_OPT )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							(OS_ERR *)&err);
	/**************创建DTU 串口处理函数 ********/
	OSTaskCreate((OS_TCB *) &AppDateDtuDealyTCB,
							(CPU_CHAR *)"App Task Date Connected",
							(OS_TASK_PTR) AppTaskDTUDealy,      //函数入口
							(void *) 0,
							(OS_PRIO ) APP_CFG_TASK_DTUDealy_PRIO,
							(CPU_STK * )&AppTaskDateDtuDealyStk[0],
							(CPU_STK_SIZE ) APP_CFG_TASK_DATE_DTUDEALY_STK_SIZE/10,
							(CPU_STK_SIZE ) APP_CFG_TASK_DATE_DTUDEALY_STK_SIZE,
							(OS_MSG_QTY)0,
							(OS_TICK)  0,
							(void *) 0,
							(OS_OPT )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							(OS_ERR *)&err);
}
/**
  * @brief  Main program.
  * @param  None
  * @retval None     
*/
int main(void)
 { 
	OS_ERR err;
	
	RCC_ClocksTypeDef Rcc_get;   // 各路时钟 
	RCC_GetClocksFreq(&Rcc_get);
  OSInit(&err);
	OSTaskCreate((OS_TCB *) &AppTaskStartTCB,
							(CPU_CHAR *)"App Task Start",
							(OS_TASK_PTR) AppTaskStart,
							(void *) 0,
							(OS_PRIO ) APP_CFG_TASK_START_PRIO,
							(CPU_STK * )&AppTaskStartStk[0],
							(CPU_STK_SIZE ) APP_CFG_TASK_START_STK_SIZE/10,
							(CPU_STK_SIZE ) APP_CFG_TASK_START_STK_SIZE,
							(OS_MSG_QTY)0,
							(OS_TICK)   0,
							(void *)    0,
							(OS_OPT )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							(OS_ERR *)&err);
	OSStart(&err);
  (void)&err;
	return (0);
	
}
int Countertimes;
static void AppTaskStart(void *p_arg)
{
		OS_ERR   err;
	  // uint16_t Buff[20]={"nihoa\r\n"};
	  static uint8_t itUps,itDowns;
    GPIO_InitTypeDef GPIO_InitStructure;
	  static uint8_t flages=0;
	  uint8_t daBuf;
	  uint8_t TxBuff[8],i;
		(void)p_arg;
	  BSP_Init();	
	  CPU_Init();
	  BSP_Tick_Init();

  	Mem_Init();                             
    Math_Init();  
	
#if OS_CFG_STAT_TASK_EN > 0u
     OSStatTaskCPUUsageInit(&err);   
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
//	Usart1_init();
//	GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  AppTaskCreate();

//	TxBuff[0]=0x1A;
//	TxBuff[1]=0x32;
//	TxBuff[2]=0xFF;
//	TxBuff[3]=0xff;
//	TxBuff[4]=0xff;
//	TxBuff[5]=0;
//	TxBuff[6]=0xA0;
//	TxBuff[7]=0xFA;
//	for(i=0;i<5;i++)
//	{
//		TxBuff[5]+=TxBuff[i];
//	}

//	
//	LCD_ShowString(100,100,(const u8 *)"-----++++");
//	LCD_ShowString(100,100,(const u8 *)"         ");
//	ShowNumber(10,10,-800);
//	ShowNumberFloat(200,10,-809.3232);
		while(1)
		{
//			daBuf=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
//			if(daBuf==0 && flages==0)
//			{
//				flages=1;		
//				for(i=0;i<8;i++)
//				{
//					Usart2_sendChar(TxBuff[i]);
//				}
//			}else if(daBuf!=0){
//				flages=0;				
//			}
			// USART3_Puts("I am OK to usatrt3");
			
//		  Trage_Led();
			// LCD_ShowString(30,40,"luihairui nidaye ");

//			LCD_ShowString(150,150,"Up");
//       ShowNumber(180,150,(int) ItUp);//ItUp
			
			// Usart2_sendChar(0xA0);
			OSTimeDlyHMSM((CPU_INT16U) 0u,
										(CPU_INT16U) 0u,
										(CPU_INT16U) 10,
										(CPU_INT32U) 500,
										(OS_OPT ) OS_OPT_TIME_HMSM_STRICT,
										(OS_ERR   *) &err);
			// GPIO_ToggleBits(GPIOC,GPIO_Pin_0);
			//Countertimes=(int)(TIM_GetCounter(TIM2));
			//Countertimes=Countertimes;
		
		}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
